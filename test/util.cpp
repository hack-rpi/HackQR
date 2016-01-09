#include "util.h"

namespace UtilUnitTest
{

  size_t runAll() {
    std::cout << "Running all Util Unit Tests..." << std::endl;
    runBinary();
    runGaloisField();
    runHelpers();
    runConversion();
    return 0;
  }

  size_t runBinary() {
    
    util::Binary a1(1);
    std::cout << a1.getString() << " == " << "1" << std::endl;
    
    util::Binary a2(2);
    std::cout << a2.getString() << " == " << "10" << std::endl;
    
    util::Binary a3(3);
    std::cout << a3.getString() << " == " << "11" << std::endl;
    
    util::Binary a4(4);
    std::cout << a4.getString() << " == " << "100" << std::endl;
    
    util::Binary a12(12);
    std::cout << a12.getString() << " == " << "1100" << std::endl;
    
    std::string m = "001100";
    size_t m2 = 12;
    util::Binary a(m);
    util::Binary b(m2);
    std::cout << "EXPECTED: 001100" << std::endl;
    std::cout << a.getString() << std::endl;
    std::cout << "EXPECTED: 12" << std::endl;
    std::cout << a.getInt() << std::endl;
    std::cout << "EXPECTED: 1100" << std::endl;
    std::cout << b.getString() << std::endl;
    std::cout << "EXPECTED: 12" << std::endl;
    std::cout << b.getInt() << std::endl;
    std::cout << "EXPECTED: 00001100" << std::endl;
    b.addLeftPadding(4);
    std::cout << b.getString() << std::endl;
    a += b;
    std::cout << "EXPECTED: 11000" << std::endl;
    std::cout << a.getString() << std::endl;
    std::cout << "EXPECTED: 24" << std::endl;
    std::cout << a.getInt() << std::endl;
    util::Binary c;
    c = a + b;
    std::cout << "EXPECTED: 11000" << std::endl;
    std::cout << a.getString() << std::endl;
    std::cout << "EXPECTED: 24" << std::endl;
    std::cout << a.getInt() << std::endl;

    size_t n = 13;
    std::string n2 = "01101";
    util::Binary f(n,8);
    util::Binary g(n2,8);
    std::cout << "EXPECTED: 00001101" << std::endl;
    std::cout << f.getString() << std::endl;
    std::cout << "EXPECTED: 13" << std::endl;
    std::cout << f.getInt() << std::endl;
    std::cout << "EXPECTED: 00001101" << std::endl;
    std::cout << f.getString() << std::endl;
    std::cout << "EXPECTED: 13" << std::endl;
    std::cout << g.getInt() << std::endl;

    std::cout << "EXPECTED: 00100101" << std::endl;
    std::cout << (a+g).getString() << std::endl;
    std::cout << "EXPECTED: 37" << std::endl;
    std::cout << (a+g).getInt() << std::endl;

    return 0;
  }

  size_t runGaloisField() {
    std::cout << "=> Galois Field Tests:" << std::endl;
    util::GaloisField gf(256, 285);

    util::GF_int* n0 = gf.log(0);
    std::cout << "n = 0" << std::endl;
    std::cout << '\t' << n0->getN() << " == " << 0 << std::endl;
    std::cout << '\t' << n0->getValue() << " == " << 1 << std::endl;

    util::GF_int* n1 = gf.antilog(1);
    std::cout << "n = 1" << std::endl;
    std::cout << '\t' << n1->getN() << " == " << 0 << std::endl;
    std::cout << '\t' << n1->getValue() << " == " << 1 << std::endl;

    util::GF_int* n8 = gf.log(8);
    std::cout << "n = 8" << std::endl;
    std::cout << '\t' << n8->getN() << " == " << 8 << std::endl;
    std::cout << '\t' << n8->getValue() << " == " << 29 << std::endl;

    n8 = gf.antilog(8);
    std::cout << "n = 8" << std::endl;
    std::cout << '\t' << n8->getN() << " == " << 3 << std::endl;
    std::cout << '\t' << n8->getValue() << " == " << 8 << std::endl;

    util::GF_int* n40 = gf.log(40);
    std::cout << "n = 40" << std::endl;
    std::cout << '\t' << n40->getN() << " == " << 40 << std::endl;
    std::cout << '\t' << n40->getValue() << " == " << 106 << std::endl;

    n40 = gf.antilog(40);
    std::cout << "n = 40" << std::endl;
    std::cout << '\t' << n40->getN() << " == " << 53 << std::endl;
    std::cout << '\t' << n40->getValue() << " == " << 40 << std::endl;

    util::GF_int* n255 = gf.log(255);
    std::cout << "n = 255" << std::endl;
    std::cout << '\t' << n255->getN() << " == " << 255 << std::endl;
    std::cout << '\t' << n255->getValue() << " == " << 1 << std::endl;

    n255 = gf.antilog(255);
    std::cout << "n = 255" << std::endl;
    std::cout << '\t' << n255->getN() << " == " << 175 << std::endl;
    std::cout << '\t' << n255->getValue() << " == " << 255 << std::endl;

    util::GF_int* n4 = gf.log(4);
    util::GF_int* n6 = gf.log(6);
    util::GF_int n10_ = *n4 * *n6;
    std::cout << "n4 * n6" << std::endl;
    std::cout << '\t' << n10_.getN() << " == " << 10 << std::endl;
    std::cout << '\t' << n10_.getValue() << " == " << 116 << std::endl;

    n1 = gf.log(1);
    util::GF_int n25_ = *n0 + *n1;
    std::cout << "n0 + n1" << std::endl;
    std::cout << '\t' << n25_.getN() << " == " << 25 << std::endl;
    std::cout << '\t' << n25_.getValue() << " == " << 3 << std::endl;
    
    util::GF_int n233_ = *gf.antilog(236) ^ *gf.antilog(5);
    std::cout << "236 ^ 5" << std::endl;
    std::cout << '\t' << n233_.getN() << " == " << 233 << std::endl;
    std::cout << '\t' << n233_.getValue() << " == " << '?' << std::endl;

    std::cout << (236 ^ 5) << std::endl;

    return 0;
  }

  size_t runHelpers() {
    std::cout << "=> Helper Function Tests:" << std::endl;
    int coeffs_a[] = {1, 2};
    size_t an = 2;
    std::vector<int> a1(coeffs_a, coeffs_a + an);
    int coeffs_b[] = {1, 3};
    size_t bn = 2;
    std::vector<int> b1(coeffs_b, coeffs_b + bn);
    std::cout << "(";
    for (size_t i=0; i<an; i++) std::cout << a1[i] << "x^" << an-i-1
      << (i < an-1 ? " + " : "");
    std::cout << ") * (";
    for (size_t i=0; i<bn; i++) std::cout << b1[i] << "x^" << an-i-1
      << (i < an-1 ? " + " : ")");
    std::cout << " = (";
    std::vector<int> result1 = util::multiplyPoly(a1,b1);
    for (size_t i=0; i<an+bn-1; i++) std::cout << result1[i] << "x^" << bn+an-i-2
      << (i < bn+an-2 ? " + " : ")");
    std::cout << std::endl;

    util::GaloisField gf(256, 285);
    util::GF_int Ca[] = {*gf.log(0), *gf.log(0)};
    an = 2;
    std::vector<util::GF_int> a2(Ca, Ca + an);
    std::cout << "(";
    for (size_t i=0; i<an; i++) std::cout << "a^" << a2[i].getN() << "x^"
      << an-i-1 << (i < an-1 ? " + " : ") * (");
    util::GF_int Cb[] = {*gf.log(0), *gf.log(1)};
    bn = 2;
    std::vector<util::GF_int> b2(Cb, Cb + bn);
    for (size_t i=0; i<bn; i++) std::cout << "a^" << b2[i].getN() << "x^"
      << bn-i-1 << (i < bn-1 ? " + " : ") = (");
    std::vector<util::GF_int> result2 = util::multiplyPoly(a2,b2);
    for (size_t i=0; i<an+bn-1; i++) std::cout << "a^" << result2[i].getN()
      << "x^" << bn+an-i-2 << (i < bn+an-2 ? " + " : ")");
    std::cout << std::endl;

    util::GF_int Cc[] = {*gf.log(0), *gf.log(2)};
    size_t cn = 2, rn = result2.size();
    std::vector<util::GF_int> c2(Cc, Cc + cn);
    std::cout << "(";
    for (size_t i=0; i<cn; i++) std::cout << "a^" << c2[i].getN() << "x^"
      << cn-i-1 << (i < cn-1 ? " + " : ") * (");
    for (size_t i=0; i<an+bn-1; i++) std::cout << "a^" << result2[i].getN()
      << "x^" << bn+an-i-2 << (i < bn+an-2 ? " + " : ") = ");
    std::vector<util::GF_int> result3 = util::multiplyPoly(result2,c2);
    for (size_t i=0; i<rn+cn-1; i++) std::cout << "a^" << result3[i].getN()
      << "x^" << cn+rn-i-2 << (i < cn+rn-2 ? " + " : ")");
    std::cout << std::endl;

    return 0;
  }

  size_t runConversion() {
    std::cout << "=> Image Conversion Tests:" << std::endl;
     
    util::imageConvert("test/assets/qr1.pbm", "qr1.jpg");
    if( util::fileExists("qr1.jpg") ) { 
      std::cout << "Successfully Made qr1.jpg" << std::endl;
      std::remove("qr1.jpg");
    } else { 
      std::cout << "Failed to Convert Image" << std::endl;
    }

    try {
      util::imageConvert("test/assets/qr1.pbm", "qr1.jpg && mkdir testBadInput");
    } catch(...) {
      std::cout << "Successfully Caught Bad Input" << std::endl;
    }
    return 0;
  }

}
