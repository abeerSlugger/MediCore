#ifndef BILL_H
#define BILL_H
#include "Utils.h"

class Bill {
    int   billId, patientId, appointmentId;
    float amount;
    char  status[12], date[12];
public:
    Bill();
    Bill(int bId, int pId, int aId, float amt,
        const char* status, const char* date);

    int         getId()            const { return billId; }
    int         getPatientId()     const { return patientId; }
    int         getAppointmentId() const { return appointmentId; }
    float       getAmount()        const { return amount; }
    const char* getStatus()        const { return status; }
    const char* getDate()          const { return date; }
    void setStatus(const char* s) { strCopy(status, s, 12); }
};
#endif