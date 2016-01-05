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
  const static bool MASK_RULE_0_(size_t x, size_t y);
  const static bool MASK_RULE_1_(size_t x, size_t y);
  const static bool MASK_RULE_2_(size_t x, size_t y);
  const static bool MASK_RULE_3_(size_t x, size_t y);
  const static bool MASK_RULE_4_(size_t x, size_t y);
  const static bool MASK_RULE_5_(size_t x, size_t y);
  const static bool MASK_RULE_6_(size_t x, size_t y);
  const static bool MASK_RULE_7_(size_t x, size_t y);
  const static std::vector<const bool (*)(size_t, size_t)> MASKS_;
  const static std::vector<bool> BAD_PATTERN_0_;
  const static std::vector<bool> BAD_PATTERN_1_;
  
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
  void drawEncodedData(Image<bool>& image, Image<bool>& set_pixels) const;
  void applyMaskPattern(Image<bool>& image, const Image<bool>& set_pixels) 
    const;
  size_t calculatePenalty(const Image<bool>& image) const;
  
  // Constant Static Accessors
  size_t getTotalCodewords() const;
  size_t getECWordsPerBlock() const;
  size_t getGroup1Blocks() const;
  size_t getGroup1WordsPerBlock() const;
  size_t getGroup2Blocks() const;
  size_t getGroup2WordsPerBlock() const;  
};

#endif
