#include "Bill.h"

Bill::Bill() : billId(0), patientId(0), appointmentId(0), amount(0.0f) {
    status[0] = '\0'; date[0] = '\0';
}
Bill::Bill(int bId, int pId, int aId, float amt, const char* st, const char* d)
    : billId(bId), patientId(pId), appointmentId(aId), amount(amt) {
    strCopy(status, st, 12); strCopy(date, d, 12);
}