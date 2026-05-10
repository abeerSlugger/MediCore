#ifndef PERSON_H
#define PERSON_H
#include "Utils.h"

class Person {
protected:
    int  id;
    char name[100];
    char password[50];
public:
    Person() : id(0) { name[0] = '\0'; password[0] = '\0'; }
    Person(int id, const char* nm, const char* pw) : id(id) {
        strCopy(name, nm, 100); strCopy(password, pw, 50);
    }
    virtual void        displayMenu() = 0;
    virtual const char* getRole() const = 0;
    int         getId()       const { return id; }
    const char* getName()     const { return name; }
    const char* getPassword() const { return password; }
    void setId(int i) { id = i; }
    void setName(const char* n) { strCopy(name, n, 100); }
    void setPassword(const char* p) { strCopy(password, p, 50); }
    bool checkPassword(const char* pw) const { return strCmp(password, pw) == 0; }
    virtual ~Person() {}
};

#endif