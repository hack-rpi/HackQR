#ifndef _QR_H_
#define _QR_H_

#include <map>
#include <string>
#include <vector>

#include "util.h"

// =============================================================================
// Encoding scheme abstract class
template <class T>
class EncodingScheme {
public:
  virtual size_t getModeIndicator() const = 0;
  virtual size_t getLength() const = 0;
  virtual T getMessage() const = 0;
  
protected:
  T message_;
  std::vector<util::Binary> encoded_message_;
  size_t length_;
  size_t version_;
  size_t char_count_indicator_;
};

// =============================================================================
// Alphanumeric encoding scheme
class Alphanumeric: public EncodingScheme<std::string> {
public:
  const static size_t mode_indicator_ = 0b0010;
  const static std::map<char,size_t> encoding_table_;
  Alphanumeric(const std::string& message, size_t version) {
    message_ = message;
    length_ = message.size();
    version_ = version;
    if (version_ > 0 && version_ < 10) {
      char_count_indicator_ = 9;
    } else if (version_ >= 10 && version_ < 27) {
      char_count_indicator_ = 11;
    } else if (version_ >= 27 && version_ <= 40) {
      char_count_indicator_ = 13;
    } else {
      throw "Invalid version";
    }
    size_t i = 0, k = 0, n = length_ % 2 == 0 ? length_ / 2 : (length_ / 2) + 1;
    encoded_message_ = std::vector<util::Binary>(n, util::Binary());
    while (i < n) {
      if (i == n - 1 && length_ % 2 == 1) {
        encoded_message_[i] = util::Binary(encoding_table_.at(message_[k]), 6);
      }
      else {
        size_t val = 45 * encoding_table_.at(message_[k]) 
          + encoding_table_.at(message_[k+1]);
        encoded_message_[i] = util::Binary(val, 11);
      }
      i += 1;
      k += 2;
    }
  }
  
  size_t getModeIndicator() const {
    return Alphanumeric::mode_indicator_;
  }
  
  size_t getCharCountIndicator() const {
    return char_count_indicator_;
  }
  
  size_t getLength() const {
    return length_;
  }
  
  std::string getMessage() const {
    return message_;
  }
  
  std::vector<util::Binary> getEncodedMessage() const {
    return encoded_message_;
  }
};

// =============================================================================
// Templated QR Class
template <class T>
class QR {
public:
  QR(const char EC, const size_t version) {
    ec_level_ = EC;
    version_ = version;
  };
  ~QR() {};
  
  std::vector<util::GF_int> getGenerator(size_t n) {
    if (n == 1) {
      std::vector<util::GF_int> result;
      result.push_back(*GF_.log(0));
      result.push_back(*GF_.log(0));
      return result;
    } else {
      std::vector<util::GF_int> factor;
      factor.push_back(*GF_.log(0));
      factor.push_back(*GF_.log(n-1));
      return util::multiplyPoly(QR<T>::getGenerator(n-1), factor);
    }
  }
  
  void Encode(const T& encoding) {
    
    size_t total_codewords = getTotalCodewords();
    
    std::vector<util::Binary> f_encoding,
      encoded_msg = encoding.getEncodedMessage();
    std::string bits = "", this_bit = "";
    
    bits += util::Binary(encoding.getModeIndicator(), 4).getString();
    bits += util::Binary(encoding.getLength(), 
      encoding.getCharCountIndicator()).getString();
    
    for (size_t i=0; i<encoded_msg.size(); i++)
      bits += encoded_msg[i].getString();
    
    size_t total_bits = total_codewords * 8;
    // divide into bytes of 8 bits each
    for (size_t b=0; b<bits.size(); b++) {
      if (b > 0 && b % 8 == 0) {
        f_encoding.push_back(util::Binary(this_bit));
        this_bit = "";
      }
      this_bit += bits[b];
    }
    
    size_t terminators = 0;
    while ( f_encoding.size() * 8 < total_bits && terminators < 4 ) {
      this_bit += "0";
      if (this_bit.size() == 8) {
        f_encoding.push_back(util::Binary(this_bit));
        this_bit = "";
      }
      terminators++;
    }
    
    while (this_bit.size() != 0 && this_bit.size() < 8) this_bit += "0";
    if (this_bit.size())
      f_encoding.push_back(util::Binary(this_bit));
      
    size_t p = 0;
    while (f_encoding.size() < total_codewords) {
      if (p % 2 == 0) {
        f_encoding.push_back(util::Binary(QR::PAD_BYTES1_));
      } else {
        f_encoding.push_back(util::Binary(QR::PAD_BYTES2_));
      }
      p++;
    }
    
    size_t ec_words_per_block = getECWordsPerBlock(),
           group_1_blocks = getGroup1Blocks(),
           group_2_blocks = getGroup2Blocks(),
           total_ec_words = (group_1_blocks + group_2_blocks) 
            * ec_words_per_block;
    std::vector<util::Binary> ec_words(total_ec_words, util::Binary());    
    std::vector<util::GF_int> generator = getGenerator(total_ec_words);
    
    std::vector<util::GF_int> tmp, tmp2, tmp3;
    for (size_t i=0; i<total_codewords; i++)
      tmp.push_back(*GF_.antilog( f_encoding[i].getInt() ));
    
    // generate the error correction words
    for (size_t i=0; i<total_codewords; i++) {     
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
      std::cout << ec_words[i].getInt() << " ";
    }
    std::cout << std::endl;
    
    
  }
  
  
private:
  const static util::GaloisField GF_;
  const static util::Binary PAD_BYTES1_;
  const static util::Binary PAD_BYTES2_;
  const static std::map<std::string, std::string> EC_codewords_;
  char ec_level_;
  char version_;
  
  size_t getTotalCodewords() {
    std::string tmp(1, version_); tmp += '-'; tmp += ec_level_; 
    std::string ec_data = EC_codewords_.at(tmp);
    size_t sep = ec_data.find(":");
    std::string result_str = ec_data.substr(0,sep);
    size_t result = std::stoi(result_str);
    return result;
  }
  
  size_t getECWordsPerBlock() {
    std::string tmp(1, version_); tmp += '-'; tmp += ec_level_; 
    std::string ec_data = EC_codewords_.at(tmp);
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
  
  size_t getGroup1Blocks() {
    std::string tmp(1, version_); tmp += '-'; tmp += ec_level_; 
    std::string ec_data = EC_codewords_.at(tmp);
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
  
  size_t getGroup1WordsPerBlock() {
    std::string tmp(1, version_); tmp += '-'; tmp += ec_level_; 
    std::string ec_data = EC_codewords_.at(tmp);
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
  
  size_t getGroup2Blocks() {
    std::string tmp(1, version_); tmp += '-'; tmp += ec_level_; 
    std::string ec_data = EC_codewords_.at(tmp);
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
  
  size_t getGroup2WordsPerBlock() {
    std::string tmp(1, version_); tmp += '-'; tmp += ec_level_; 
    std::string ec_data = EC_codewords_.at(tmp);
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
  
};

#include "qr_const.hpp"

#endif
