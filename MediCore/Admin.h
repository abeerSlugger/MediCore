#ifndef ADMIN_H
#define ADMIN_H
#include "Person.h"

class Admin : public Person {
public:
    Admin();
    Admin(int id, const char* nm, const char* pw);
    void        displayMenu() override {}
    const char* getRole() const override { return "Admin"; }
};
#endif
