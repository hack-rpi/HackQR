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

}
