#include <iostream>

#include "image.h"
#include "qr.h"

void usage(char* fn) {
  std::cerr << fn << " [test name]" << std::endl;
  std::cerr << "\t Tests: image qr" << std::endl;
}

int main(int argc, char** argv) {
  
  if (argc < 2) {
    usage(argv[0]);
    return 1; 
  }
    
  char* test_name = argv[1];
  
  if (test_name == std::string("image")) {
    ImageUnitTests::runAll();
  } else if (test_name == std::string("qr")) {
    QRUnitTests::runAll();
  } else {
    std::cerr << "Invalid test name." << std::endl;
    usage(argv[0]);
    return 1;
  }
  
  return 0;
}
