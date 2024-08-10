#include "Mass.h"
#include <iostream>

using namespace amdc;

double amdc::Mass(int Z, int A) {
    if (Z < 0 || Z > maxZ || A < 0 || A > maxA) {
        std::cerr << "No such an isotope!" << std::endl;
        return -1000.;
    }

    double mass = masstab[Z][A];

    if (mass > 0)
        return mass;
    else {
        std::cerr << "No such an isotope!" << std::endl;
        return -1000.;
    }
}

double amdc::Mass(int A, const std::string &El) {
    int Z = GetZ(El);
    if (Z < 0 || A < 0 || A > maxA) {
        std::cerr << "No such an isotope!" << std::endl;
        return -1000.;
    }

    double mass = masstab[Z][A];

    if (mass > 0)
        return mass;
    else {
        std::cerr << "No such an isotope!" << std::endl;
        return -1000.;
    }
}

double amdc::MassExcess(int Z, int A) {
    return (Mass(Z, A) - A) * amu;
}

double amdc::MassExcess(int A, const std::string &El) {
    return (Mass(GetZ(El), A) - A) * amu;
}

double amdc::EBindPu(int Z, int A) {
    return (Z * Mass(1, 1) + (A - Z) * Mass(0, 1) - Mass(Z, A)) * amu / A;
}

double amdc::EBindPu(int A, const std::string &El) {
    int Z = GetZ(El);
    return (Z * Mass(1, 1) + (A - Z) * Mass(0, 1) - Mass(Z, A)) * amu / A;
}

std::string amdc::GetEl(int Z) {
    if (Z < 0 || Z > maxZ) {
        std::cerr << "No such an isotope!" << std::endl;
        return "X";
    }

    return element_names[Z];
}

int amdc::GetZ(const std::string &El) {
    if (El.empty() || El.length() > 2) {
        std::cerr << "Invalid element symbol!" << std::endl;
        return -1000;
    }

    char first = El[0];
    char second = (El.length() > 1) ? El[1] : 0;
    int hash = isotope_hash(first, second);
    int Z = isotope_table[hash];

    if (Z < 0) {
        std::cerr << "No such element!" << std::endl;
        return -1000;
    }
    return Z;
}
