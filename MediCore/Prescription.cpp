#include "Prescription.h"

Prescription::Prescription()
    : prescriptionId(0), appointmentId(0), patientId(0), doctorId(0) {
    date[0] = medicines[0] = notes[0] = '\0';
}
Prescription::Prescription(int prId, int aId, int pId, int dId,
    const char* d, const char* m, const char* n)
    : prescriptionId(prId), appointmentId(aId), patientId(pId), doctorId(dId) {
    strCopy(date, d, 12); strCopy(medicines, m, 500); strCopy(notes, n, 300);
}