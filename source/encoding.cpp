#include "encoding.h"

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

std::string Alphanumeric::getMessage() const {
  return message_;
}

size_t Alphanumeric::getModeIndicator() const {
  return Alphanumeric::mode_indicator_;
}

std::vector<util::Binary> Alphanumeric::getEncodedMessage() const {
  size_t i = 0, k = 0, n = length_ % 2 == 0 ? length_ / 2 : (length_ / 2) + 1;
  std::vector<util::Binary> encoded_message(n, util::Binary());
  while (i < n) {
    if (i == n - 1 && length_ % 2 == 1) {
      encoded_message[i] = 
        util::Binary(Alphanumeric::encoding_table_.at(message_[k]), 6);
    }
    else {
      size_t val = 45 * Alphanumeric::encoding_table_.at(message_[k]) 
        + Alphanumeric::encoding_table_.at(message_[k+1]);
      encoded_message[i] = util::Binary(val, 11);
    }
    i += 1;
    k += 2;
  }
  return encoded_message;
}
