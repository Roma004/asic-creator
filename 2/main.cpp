#include "tristate.hpp"
#include "tri-register.hpp"
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>

int main() {
    using st = Tristate::STATE;
    TriRegister reg1("01x0110x01");
    TriRegister reg2{0, 1, 0, 1, 0, 3, 1, 8, 0};
    TriRegister reg3{'0', 't', 'f', '1', 'z'};

    std::vector<int> asd{0, 1, 0, 1, 0, 3, 1, 8, 0};
    TriRegister reg4(asd.begin(), asd.end());
    int sdf[] = {1, 2, 0, 1, 2};
    TriRegister reg5(sdf, sdf + 5);
    TriRegister reg6(6);
    TriRegister reg7 = reg1 | reg5;

    std::stringstream ss;
    ss << reg1;

    std::cout << reg1 << std::endl;
    std::cout << reg2 << std::endl;
    std::cout << reg3 << std::endl;
    std::cout << reg4 << std::endl;
    std::cout << reg5 << std::endl;
    std::cout << reg6 << std::endl;
    std::cout << ss.str() << std::endl;
    std::cout << "------------------------" << std::endl;
    std::cout << reg7 << std::endl;
    std::cout << (reg1 | ~reg1) << std::endl;
}
