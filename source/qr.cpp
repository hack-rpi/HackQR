#include "qr.h"

QR::QR(const char EC, const int version) {
  ec_level_ = EC;
  version_ = version;
  size_t size = ((version_ - 1) * 4) + 21;
  image_.Allocate(size, size);
  image_.SetAllPixels(false);
};

QR::~QR() {};
  
void QR::Encode(const EncodingScheme& encoding) {    
  std::vector<util::Binary> f_encoding = getEncodedData(encoding);
  size_t ec_words_per_block = getECWordsPerBlock(),
          num_group_1_blocks = getGroup1Blocks(),
          group_1_words_per_block = getGroup1WordsPerBlock(),
          num_group_2_blocks = getGroup2Blocks(),
          group_2_words_per_block = getGroup2WordsPerBlock();
  
  // divide the data codewords into groups and blocks as required by the 
  //  version number
  std::vector< std::vector<util::Binary> > group1, group2;
  size_t w = 0;
  for (size_t i=0; i<num_group_1_blocks; i++) {
    std::vector<util::Binary> block;
    for (size_t k=0; k<group_1_words_per_block; k++, w++) {
      block.push_back( f_encoding[w] );
    }
    group1.push_back(block);
  }
  for (size_t i=0; i<num_group_2_blocks; i++) {
    std::vector<util::Binary> block;
    for (size_t k=0; k<group_2_words_per_block; k++, w++) {
      block.push_back( f_encoding[w] );
    }
    group2.push_back(block);
  }
  
  // generate the error correction words seperately for each block 
  std::vector< std::vector<util::Binary> > ECgroup1, ECgroup2;    
  for (size_t g=0; g<group1.size(); g++) {
    ECgroup1.push_back(getErrorCorrectionWords(ec_words_per_block, 
      group1[g]));
  }
  for (size_t g=0; g<group2.size(); g++) { 
    ECgroup2.push_back(getErrorCorrectionWords(ec_words_per_block, 
      group2[g]));
  }
  
  // interweave the data and error correction code words to get the final 
  //  representation of the message data 
  w = 0;
  encoded_data_.clear();
  while (w < group_1_words_per_block || w < group_2_words_per_block) {
    if (w < group_1_words_per_block) {
      for (size_t i=0; i<group1.size(); i++) { 
        encoded_data_.push_back(group1[i][w]);
      }
    }
    if (w < group_2_words_per_block) {
      for (size_t i=0; i<group2.size(); i++) {
        encoded_data_.push_back(group2[i][w]);
      }
    }
    w++;
  }    
  w = 0;
  while (w < ec_words_per_block) {
    for (size_t i=0; i<ECgroup1.size(); i++) { 
      encoded_data_.push_back(ECgroup1[i][w]);
    }
    for (size_t i=0; i<ECgroup2.size(); i++) {
      encoded_data_.push_back(ECgroup2[i][w]);
    }
    w++;
  }
  
  // add the remainder bits specified by the version
  size_t r_bits = REMAINDER_BITS_.at(version_);
  if (r_bits > 0) 
    encoded_data_.push_back(util::Binary("0", r_bits));
  
  // create the QR bitmap
  createBitmap();
}

void QR::Save(const std::string& filename) const {
  image_.Save(filename);
}

// =============================================================================

std::vector<util::GF_int> QR::getGenerator(size_t n) const {
  if (n == 1) {
    std::vector<util::GF_int> result;
    result.push_back(*GF_.log(0));
    result.push_back(*GF_.log(0));
    return result;
  } else {
    std::vector<util::GF_int> factor;
    factor.push_back(*GF_.log(0));
    factor.push_back(*GF_.log(n-1));
    return util::multiplyPoly(getGenerator(n-1), factor);
  }
}
  
std::vector<util::Binary> QR::getEncodedData(const EncodingScheme& encoding)        const {
  size_t total_codewords = getTotalCodewords();
  std::vector<util::Binary> f_encoding,
                            encoded_msg = encoding.getEncodedMessage();
  std::string bits = "", this_bit = "";
  // start by adding the mode indicator and character count indicator
  bits += util::Binary(encoding.getModeIndicator(), 4).getString();
  bits += util::Binary(encoding.getLength(), 
    encoding.getCharCountIndicator()).getString();
  // add the binary representation of the message
  for (size_t i=0; i<encoded_msg.size(); i++)
    bits += encoded_msg[i].getString();
  // divide the encoded data into bytes
  size_t total_bits = total_codewords * 8;
  for (size_t b=0; b<bits.size(); b++) {
    if (b > 0 && b % 8 == 0) {
      f_encoding.push_back(util::Binary(this_bit));
      this_bit = "";
    }
    this_bit += bits[b];
  }
  // add terminators ("0"), max of 4, if less than the required length    
  size_t terminators = 0;
  while ( f_encoding.size() * 8 < total_bits && terminators < 4 ) {
    this_bit += "0";
    if (this_bit.size() == 8) {
      f_encoding.push_back(util::Binary(this_bit));
      this_bit = "";
    }
    terminators++;
  }
  // add "0" until the last byte has 8 bits
  while (this_bit.size() != 0 && this_bit.size() < 8) this_bit += "0";
  if (this_bit.size())
    f_encoding.push_back(util::Binary(this_bit));
  // add the padding bytes until the required length is met      
  size_t p = 0;
  while (f_encoding.size() < total_codewords) {
    if (p % 2 == 0) {
      f_encoding.push_back(util::Binary(QR::PAD_BYTES1_));
    } else {
      f_encoding.push_back(util::Binary(QR::PAD_BYTES2_));
    }
    p++;
  }
  return f_encoding;
}
  
std::vector<util::Binary> QR::getErrorCorrectionWords(
    const size_t& total_ec_words, 
    const std::vector<util::Binary>& data_words) const {
  std::vector<util::Binary> ec_words(total_ec_words, util::Binary());    
  std::vector<util::GF_int> generator = getGenerator(total_ec_words),
                            tmp, tmp2, tmp3;
  for (size_t i=0; i<data_words.size(); i++) {
    tmp.push_back( *GF_.antilog( data_words[i].getInt() ) );
  }
  // generate the error correction words
  for (size_t i=0; i<data_words.size(); i++) {     
    // Multiply the Generator Polynomial by the Lead Term of the 
    //    Message Polynomial
    for (size_t k=0; k<generator.size(); k++) {
      tmp3.push_back(tmp[0] * generator[k]);
    }
    // XOR the result with the message polynomial
    size_t k_max = tmp.size() > tmp3.size() ? tmp.size() : tmp3.size(); 
    for (size_t k=1; k<k_max; k++) {
      if (k >= tmp3.size()) { 
        tmp2.push_back( tmp[k] );
      } else if (k >= tmp.size()) {
        tmp2.push_back( tmp3[k] );
      } else if (tmp[k].getN() == tmp3[k].getN()) {
        tmp2.push_back( *GF_.antilog(0) );
      } else {
        tmp2.push_back( tmp[k] ^ tmp3[k] );
      }
    }
    tmp = tmp2;
    tmp2.clear();
    tmp3.clear();
  } // end error correction word gen
  for (size_t i=0; i<tmp.size(); i++) {
    ec_words[i] = util::Binary( tmp[i].getValue() );
  }
  return ec_words;
}
 
void QR::createBitmap() {
  Image<bool> set_pixels;
  set_pixels.Allocate(image_.Width(), image_.Height());
  set_pixels.SetAllPixels(false);
  drawFunctionPatterns(image_, set_pixels);
  return;
}

void QR::drawFunctionPatterns(Image<bool>& image, Image<bool>& set_pixels) 
    const {
  auto drawFinderPattern = [&image, &set_pixels](int x, int y) -> void {
    for (int a=x; a<x+7; a++) {
      for (int b=y; b>y-7; b--) {
        if ((a > x && a < x + 6 && (b == y - 1 || b == y - 5)) || 
            (b < y && b > y - 6 && (a == x + 1 || a == x + 5))) {
          image.SetPixel(a, b, false);
        } else {
          image.SetPixel(a, b, true);
        }
        set_pixels.SetPixel(a, b, true);
      }
    }
  };
  
  auto drawHLine = [&image, &set_pixels](int x0, int n, int y, bool val) 
      -> void {
    for (int a=x0; a<x0+n; a++) {
      image.SetPixel(a, y, val);
      set_pixels.SetPixel(a, y, true);
    }
  };
  
  auto drawVLine = [&image, &set_pixels](int y0, int n, int x, bool val) 
      -> void {
    for (int b=y0; b<y0+n; b++) {
      image.SetPixel(x, b, val);
      set_pixels.SetPixel(x, b, true);
    }
  };
  
  auto drawAlignmentPattern = [&image, &set_pixels](int x, int y) -> void {
    for (int a=x; a<x+5; a++) {
      for (int b=y; b>y-5; b--) {
        if (a == x || b == y || a == x + 4 || b == y - 4 || 
            (a == x + 2 && b == y - 2)) {
          image.SetPixel(a, b, true);
        } else {
          image.SetPixel(a, b, false);
        }
        set_pixels.SetPixel(a, b, true);
      }
    }
  };
  
  // draw the finder patterns
  drawFinderPattern(0, image.Height() - 1);
  drawFinderPattern(0, 6);
  drawFinderPattern(image.Width() - 7, image.Height() - 1);
  
  // draw the separators
  drawHLine(0, 8, 7, false);
  drawVLine(0, 8, 7, false);
  drawHLine(0, 8, image.Height() - 8, false);
  drawVLine(image.Height() - 8, 8, 7, false);
  drawHLine(image.Width() - 8, 8, image.Height() - 8, false);
  drawVLine(image.Height() - 8, 8, image.Width() - 8, false);
  
  // draw the alignment patterns
  if (version_ > 1) {
    std::vector<int> locs = ALIGNMENT_LOCS_.at(version_);
    for (size_t i=0; i<locs.size(); i++) {
      for (size_t k=0; k<locs.size(); k++) {
        if (! set_pixels.GetPixel(locs[i], image.Height() - locs[k] - 1))
          drawAlignmentPattern(locs[i] - 2, image.Height() - locs[k] - 1 + 2);
      }
    }
  }
  
  // draw the timing patterns
  bool val = true;
  for (size_t a=6; a<image.Width()-8; a++) {
    image.SetPixel(a, image.Height() - 7, val);
    val = !val;
    set_pixels.SetPixel(a, image.Height() - 7, val);
  }
  val = true;
  for (size_t b=6; b<image.Height()-8; b++) {
    image.SetPixel(6, b, val);
    val = !val;
    set_pixels.SetPixel(6, b, true);
  }
  
  // draw the dark module
  image.SetPixel(8, 7, true);
  set_pixels.SetPixel(8, 7, true);
  
  // mark the reserved areas
  if (version_ < 7) {
    drawVLine(0, 7, 8, false);
    drawHLine(image.Width() - 7, 7, image.Height() - 9, false);
    drawHLine(0, 6, image.Height() - 9, false);
    drawHLine(7, 2, image.Height() - 9, false);
    drawVLine(image.Height() - 6, 6, 8, false);
    drawVLine(image.Height() - 8, 1, 8, false);
  } else {
    drawHLine(0, 6, 8, false);
    drawHLine(0, 6, 9, false);
    drawHLine(0, 6, 10, false);
    drawVLine(image.Height() - 6, 6, image.Width() - 9, false);
    drawVLine(image.Height() - 6, 6, image.Width() - 10, false);
    drawVLine(image.Height() - 6, 6, image.Width() - 11, false);
  }
  
  return;
}
  
 // ============================================================================
  
size_t QR::getTotalCodewords() const {
  char buf[50];
  sprintf(buf, "%d", version_);
  std::string tmp(buf); tmp += '-'; tmp += ec_level_; 
  std::string ec_data = EC_CODEWORDS_.at(tmp);
  size_t sep = ec_data.find(":");
  std::string result_str = ec_data.substr(0,sep);
  size_t result = std::stoi(result_str);
  return result;
}
  
size_t QR::getECWordsPerBlock() const {
  char buf[50];
  sprintf(buf, "%d", version_);
  std::string tmp(buf); tmp += '-'; tmp += ec_level_; 
  std::string ec_data = EC_CODEWORDS_.at(tmp);
  size_t index = 2, pos = 0, last;
  for (index; index > 0; index--) {
    last = pos;
    pos = ec_data.find(":", pos);
    pos++;
  }
  std::string result_str = ec_data.substr(last,pos);
  size_t result = std::stoi(result_str);
  return result;
}

size_t QR::getGroup1Blocks() const {
  char buf[50];
  sprintf(buf, "%d", version_);
  std::string tmp(buf); tmp += '-'; tmp += ec_level_; 
  std::string ec_data = EC_CODEWORDS_.at(tmp);
  size_t index = 3, pos = 0, last;
  for (index; index > 0; index--) {
    last = pos;
    pos = ec_data.find(":", pos);
    pos++;
  }
  std::string result_str = ec_data.substr(last,pos);
  size_t result = std::stoi(result_str);
  return result;
}

size_t QR::getGroup1WordsPerBlock() const {
  char buf[50];
  sprintf(buf, "%d", version_);
  std::string tmp(buf); tmp += '-'; tmp += ec_level_; 
  std::string ec_data = EC_CODEWORDS_.at(tmp);
  size_t index = 4, pos = 0, last;
  for (index; index > 0; index--) {
    last = pos;
    pos = ec_data.find(":", pos);
    pos++;
  }
  std::string result_str = ec_data.substr(last,pos);
  size_t result = std::stoi(result_str);
  return result;
}

size_t QR::getGroup2Blocks() const {
  char buf[50];
  sprintf(buf, "%d", version_);
  std::string tmp(buf); tmp += '-'; tmp += ec_level_; 
  std::string ec_data = EC_CODEWORDS_.at(tmp);
  size_t index = 5, pos = 0, last;
  for (index; index > 0; index--) {
    last = pos;
    pos = ec_data.find(":", pos);
    pos++;
  }
  std::string result_str = ec_data.substr(last,pos);
  size_t result = std::stoi(result_str);
  return result;
}

size_t QR::getGroup2WordsPerBlock() const {
  char buf[50];
  sprintf(buf, "%d", version_);
  std::string tmp(buf); tmp += '-'; tmp += ec_level_; 
  std::string ec_data = EC_CODEWORDS_.at(tmp);
  size_t index = 6, pos = 0, last;
  for (index; index > 0; index--) {
    last = pos;
    pos = ec_data.find(":", pos);
    if (pos >= ec_data.size() - 1) {
      pos = ec_data.size() - 1;
      break;
    } else {
      pos++;
    }
  }
  std::string result_str = ec_data.substr(last,pos);
  size_t result = std::stoi(result_str);
  return result;
}
