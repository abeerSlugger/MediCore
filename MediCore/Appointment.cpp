#include "Appointment.h"

Appointment::Appointment() : appointmentId(0), patientId(0), doctorId(0) {
    date[0] = timeSlot[0] = status[0] = '\0';
}
Appointment::Appointment(int aId, int pId, int dId,
    const char* d, const char* s, const char* st)
    : appointmentId(aId), patientId(pId), doctorId(dId) {
    strCopy(date, d, 12); strCopy(timeSlot, s, 6); strCopy(status, st, 12);
}
bool Appointment::operator==(const Appointment& o) const {
    if (doctorId != o.doctorId)             return false;
    if (strCmp(date, o.date) != 0)           return false;
    if (strCmp(timeSlot, o.timeSlot) != 0)   return false;
    if (strCmp(status, "cancelled") == 0)    return false;
    if (strCmp(o.status, "cancelled") == 0)  return false;
    return true;
}
std::ostream& operator<<(std::ostream& os, const Appointment& a) {
    os << "ID:" << a.appointmentId << " Patient:" << a.patientId
        << " Doctor:" << a.doctorId << " Date:" << a.date
        << " Slot:" << a.timeSlot << " Status:" << a.status;
    return os;
}