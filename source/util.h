#ifndef _BINARY_H_
#define _BINARY_H_

#include <string>

namespace util
{
  // =============================================================================
  //Binary representation object
  class Binary {
  public:
    Binary(const size_t int_value);
    Binary(const std::string& str_value);

    std::string getString();
    size_t getInt();

  private:
    size_t computeInteger(const std::string& str_value) const;
    std::string computeString(size_t int_value) const;

    size_t integer_value_;
    std::string string_value_;
  };
}
#endif
