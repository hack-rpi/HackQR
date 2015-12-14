#include "util.h"

namespace UtilUnitTest
{

  size_t runAll() {
    std::cout << "Running all Util Unit Tests..." << std::endl;
    runBinary();
    return 0;
  }

  size_t runBinary() {
    std::string m = "001100";
    size_t m2 = 12;
    util::Binary b(m2);
    util::Binary a(m);
    std::cout << a.getString() << std::endl;
    std::cout << a.getInt() << std::endl;
    std::cout << b.getString() << std::endl;
    std::cout << b.getInt() << std::endl;
  }

}
