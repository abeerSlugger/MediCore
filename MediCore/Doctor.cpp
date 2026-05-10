#include "Doctor.h"

Doctor::Doctor() : Person(), fee(0.0f) {
    specialization[0] = '\0'; contact[0] = '\0';
}
Doctor::Doctor(int id, const char* nm, const char* spec,
    const char* c, const char* pw, float f)
    : Person(id, nm, pw), fee(f) {
    strCopy(specialization, spec, 60); strCopy(contact, c, 15);
}
bool Doctor::operator==(const Doctor& o) const { return id == o.id; }
std::ostream& operator<<(std::ostream& os, const Doctor& d) {
    char f[20]; floatToStr(d.fee, f);
    os << "ID:" << d.id << " Name:" << d.name << " Spec:" << d.specialization
        << " Contact:" << d.contact << " Fee:PKR " << f;
    return os;
}