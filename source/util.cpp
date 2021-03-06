#include "util.h"
#include <stdlib.h>
#include <fstream>

namespace util
{

  // ===========================================================================
  // Binary Class Implementation

  Binary::Binary(){
    string_value_ = "0";
    integer_value_ = 0;
  }

  Binary::Binary(const std::string& str_value) {
    string_value_ = str_value;
    integer_value_ = computeInteger(str_value);
  }

  Binary::Binary(const std::string& str_value, const size_t binaryLength) {
    string_value_ = str_value;
    integer_value_ = computeInteger(str_value);
    this->addLeftPadding(binaryLength - string_value_.length());
  }

  Binary::Binary(size_t int_val) {
    string_value_ = computeString(int_val);
    integer_value_ = int_val;
  }

  Binary::Binary(const size_t int_value, const size_t binaryLength) {
    string_value_ = computeString(int_value);
    integer_value_ = int_value;
    this->addLeftPadding(binaryLength - string_value_.length());
  }

  std::string Binary::getString() const{
    return string_value_;
  }

  size_t Binary::getInt() const{
    return integer_value_;
  }

  void Binary::addLeftPadding(size_t amount) {
    std::string toAdd = "";
    for (int i = 0; i < amount; i++) {
      toAdd += "0";
    }
    string_value_ = toAdd + string_value_;
  }

  void Binary::operator+=(const Binary& cBinary) {
    size_t length1 = this->getString().length();
    size_t length2 = cBinary.getString().length();
    size_t maxLength = (length1>length2) ? length1 : length2;
    this->integer_value_ += cBinary.getInt();
    string_value_ = computeString(this->integer_value_);
    this->addLeftPadding(maxLength - string_value_.length());
  }

  Binary Binary::operator+(const Binary& b){
    size_t length1 = this->getString().length();
    size_t length2 = b.getString().length();
    size_t maxLength = (length1>length2) ? length1 : length2;
    return Binary(this->integer_value_ + b.getInt(), maxLength);
  }

  size_t Binary::computeInteger(const std::string& str_value) const {
    int length = str_value.length();
    size_t total = 0;
    size_t power = 1;
    for (int i = length -1; i >= 0; i--) {
      if (str_value[i] == '0'){
        //Nothing to do, just checking if a valid char in string
      }
      else if (str_value[i] == '1') {
        total += power;
      } else {
        throw "INVALID CHARACTER PASSED TO BINARY NUMBER";
      }
      power *= 2;
    }
    return total;
  }
  std::string Binary::computeString(size_t int_value) const {
    size_t tmp = int_value;
    std::string r_result = "", result = "";
    while (tmp > 0) {
      if (tmp % 2 == 1) {
        r_result += '1';
      } else {
        r_result += '0';
      }
      tmp /= 2;
    }
    for (int i=r_result.size()-1; i>=0; result += r_result[i], i--);
    return result;
  }

  // ===========================================================================
  // Galois Field Implementation
  GaloisField::GaloisField(size_t size, size_t modulus) {
    size_ = size;
    modulus_ = modulus;
    log_table_ = std::vector<GF_int*>(size_, NULL);
    size_t val = 1;
    for (size_t n=0; n<size_; n++) {
      GF_int* gf_int = new GF_int(n, val, this);
      log_table_[n] = gf_int;
      antilog_table_[val] = gf_int;
      val *= 2;
      if (val >= size_) {
        val = val ^ modulus_;
      }
    }
    antilog_table_[0] = new GF_int(0, 0, this);
    antilog_table_[1] = log_table_[0];
  }

  GaloisField::~GaloisField() {
    for (size_t n=0; n<size_; n++) {
      delete log_table_[n];
    }
  }

  GF_int* GaloisField::log(const size_t n) const {
    return log_table_[n];
  }

  GF_int* GaloisField::antilog(const size_t n) const {
    return antilog_table_.at(n);
  }

  GF_int::GF_int(size_t n, size_t val, GaloisField* field) {
    n_ = n;
    value_ = val;
    field_ = field;
  }

  GF_int::GF_int() {
    n_ = -1;
    value_ = -1;
    field_ = NULL;
  }

  size_t GF_int::getN() {
    return n_;
  }

  size_t GF_int::getValue() {
    return value_;
  }

  GF_int operator*(const GF_int& a, const GF_int& b) {
    size_t new_n = (a.n_ + b.n_) % (a.field_->size_ - 1);
    return *a.field_->log_table_[new_n];
  }

  GF_int operator^(const GF_int& a, const GF_int& b) {
    return *a.field_->antilog_table_[ a.value_ ^ b.value_ ];
  }

  GF_int operator+(const GF_int& a, const GF_int& b) {
    return a ^ b;
  }

  // ===========================================================================
  // Image Conversion Tools
  bool fileExists(const std::string& filename) {
    std::ifstream ifile(filename.c_str());
    return ifile;
  }

  bool containsValidCharacters(const std::string& target) {
    std::string validChars = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_./";
    bool flag = false;
    for(int i = 0; i < target.length(); i++) {
      for(int j = 0; j < validChars.length() && !flag; j++){
        if(target[i] == validChars[j]) {
          flag = true;
        }
      }

      if(!flag) {
        return false;
      }
      flag = false;

    }
    return true; 
  }  
  
  bool imageConvert(const std::string& filename, const std::string& destination) {
    if(!fileExists(filename)) {
      throw "Unable to Find File: " + filename;
    }
    std::string command = "convert ";
    if(!containsValidCharacters(filename) ) {
      throw "Invalid Character in: " + filename;
    }
    command += filename;
    command += " ";
    if(!containsValidCharacters(destination) ) {
      throw "Invalid Character in: " + destination;
    }
    command += destination;
    std::system(command.c_str() );
    return fileExists(destination);
  } 

}
