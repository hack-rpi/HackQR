#ifndef _BINARY_H_
#define _BINARY_H_

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <iostream>

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
    friend GF_int operator*(const GF_int& a, const GF_int& b);
    friend GF_int operator^(const GF_int& a, const GF_int& b);
    friend GF_int operator+(const GF_int& a, const GF_int& b);
  public:
    GaloisField(size_t size, size_t modulus);
    ~GaloisField();
    
    GF_int* log(const size_t n) const;
    GF_int* antilog(const size_t n) const;
  
  private:
    size_t size_;
    size_t modulus_;
    std::vector<GF_int*> log_table_;
    std::map<int, GF_int*> antilog_table_;
    
  };
  
  class GF_int {
    friend GF_int operator*(const GF_int& a, const GF_int& b);
    friend GF_int operator^(const GF_int& a, const GF_int& b);
    friend GF_int operator+(const GF_int& a, const GF_int& b);
  public:
    GF_int();
    GF_int(size_t n, size_t val, GaloisField* field);
    
    size_t getN();
    size_t getValue();
    
  private:
    size_t n_;
    size_t value_;
    GaloisField* field_;
  };
  
  // ===========================================================================
  // Helper Functions
  
  template <typename T>
  std::vector<T> multiplyPoly(const std::vector<T>& a, const std::vector<T>& b) {
    std::vector<T> result(a.size() + b.size() - 1, T());
    std::vector<bool> seen(a.size() + b.size() - 1, false);
    
    size_t an = a.size(), bn = b.size();
    
    for (size_t i=0; i<an; i++) {
      for (size_t k=0; k<bn; k++) {
        T p = a[i] * b[k];
        size_t d = i + k;
        if (seen[d]) {
          result[d] = result[d] + p;
        } else {
          result[d] = p;
          seen[d] = true;
        }
      }
    }
    
    return result;
  }

}
#endif
