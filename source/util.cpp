#include "util.h"
#include <iostream>

namespace util
{
  // =============================================================================
  // Binary Class Implementation
  Binary::Binary(const std::string& str_value) {
    string_value_ = str_value;
    integer_value_ = computeInteger(str_value);
  }

  Binary::Binary(size_t int_val) {
    string_value_ = computeString(int_val);
    integer_value_ = int_val;
  }

  std::string Binary::getString() {
    return string_value_;
  }

  size_t Binary::getInt() {
    return integer_value_;
  }

  size_t Binary::computeInteger(const std::string& str_value) const {
    int length = str_value.length();
    size_t total = 0;
    size_t power = 1;
    for(int i = length -1; i >= 0; i--) {
      if(str_value[i] == '0'){
        //Nothing to do, just checking if a valid char in string
      }
      else if(str_value[i] == '1') {
        total += power;
      }
      else {
        throw "INVALID CHARACTER PASSED TO BINARY NUMBER";
      }
      power *= 2;
    }
    return total;
  }
  std::string Binary::computeString(size_t int_value) const {
    size_t startingBase = 1;
    std::string string_representation = "";
    //Covering base case
    if(int_value == 0) {
      string_representation += '0';
    }
    else {
      //Finding the base to start the binary number at
      while(startingBase * 2 < int_value) {
        startingBase *= 2;
      }
      while(startingBase > 0){
        if(startingBase <= int_value) {
          string_representation += '1';
          int_value -= startingBase;
        }
        else {
          string_representation += '0';
        }
        startingBase /= 2;
      }
    }
    return string_representation;
  }
}
