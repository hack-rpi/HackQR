#ifndef _QR_H_
#define _QR_H_

#include <cstdio>
#include <map>
#include <string>
#include <vector>

#include "encoding.h"
#include "image.h"
#include "util.h"

// =============================================================================
// QR Class
class QR {
public:
  // Constructors
  QR(const char EC, const int version);
  
  // Deconstructor
  ~QR();
  
  // Modifiers
  void Encode(const EncodingScheme& encoding);
  
  // Accessors
  void Save(const std::string& filename) const;

private:
  // Static Constants
  const static util::GaloisField GF_;
  const static util::Binary PAD_BYTES1_;
  const static util::Binary PAD_BYTES2_;
  const static std::map<std::string, std::string> EC_CODEWORDS_;
  const static std::map<int, size_t> REMAINDER_BITS_;
  const static std::map<int, std::vector<int> > ALIGNMENT_LOCS_;
  
  // Representation
  char ec_level_;
  int version_;
  std::vector<util::Binary> encoded_data_;
  Image<bool> image_;
  
  // Helper Functions
  std::vector<util::GF_int> getGenerator(size_t n) const;
  std::vector<util::Binary> getEncodedData(const EncodingScheme& encoding) 
    const;
  std::vector<util::Binary> getErrorCorrectionWords(
    const size_t& total_ec_words, 
    const std::vector<util::Binary>& data_words) const;
  
  void createBitmap();
  void drawFunctionPatterns(Image<bool>& image, Image<bool>& set_pixels) const;
  
  // Constant Static Accessors
  size_t getTotalCodewords() const;
  size_t getECWordsPerBlock() const;
  size_t getGroup1Blocks() const;
  size_t getGroup1WordsPerBlock() const;
  size_t getGroup2Blocks() const;
  size_t getGroup2WordsPerBlock() const;  
};

#endif
