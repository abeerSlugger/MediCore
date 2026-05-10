#include "Patient.h"

Patient::Patient() : Person(), age(0), balance(0.0f) {
    gender[0] = '\0'; contact[0] = '\0';
}
Patient::Patient(int id, const char* nm, int a, const char* g,
    const char* c, const char* pw, float bal)
    : Person(id, nm, pw), age(a), balance(bal) {
    strCopy(gender, g, 5); strCopy(contact, c, 15);
}
Patient& Patient::operator+=(float amt) { balance += amt; return *this; }
Patient& Patient::operator-=(float amt) { balance -= amt; return *this; }
bool Patient::operator==(const Patient& o) const { return id == o.id; }
std::ostream& operator<<(std::ostream& os, const Patient& p) {
    char b[20]; floatToStr(p.balance, b);
    os << "ID:" << p.id << " Name:" << p.name << " Age:" << p.age
        << " Gender:" << p.gender << " Contact:" << p.contact << " Balance:PKR " << b;
    return os;
}