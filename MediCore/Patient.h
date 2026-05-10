#ifndef PATIENT_H
#define PATIENT_H
#include <iostream>
#include "Person.h"

class Patient : public Person {
    int   age;
    char  gender[5];
    char  contact[15];
    float balance;
public:
    Patient();
    Patient(int id, const char* nm, int age, const char* gender,
        const char* contact, const char* pw, float balance);

    int         getAge()     const { return age; }
    const char* getGender()  const { return gender; }
    const char* getContact() const { return contact; }
    float       getBalance() const { return balance; }
    void setBalance(float b) { balance = b; }
    void setAge(int a) { age = a; }
    void setGender(const char* g) { strCopy(gender, g, 5); }
    void setContact(const char* c) { strCopy(contact, c, 15); }

    Patient& operator+=(float amt);
    Patient& operator-=(float amt);
    bool     operator==(const Patient& o) const;
    friend std::ostream& operator<<(std::ostream& os, const Patient& p);

    void        displayMenu() override {}
    const char* getRole() const override { return "Patient"; }
};
#endif