#include "HospitalSystem.h"
#include <ctime>

HospitalSystem::HospitalSystem() {}

void HospitalSystem::loadAllData() {
    FileHandler::loadPatients(patients);
    FileHandler::loadDoctors(doctors);
    FileHandler::loadAdmin(admin);
    FileHandler::loadAppointments(appointments);
    FileHandler::loadBills(bills);
    FileHandler::loadPrescriptions(prescriptions);
}

void HospitalSystem::getTodayString(char* buf) {
    time_t now = time(nullptr);
    strftime(buf, 12, "%d-%m-%Y", localtime(&now));
}

int HospitalSystem::getCurrentYear() {
    time_t now = time(nullptr); tm* t = localtime(&now); return t->tm_year + 1900;
}

Patient* HospitalSystem::loginPatient(int id, const char* pw) {
    Patient* p = patients.findById(id);
    return (p && p->checkPassword(pw)) ? p : nullptr;
}
Doctor* HospitalSystem::loginDoctor(int id, const char* pw) {
    Doctor* d = doctors.findById(id);
    return (d && d->checkPassword(pw)) ? d : nullptr;
}
bool HospitalSystem::loginAdmin(int id, const char* pw) {
    return admin.getId() == id && admin.checkPassword(pw);
}

bool HospitalSystem::isSlotTaken(int did, const char* date, const char* slot) {
    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.get(i);
        if (a.getDoctorId() == did &&
            strCmp(a.getDate(), date) == 0 &&
            strCmp(a.getTimeSlot(), slot) == 0 &&
            strCmp(a.getStatus(), "cancelled") != 0) return true;
    }
    return false;
}

int HospitalSystem::getAvailableSlots(int did, const char* date, char slots[][6], int max) {
    int n = 0;
    for (int i = 0; i < SLOT_COUNT && n < max; i++)
        if (!isSlotTaken(did, date, TIME_SLOTS[i])) { strCopy(slots[n], TIME_SLOTS[i], 6); n++; }
    return n;
}

bool HospitalSystem::bookAppointment(int pid, int did, const char* date, const char* slot, char* msg) {
    Patient* p = patients.findById(pid); Doctor* d = doctors.findById(did);
    if (!p || !d) { strCopy(msg, "Patient or doctor not found.", 200); return false; }
    if (isSlotTaken(did, date, slot)) {
        try { throw SlotUnavailableException(slot); }
        catch (SlotUnavailableException& e) { strCopy(msg, e.what(), 200); return false; }
    }
    if (p->getBalance() < d->getFee()) {
        try { throw InsufficientFundsException(d->getFee(), p->getBalance()); }
        catch (InsufficientFundsException& e) { strCopy(msg, e.what(), 200); return false; }
    }
    *p -= d->getFee(); FileHandler::updatePatient(*p);
    int aId = appointments.maxId() + 1;
    Appointment a(aId, pid, did, date, slot, "pending");
    appointments.add(a); FileHandler::appendAppointment(a);
    int bId = bills.maxId() + 1;
    Bill b(bId, pid, aId, d->getFee(), "unpaid", date);
    bills.add(b); FileHandler::appendBill(b);
    char idBuf[10]; intToStr(aId, idBuf);
    strCopy(msg, "Appointment booked! ID: ", 200); strConcat(msg, idBuf, 200);
    return true;
}

bool HospitalSystem::cancelAppointment(int pid, int apptId, char* msg) {
    Patient* p = patients.findById(pid);
    Appointment* a = appointments.findById(apptId);
    if (!a || a->getPatientId() != pid || strCmp(a->getStatus(), "pending") != 0) {
        strCopy(msg, "Invalid appointment ID.", 200); return false;
    }
    Doctor* d = doctors.findById(a->getDoctorId());
    float fee = d ? d->getFee() : 0;
    a->setStatus("cancelled"); FileHandler::updateAppointment(*a);
    if (p) { *p += fee; FileHandler::updatePatient(*p); }
    for (int i = 0; i < bills.size(); i++) {
        if (bills.get(i).getAppointmentId() == apptId) {
            bills.get(i).setStatus("cancelled");
            FileHandler::updateBill(bills.get(i)); break;
        }
    }
    char fb[20]; floatToStr(fee, fb);
    strCopy(msg, "Cancelled. PKR ", 200); strConcat(msg, fb, 200); strConcat(msg, " refunded.", 200);
    return true;
}

bool HospitalSystem::payBill(int pid, int billId, char* msg) {
    Patient* p = patients.findById(pid);
    Bill* b = bills.findById(billId);
    if (!b || b->getPatientId() != pid || strCmp(b->getStatus(), "unpaid") != 0) {
        strCopy(msg, "Invalid bill ID.", 200); return false;
    }
    if (p->getBalance() < b->getAmount()) {
        try { throw InsufficientFundsException(b->getAmount(), p->getBalance()); }
        catch (InsufficientFundsException& e) { strCopy(msg, e.what(), 200); return false; }
    }
    *p -= b->getAmount(); b->setStatus("paid");
    FileHandler::updatePatient(*p); FileHandler::updateBill(*b);
    char bb[20]; floatToStr(p->getBalance(), bb);
    strCopy(msg, "Bill paid. Balance: PKR ", 200); strConcat(msg, bb, 200);
    return true;
}

bool HospitalSystem::topUpBalance(int pid, float amt, char* msg) {
    if (amt <= 0) {
        try { throw InvalidInputException("Amount must be > 0."); }
        catch (InvalidInputException& e) { strCopy(msg, e.what(), 200); return false; }
    }
    Patient* p = patients.findById(pid);
    if (!p) { strCopy(msg, "Patient not found.", 200); return false; }
    *p += amt; FileHandler::updatePatient(*p);
    char bb[20]; floatToStr(p->getBalance(), bb);
    strCopy(msg, "Balance updated. PKR ", 200); strConcat(msg, bb, 200);
    return true;
}

bool HospitalSystem::markComplete(int did, int apptId, char* msg) {
    char today[12]; getTodayString(today);
    Appointment* a = appointments.findById(apptId);
    if (!a || a->getDoctorId() != did || strCmp(a->getStatus(), "pending") != 0 || strCmp(a->getDate(), today) != 0) {
        strCopy(msg, "Invalid appointment.", 200); return false;
    }
    a->setStatus("completed"); FileHandler::updateAppointment(*a);
    strCopy(msg, "Marked as completed.", 200); return true;
}

bool HospitalSystem::markNoShow(int did, int apptId, char* msg) {
    char today[12]; getTodayString(today);
    Appointment* a = appointments.findById(apptId);
    if (!a || a->getDoctorId() != did || strCmp(a->getStatus(), "pending") != 0 || strCmp(a->getDate(), today) != 0) {
        strCopy(msg, "Invalid appointment.", 200); return false;
    }
    a->setStatus("noshow"); FileHandler::updateAppointment(*a);
    for (int i = 0; i < bills.size(); i++) {
        if (bills.get(i).getAppointmentId() == apptId) {
            bills.get(i).setStatus("cancelled");
            FileHandler::updateBill(bills.get(i)); break;
        }
    }
    strCopy(msg, "Marked as no-show.", 200); return true;
}

bool HospitalSystem::writePrescription(int did, int apptId, const char* meds, const char* notes, char* msg) {
    Appointment* a = appointments.findById(apptId);
    if (!a || a->getDoctorId() != did || strCmp(a->getStatus(), "completed") != 0) {
        strCopy(msg, "Invalid appointment.", 200); return false;
    }
    for (int i = 0; i < prescriptions.size(); i++)
        if (prescriptions.get(i).getAppointmentId() == apptId) {
            strCopy(msg, "Prescription already written.", 200); return false;
        }
    int prId = prescriptions.maxId() + 1;
    Prescription pr(prId, apptId, a->getPatientId(), did, a->getDate(), meds, notes);
    prescriptions.add(pr); FileHandler::appendPrescription(pr);
    strCopy(msg, "Prescription saved.", 200); return true;
}

bool HospitalSystem::addDoctor(const char* name, const char* spec, const char* contact, const char* pw, float fee, char* msg) {
    int dId = doctors.maxId() + 1;
    Doctor d(dId, name, spec, contact, pw, fee);
    doctors.add(d); FileHandler::appendDoctor(d);
    char idBuf[10]; intToStr(dId, idBuf);
    strCopy(msg, "Doctor added. ID: ", 200); strConcat(msg, idBuf, 200);
    return true;
}

bool HospitalSystem::removeDoctor(int did, char* msg) {
    for (int i = 0; i < appointments.size(); i++)
        if (appointments.get(i).getDoctorId() == did && strCmp(appointments.get(i).getStatus(), "pending") == 0) {
            strCopy(msg, "Cannot remove: doctor has pending appointments.", 200); return false;
        }
    doctors.removeById(did); FileHandler::deleteDoctor(did);
    strCopy(msg, "Doctor removed.", 200); return true;
}

bool HospitalSystem::dischargePatient(int pid, char* msg) {
    Patient* p = patients.findById(pid);
    if (!p) { strCopy(msg, "Patient not found.", 200); return false; }
    for (int i = 0; i < bills.size(); i++)
        if (bills.get(i).getPatientId() == pid && strCmp(bills.get(i).getStatus(), "unpaid") == 0) {
            strCopy(msg, "Cannot discharge: unpaid bills exist.", 200); return false;
        }
    for (int i = 0; i < appointments.size(); i++)
        if (appointments.get(i).getPatientId() == pid && strCmp(appointments.get(i).getStatus(), "pending") == 0) {
            strCopy(msg, "Cannot discharge: pending appointments exist.", 200); return false;
        }
    FileHandler::archivePatient(*p, appointments, bills, prescriptions);
    FileHandler::deleteAppointmentsByPatient(pid);
    FileHandler::deleteBillsByPatient(pid);
    FileHandler::deletePrescriptionsByPatient(pid);
    FileHandler::deletePatient(pid);
    patients.removeById(pid);
    strCopy(msg, "Patient discharged and archived.", 200); return true;
}

int HospitalSystem::getAppointmentsForPatient(int pid, Appointment* out, int max) {
    int n = 0;
    for (int i = 0; i < appointments.size() && n < max; i++)
        if (appointments.get(i).getPatientId() == pid) out[n++] = appointments.get(i);
    for (int i = 0; i < n - 1; i++) for (int j = 0; j < n - i - 1; j++)
        if (strCmp(out[j].getDate(), out[j + 1].getDate()) > 0) { Appointment t = out[j]; out[j] = out[j + 1]; out[j + 1] = t; }
    return n;
}
int HospitalSystem::getBillsForPatient(int pid, Bill* out, int max) {
    int n = 0;
    for (int i = 0; i < bills.size() && n < max; i++)
        if (bills.get(i).getPatientId() == pid) out[n++] = bills.get(i);
    return n;
}
int HospitalSystem::getPrescriptionsForPatient(int pid, Prescription* out, int max) {
    int n = 0;
    for (int i = 0; i < prescriptions.size() && n < max; i++)
        if (prescriptions.get(i).getPatientId() == pid) out[n++] = prescriptions.get(i);
    for (int i = 0; i < n - 1; i++) for (int j = 0; j < n - i - 1; j++)
        if (strCmp(out[j].getDate(), out[j + 1].getDate()) < 0) { Prescription t = out[j]; out[j] = out[j + 1]; out[j + 1] = t; }
    return n;
}
int HospitalSystem::getTodayAppointmentsForDoctor(int did, Appointment* out, int max) {
    char today[12]; getTodayString(today); int n = 0;
    for (int i = 0; i < appointments.size() && n < max; i++) {
        Appointment& a = appointments.get(i);
        if (a.getDoctorId() == did && strCmp(a.getDate(), today) == 0) out[n++] = a;
    }
    for (int i = 0; i < n - 1; i++) for (int j = 0; j < n - i - 1; j++)
        if (strCmp(out[j].getTimeSlot(), out[j + 1].getTimeSlot()) > 0) { Appointment t = out[j]; out[j] = out[j + 1]; out[j + 1] = t; }
    return n;
}
int HospitalSystem::getDoctorsBySpec(const char* spec, Doctor* out, int max) {
    int n = 0;
    for (int i = 0; i < doctors.size() && n < max; i++)
        if (strCmpInsensitive(doctors.get(i).getSpecialization(), spec) == 0) out[n++] = doctors.get(i);
    return n;
}
int HospitalSystem::getUnpaidBills(Bill* out, int max) {
    int n = 0;
    for (int i = 0; i < bills.size() && n < max; i++)
        if (strCmp(bills.get(i).getStatus(), "unpaid") == 0) out[n++] = bills.get(i);
    return n;
}
int HospitalSystem::getAllAppointments(Appointment* out, int max) {
    int n = 0;
    for (int i = 0; i < appointments.size() && n < max; i++) out[n++] = appointments.get(i);
    for (int i = 0; i < n - 1; i++) for (int j = 0; j < n - i - 1; j++)
        if (strCmp(out[j].getDate(), out[j + 1].getDate()) < 0) { Appointment t = out[j]; out[j] = out[j + 1]; out[j + 1] = t; }
    return n;
}