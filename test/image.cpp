#include "image.h"

namespace ImageUnitTests 
{
  
  size_t runAll() {
    std::cout << "Running all Image Unit Tests..." << std::endl;
    runExpansion(); 
    return 0;
  }

  size_t runExpansion() {
    Image<bool> testImage;
    testImage.Load("test/assets/qr1.pbm");
    testImage.expandImage(4);
    testImage.Save("qr1EXPAND.pbm");
    return 0;
  }
  
}
