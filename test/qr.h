#ifndef _QR_UNIT_TESTS_
#define _QR_UNIT_TESTS_

#include <iostream>
#include <vector>

#include "./../source/encoding.h"
#include "./../source/qr.h"
#include "./../source/util.h"

namespace QRUnitTests 
{
  
  extern size_t runAll();
  extern size_t runAlphanumeric();
  extern size_t runGenerator();
  extern size_t runEncoder();
  
}

#endif
