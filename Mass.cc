#include "Mass.h"
#include <cstring>
#include <iostream>

using namespace amdc;

double amdc::Mass(int Z, int A) {
    if (Z < 0 || Z > amdc::maxZ || A < 0 || A > amdc::maxA) {
        std::cout << "No such an isotope!" << std::endl;
        return -1000.;
    }

    double mass = masstab[Z][A].vM;

    if (mass < 0) {
        std::cout << "No such an isotope!" << std::endl;
        return -1000.;
    }

    return mass;
}

double amdc::Mass(int A, const char *El) {
    if (A < 0 || A > amdc::maxA) {
        std::cout << "No such an isotope!" << std::endl;
        return -1000.;
    }

    for (decltype(masstab.size()) z = 0; z < masstab.size(); z++) {
        if (strcmp(masstab[z][A].vEl, El) == 0)
            return masstab[z][A].vM;
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
    if (Z < 0 || Z > amdc::maxZ) {
        std::cout << "No such an isotope!" << std::endl;
        return "X";
    }

    for (decltype(masstab[Z].size()) a = 0; a < masstab[Z].size(); a++) {
        if (masstab[Z][a].vM > 0.0)
            return masstab[Z][a].vEl;
    }

    std::cout << "No such an element! " << std::endl;
    return "X";
}

int amdc::GetZ(const char *El) {
    for (decltype(masstab.size()) z = 0; z < masstab.size(); z++) {
        for (decltype(masstab[z].size()) a = 0; a < masstab[z].size(); a++) {
            if (masstab[z][a].vM > 0) {
                if (strcmp(masstab[z][a].vEl, El) == 0) {
                    return z;
                } else
                    break;
            }
        }
    }

    std::cout << "No such an element! " << std::endl;
    return -1000;
}
