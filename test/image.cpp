#include "image.h"

namespace ImageUnitTests 
{
  
  size_t runAll() {
    std::cout << "Running all Image Unit Tests..." << std::endl;
    runAugmentation(); 
    return 0;
  }

  size_t runAugmentation() {
    Image<bool> testImage;
    testImage.Load("test/assets/qr1.pbm");
    testImage.expandImage(4);
    testImage.Save("qr1EXPAND.pbm");
    std::cout << "Successfully Expanded Image" << std::endl;

    testImage.addPadding(10);
    testImage.Save("qr1PADDED.pbm");
    std::cout << "Successfully Added Padding to Image" << std::endl;
    return 0;
  }
  
}
