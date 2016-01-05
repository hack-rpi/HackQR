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
    std::string m = "HELLO WORLD";
    
    QR qr1('M', 1);
    Alphanumeric a(m, 1);
    qr1.Encode(a);
    qr1.Save("qr1.pbm");
    
    // QR qr2('Q', 1);
    // Alphanumeric b(m, 1);
    // qr2.Encode(b);
    // qr2.Save("qr2.pbm");
    
    // QR qr3('Q', 2);
    // Alphanumeric c(m, 2);
    // qr3.Encode(c);
    // qr3.Save("qr3.pbm");
    
    // QR qr4('Q', 8);
    // Alphanumeric d(m, 8);
    // qr4.Encode(d);
    // qr4.Save("qr4.pbm");
    
    return 0;
  }
  
}
