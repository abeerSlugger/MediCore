#ifndef APPOINTMENT_H
#define APPOINTMENT_H
#include <iostream>
#include "Utils.h"

class Appointment {
    int  appointmentId, patientId, doctorId;
    char date[12], timeSlot[6], status[12];
public:
    Appointment();
    Appointment(int aId, int pId, int dId,
        const char* date, const char* slot, const char* status);

    int         getId()        const { return appointmentId; }
    int         getPatientId() const { return patientId; }
    int         getDoctorId()  const { return doctorId; }
    const char* getDate()      const { return date; }
    const char* getTimeSlot()  const { return timeSlot; }
    const char* getStatus()    const { return status; }
    void setStatus(const char* s) { strCopy(status, s, 12); }

    bool operator==(const Appointment& o) const;
    friend std::ostream& operator<<(std::ostream& os, const Appointment& a);
};
#endif
