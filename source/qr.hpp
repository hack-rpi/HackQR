#ifndef _QR_H_
#define _QR_H_

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
  size_t length_;
  size_t version_;
  size_t char_count_indicator_;
};

// =============================================================================
// Alphanumeric encoding scheme
class Alphanumeric: public EncodingScheme<std::string> {
public:
  const static size_t mode_indicator_ = 0b0010;  
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
  }
  
  size_t getModeIndicator() const {
    return Alphanumeric::mode_indicator_;
  }
  
  size_t getLength() const {
    return length_;
  }
  
  std::string getMessage() const {
    return message_;
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
  
  void Encode(const T& encoding);
  
private:
  static const util::GaloisField GF_;
  char ec_level_;
  size_t version_;
};

template <class T>
const util::GaloisField QR<T>::GF_ = util::GaloisField(256, 285);

#endif
