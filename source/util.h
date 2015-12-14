#ifndef _BINARY_H_
#define _BINARY_H_

#include <string>
#include <vector>

namespace util
{
  
  // ===========================================================================
  // Binary representation object
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
  
  // ===========================================================================
  // GaloisField and GF_int objects
  class GF_int;
  
  class GaloisField {
    friend class GF_int;
  public:
    GaloisField(size_t size, size_t modulus);
    ~GaloisField();
    
    GF_int* operator[](const size_t n);
  
  private:
    size_t size_;
    size_t modulus_;
    std::vector<GF_int*> members_;
  };
  
  class GF_int {
  public:
    GF_int(size_t n, size_t val, GaloisField* field);
    
    size_t getN();
    size_t getValue();
    
    GF_int operator^(const GF_int& b) const;
    GF_int* operator*(const GF_int& b) const;
    
  private:
    size_t n_;
    size_t value_;
    GaloisField* field_;
  };
  
}
#endif
