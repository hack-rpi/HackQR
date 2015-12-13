#include "qr.h"

namespace QRUnitTests 
{

  size_t runAll() {
    std::cout << "Running all QR Unit Tests..." << std::endl;
    runAlphanumeric();
    return 0;
  }
  
  size_t runAlphanumeric() {
    std::string m = "HELLO WORLD";
    Alphanumeric a(m, 1);
    std::cout << a.getLength() << std::endl;
    std::cout << a.getMessage() << std::endl;
  }
  
}
