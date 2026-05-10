#include "Validator.h"

static const char* SLOTS[] = { "09:00","10:00","11:00","12:00","13:00","14:00","15:00","16:00" };
static const int   NSLOTS = 8;

bool Validator::isValidDate(const char* date, int currentYear) {
    if (strLen(date) != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;
    char d[3] = { date[0],date[1],'\0' };
    char m[3] = { date[3],date[4],'\0' };
    char y[5] = { date[6],date[7],date[8],date[9],'\0' };
    if (!isAllDigits(d) || !isAllDigits(m) || !isAllDigits(y)) return false;
    int day = strToInt(d), mon = strToInt(m), yr = strToInt(y);
    if (day < 1 || day>31) return false;
    if (mon < 1 || mon>12) return false;
    if (yr < currentYear) return false;
    return true;
}

bool Validator::isValidTimeSlot(const char* slot) {
    for (int i = 0; i < NSLOTS; i++) if (strCmp(slot, SLOTS[i]) == 0) return true;
    return false;
}

bool Validator::isValidContact(const char* c) {
    return strLen(c) == 11 && isAllDigits(c);
}

bool Validator::isValidPassword(const char* pw) {
    return strLen(pw) >= 6;
}

bool Validator::isPositiveFloat(const char* s) {
    if (!s[0]) return false;
    bool dot = false;
    for (int i = 0; s[i]; i++) {
        if (s[i] == '.') { if (dot) return false; dot = true; }
        else if (s[i] < '0' || s[i]>'9') return false;
    }
    return strToFloat(s) > 0.0f;
}

bool Validator::isPositiveInt(const char* s) {
    return isAllDigits(s) && strToInt(s) > 0;
}

bool Validator::isValidMenuChoice(int c, int mn, int mx) {
    return c >= mn && c <= mx;
}

bool Validator::isValidId(const char* s) {
    return isAllDigits(s) && strToInt(s) > 0;
}