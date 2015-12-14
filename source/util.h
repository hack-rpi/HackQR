#ifndef _BINARY_H_
#define _BINARY_H_

#include <string>

namespace util
{
  // ===========================================================================
  //Binary representation object
  class Binary {
  public:
    Binary();
    Binary(const size_t int_value);
    Binary(const std::string& str_value);

    // =========================================================================
    // Accessors
    std::string getString() const;
    size_t getInt() const;

    // =========================================================================
    // Mutators
    void addLeftPadding(size_t amount);
    void operator+=(const Binary& nBinary);
    Binary operator+(const Binary& b);


  private:
    size_t computeInteger(const std::string& str_value) const;
    std::string computeString(size_t int_value) const;

    size_t integer_value_;
    std::string string_value_;
  };
}
#endif
