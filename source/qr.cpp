#include "qr.h"

// =============================================================================
// Alphanumeric Class Implementation
Alphanumeric::Alphanumeric(const std::string& message, size_t version) {
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

size_t Alphanumeric::getModeIndicator() const {
  return Alphanumeric::mode_indicator_;
}

size_t Alphanumeric::getLength() const {
  return length_;
}

std::string Alphanumeric::getMessage() const {
  return message_;
}

// =============================================================================
// QR Class Implementation
template <class T>
QR<T>::QR(const char EC, const size_t version) {
  ec_level_ = EC;
  version_ = version;
}

template <class T>
void QR<T>::Encode(const T& encoding) {
  
}
