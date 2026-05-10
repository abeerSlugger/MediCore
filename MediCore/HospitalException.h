#ifndef HOSPITALEXCEPTION_H
#define HOSPITALEXCEPTION_H
#include "Utils.h"

class HospitalException {
protected:
    char message[200];
public:
    HospitalException(const char* msg) { strCopy(message, msg, 200); }
    virtual const char* what() const { return message; }
    virtual ~HospitalException() {}
};

class FileNotFoundException : public HospitalException {
public:
    FileNotFoundException(const char* f) : HospitalException("") {
        strCopy(message, "File not found: ", 200); strConcat(message, f, 200);
    }
};

class InsufficientFundsException : public HospitalException {
public:
    InsufficientFundsException(float req, float avail) : HospitalException("") {
        char r[20], a[20]; floatToStr(req, r); floatToStr(avail, a);
        strCopy(message, "Insufficient funds. Required: PKR ", 200);
        strConcat(message, r, 200); strConcat(message, ", Available: PKR ", 200); strConcat(message, a, 200);
    }
};

class InvalidInputException : public HospitalException {
public:
    InvalidInputException(const char* d) : HospitalException("") {
        strCopy(message, "Invalid input: ", 200); strConcat(message, d, 200);
    }
};

class SlotUnavailableException : public HospitalException {
public:
    SlotUnavailableException(const char* slot) : HospitalException("") {
        strCopy(message, "Slot ", 200); strConcat(message, slot, 200);
        strConcat(message, " is already taken.", 200);
    }
};

#endif
