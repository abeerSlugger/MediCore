#ifndef DOCTOR_H
#define DOCTOR_H
#include <iostream>
#include "Person.h"

class Doctor : public Person {
    char  specialization[60];
    char  contact[15];
    float fee;
public:
    Doctor();
    Doctor(int id, const char* nm, const char* spec,
        const char* contact, const char* pw, float fee);

    const char* getSpecialization() const { return specialization; }
    const char* getContact()        const { return contact; }
    float       getFee()            const { return fee; }
    void setFee(float f) { fee = f; }
    void setSpecialization(const char* s) { strCopy(specialization, s, 60); }
    void setContact(const char* c) { strCopy(contact, c, 15); }

    bool operator==(const Doctor& o) const;
    friend std::ostream& operator<<(std::ostream& os, const Doctor& d);

    void        displayMenu() override {}
    const char* getRole() const override { return "Doctor"; }
};
#endif
