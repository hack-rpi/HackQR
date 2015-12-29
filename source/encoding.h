#ifndef _ENCODING_H_
#define _ENCODING_H_

#include <map>
#include <string>
#include <vector>

#include "util.h"

// =============================================================================
// Encoding scheme abstract class
class EncodingScheme {
public:
  virtual size_t getModeIndicator() const = 0;
  virtual std::vector<util::Binary> getEncodedMessage() const = 0;
  size_t getCharCountIndicator() const {
    return char_count_indicator_;
  }
  size_t getLength() const {
    return length_;
  }
  
protected:
  size_t length_;
  size_t version_;
  size_t char_count_indicator_;
};

// =============================================================================
// Alphanumeric encoding scheme
class Alphanumeric: public EncodingScheme {
public:
  // Static Members
  const static size_t mode_indicator_ = 0b0010;
  const static std::map<char, size_t> encoding_table_;
  
  // Constructors
  Alphanumeric(const std::string& message, size_t version);
  
  // Accessors
  size_t getModeIndicator() const;
  std::string getMessage() const;
  std::vector<util::Binary> getEncodedMessage() const;

private:
  std::string message_;
};

#endif
