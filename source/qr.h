#ifndef _QR_H_
#define _QR_H_

#include <string>

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
  Alphanumeric(const std::string& message, size_t version);
  
  size_t getModeIndicator() const;
  size_t getLength() const;
  std::string getMessage() const;
};

// =============================================================================
// Templated QR Class
template <class T>
class QR {
public:
  QR(const char EC, const size_t version);
  ~QR();
  
  void Encode(const T& encoding);
  
private:
  char ec_level_;
  size_t version_;
};

#endif
