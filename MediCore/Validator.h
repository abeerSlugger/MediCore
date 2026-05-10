#ifndef VALIDATOR_H
#define VALIDATOR_H
#include "Utils.h"

class Validator {
public:
    static bool isValidDate(const char* date, int currentYear);
    static bool isValidTimeSlot(const char* slot);
    static bool isValidContact(const char* contact);
    static bool isValidPassword(const char* pw);
    static bool isPositiveFloat(const char* s);
    static bool isPositiveInt(const char* s);
    static bool isValidMenuChoice(int choice, int minVal, int maxVal);
    static bool isValidId(const char* s);
};
#endif
