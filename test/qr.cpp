#include "qr.h"

namespace QRUnitTests 
{

  size_t runAll() {
    std::cout << "Running all QR Unit Tests..." << std::endl;
    runAlphanumeric();
    runGenerator();
    runEncoder();
    return 0;
  }
  
  size_t runAlphanumeric() {
    std::string m = "HELLO WORLD";
    Alphanumeric a(m, 1);
    std::cout << a.getLength() << std::endl;
    std::cout << a.getMessage() << std::endl;
    
    std::vector<util::Binary> encoding = a.getEncodedMessage();
    for (size_t b=0; b<encoding.size(); b++) {
      std::cout << encoding[b].getString() << ' ';
    }
    std::cout << std::endl;
    
    return 0;
  }
  
  size_t runGenerator() {
    return 0; 
  }
  
  size_t runEncoder() {
    QR qr1('M', 1);
    std::string m = "HELLO WORLD";
    Alphanumeric a(m, 1);
    qr1.Encode(a);
    
    QR qr2('Q', 10);
    Alphanumeric b(m, 10);
    qr2.Encode(b);
    
    return 0;
  }
  
}
