#include "util.h"

namespace UtilUnitTest
{

  size_t runAll() {
    std::cout << "Running all Util Unit Tests..." << std::endl;
    // runBinary();
    runGaloisField();
    return 0;
  }

  size_t runBinary() {
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
  }
  
  size_t runGaloisField() {
    std::cout << "=> Galois Field Tests:" << std::endl; 
    util::GaloisField gf(256, 285);
    
    util::GF_int* n0 = gf[0];
    std::cout << "n = 0" << std::endl;
    std::cout << '\t' << n0->getN() << " == " << 0 << std::endl;
    std::cout << '\t' << n0->getValue() << " == " << 1 << std::endl;
    
    util::GF_int* n8 = gf[8];
    std::cout << "n = 8" << std::endl;
    std::cout << '\t' << n8->getN() << " == " << 8 << std::endl;
    std::cout << '\t' << n8->getValue() << " == " << 29 << std::endl;
    
    util::GF_int* n40 = gf[40];
    std::cout << "n = 40" << std::endl;
    std::cout << '\t' << n40->getN() << " == " << 40 << std::endl;
    std::cout << '\t' << n40->getValue() << " == " << 106 << std::endl;
    
    util::GF_int* n255 = gf[255];
    std::cout << "n = 255" << std::endl;
    std::cout << '\t' << n255->getN() << " == " << 255 << std::endl;
    std::cout << '\t' << n255->getValue() << " == " << 1 << std::endl;
  }

}
