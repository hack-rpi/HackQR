#include "qr.h"

namespace QRUnitTests 
{

  size_t runAll() {
    std::cout << "Running all QR Unit Tests..." << std::endl;
    runAlphanumeric();
    runGenerator();
    return 0;
  }
  
  size_t runAlphanumeric() {
    std::string m = "HELLO WORLD";
    Alphanumeric a(m, 1);
    std::cout << a.getLength() << std::endl;
    std::cout << a.getMessage() << std::endl;
  }
  
  size_t runGenerator() {
    QR<Alphanumeric> qr('Q', 1);
    
    std::vector<util::GF_int> g1 = qr.getGenerator(1);
    std::cout << "(";
    for (size_t i=0; i<g1.size(); i++) 
      std::cout << "a^" << g1[i].getN() << "x^" << g1.size()-i-1 
        << (i < g1.size()-1 ? " + " : ")");
    std::cout << std::endl;
    
    std::vector<util::GF_int> g2 = qr.getGenerator(2);
    std::cout << "(";
    for (size_t i=0; i<g2.size(); i++) 
      std::cout << "a^" << g2[i].getN() << "x^" << g2.size()-i-1 
        << (i < g2.size()-1 ? " + " : ")");
    std::cout << std::endl;
    
    std::vector<util::GF_int> g3 = qr.getGenerator(3);
    std::cout << "(";
    for (size_t i=0; i<g3.size(); i++) 
      std::cout << "a^" << g3[i].getN() << "x^" << g3.size()-i-1 
        << (i < g3.size()-1 ? " + " : ")");
    std::cout << std::endl;
    
    std::vector<util::GF_int> g7 = qr.getGenerator(7);
    std::cout << "(";
    for (size_t i=0; i<g7.size(); i++) 
      std::cout << "a^" << g7[i].getN() << "x^" << g7.size()-i-1 
        << (i < g7.size()-1 ? " + " : ")");
    std::cout << std::endl;
    
  }
  
}
