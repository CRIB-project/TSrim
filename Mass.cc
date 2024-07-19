#include "Mass.h"
#include <cstring>
#include <iostream>

using namespace amdc;

double amdc::Mass(int Z, int A) {
    for (decltype(masstab.size()) i = 0; i < masstab.size(); i++) {
        if (masstab.at(i).vZ == Z && masstab.at(i).vA == A)
            return masstab.at(i).vM;
    }
    std::cout << "No such an isotope!" << std::endl;
    return -1000.;
}

double amdc::Mass(int A, const char *El) {
    for (decltype(masstab.size()) i = 0; i < masstab.size(); i++) {
        if (masstab.at(i).vA == A && strcmp(masstab.at(i).vEl, El) == 0)
            return masstab.at(i).vM;
    }
    std::cout << "No such an isotope!" << std::endl;
    return -1000.;
}

double amdc::MassExcess(int Z, int A) {
    return (Mass(Z, A) - A) * amu;
}

double amdc::MassExcess(int A, const char *El) {
    return (Mass(A, El) - A) * amu;
}

double amdc::EBindPu(int Z, int A) {
    return (Z * Mass(1, 1) + (A - Z) * Mass(0, 1) - Mass(Z, A)) * amu / A;
}

double amdc::EBindPu(int A, const char *El) {
    return (double(GetZ(El)) * Mass(1, 1) + (A - double(GetZ(El))) * Mass(0, 1) - Mass(A, El)) * amu / A;
}

const char *amdc::GetEl(int Z) {
    for (decltype(masstab.size()) i = 0; i < masstab.size(); i++) {
        if (masstab.at(i).vZ == Z)
            return masstab.at(i).vEl;
    }
    std::cout << "No such an element! " << std::endl;
    return "X";
}

int amdc::GetZ(const char *El) {
    for (decltype(masstab.size()) i = 0; i < masstab.size(); i++) {
        if (strcmp(masstab.at(i).vEl, El) == 0)
            return masstab.at(i).vZ;
    }
    std::cout << "No such an element! " << std::endl;
    return -1000;
}
