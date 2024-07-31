#include "Mass.h"
#include <algorithm>
#include <iostream>

using namespace amdc;

double amdc::Mass(int Z, int A) {
    if (Z < 0 || Z > maxZ || A < 0 || A > maxA) {
        std::cout << "No such an isotope!" << std::endl;
        return -1000.;
    }

    double mass = masstab[Z][A];

    if (mass > 0)
        return mass;
    else {
        std::cout << "No such an isotope!" << std::endl;
        return -1000.;
    }
}

double amdc::Mass(int A, std::string El) {
    if (A < 0 || A > maxA) {
        std::cout << "No such an isotope!" << std::endl;
        return -1000.;
    }

    if (auto it = map_ElToZ.find(El); it != map_ElToZ.end())
        return Mass(it->second, A);
    else {
        std::cout << "No such an isotope!" << std::endl;
        return -1000.;
    }
}

double amdc::MassExcess(int Z, int A) {
    return (Mass(Z, A) - A) * amu;
}

double amdc::MassExcess(int A, std::string El) {
    return (Mass(A, El) - A) * amu;
}

double amdc::EBindPu(int Z, int A) {
    return (Z * Mass(1, 1) + (A - Z) * Mass(0, 1) - Mass(Z, A)) * amu / A;
}

double amdc::EBindPu(int A, std::string El) {
    return (double(GetZ(El)) * Mass(1, 1) + (A - double(GetZ(El))) * Mass(0, 1) - Mass(A, El)) * amu / A;
}

std::string amdc::GetEl(int Z) {
    if (Z < 0 || Z > maxZ) {
        std::cout << "No such an isotope!" << std::endl;
        return "X";
    }

    auto it = std::find_if(map_ElToZ.begin(), map_ElToZ.end(),
                           [&Z](const std::pair<std::string, int> &p) {
                               return p.second == Z;
                           });

    if (it != map_ElToZ.end())
        return it->first;
    else {
        std::cout << "No such an element! " << std::endl;
        return "X";
    }
}

int amdc::GetZ(std::string El) {
    if (auto it = map_ElToZ.find(El); it != map_ElToZ.end())
        return it->second;
    else {
        std::cout << "No such an element! " << std::endl;
        return -1000;
    }
}
