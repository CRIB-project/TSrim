#include "Mass.h"

double Mass(int Z, int A) {
    for (int i = 0; i < masstab.size(); i++) {
        if (masstab.at(i).vZ == Z && masstab.at(i).vA == A)
            return masstab.at(i).vM;
    }
    cout << "No such an isotope!" << endl;
    return -1000.;
}

double Mass(int A, const char *El) {
    for (int i = 0; i < masstab.size(); i++) {
        if (masstab.at(i).vA == A && strcmp(masstab.at(i).vEl, El) == 0)
            return masstab.at(i).vM;
    }
    cout << "No such an isotope!" << endl;
    return -1000.;
}

double MassExcess(int Z, int A) {
    return (Mass(Z, A) - A) * amu;
}

double MassExcess(int A, const char *El) {
    return (Mass(A, El) - A) * amu;
}

double EBindPu(int Z, int A) {
    return (Z * Mass(1, 1) + (A - Z) * Mass(0, 1) - Mass(Z, A)) * amu / A;
}

double EBindPu(int A, const char *El) {
    return (double(GetZ(El)) * Mass(1, 1) + (A - double(GetZ(El))) * Mass(0, 1) - Mass(A, El)) * amu / A;
}

const char *GetEl(int Z) {
    for (int i = 0; i < masstab.size(); i++) {
        if (masstab.at(i).vZ == Z)
            return masstab.at(i).vEl;
    }
    cout << "No such an element! " << endl;
    return "X";
}

int GetZ(const char *El) {
    for (int i = 0; i < masstab.size(); i++) {
        if (strcmp(masstab.at(i).vEl, El) == 0)
            return masstab.at(i).vZ;
    }
    cout << "No such an element! " << endl;
    return -1000;
}
