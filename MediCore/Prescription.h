#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H
#include "Utils.h"

class Prescription {
    int  prescriptionId, appointmentId, patientId, doctorId;
    char date[12], medicines[500], notes[300];
public:
    Prescription();
    Prescription(int prId, int aId, int pId, int dId,
        const char* date, const char* meds, const char* notes);

    int         getId()            const { return prescriptionId; }
    int         getAppointmentId() const { return appointmentId; }
    int         getPatientId()     const { return patientId; }
    int         getDoctorId()      const { return doctorId; }
    const char* getDate()          const { return date; }
    const char* getMedicines()     const { return medicines; }
    const char* getNotes()         const { return notes; }
};
#endif
