#include "FileHandler.h"
#include <iostream>

//  Line builders 
void FileHandler::buildPatientLine(const Patient& p, char* buf, int len) {
    char id[10], age[10], bal[20];
    intToStr(p.getId(), id); intToStr(p.getAge(), age); floatToStr(p.getBalance(), bal);
    strCopy(buf, id, len);
    strConcat(buf, ",", len); strConcat(buf, p.getName(), len);
    strConcat(buf, ",", len); strConcat(buf, age, len);
    strConcat(buf, ",", len); strConcat(buf, p.getGender(), len);
    strConcat(buf, ",", len); strConcat(buf, p.getContact(), len);
    strConcat(buf, ",", len); strConcat(buf, p.getPassword(), len);
    strConcat(buf, ",", len); strConcat(buf, bal, len);
    strConcat(buf, "\n", len);
}
void FileHandler::buildDoctorLine(const Doctor& d, char* buf, int len) {
    char id[10], fee[20];
    intToStr(d.getId(), id); floatToStr(d.getFee(), fee);
    strCopy(buf, id, len);
    strConcat(buf, ",", len); strConcat(buf, d.getName(), len);
    strConcat(buf, ",", len); strConcat(buf, d.getSpecialization(), len);
    strConcat(buf, ",", len); strConcat(buf, d.getContact(), len);
    strConcat(buf, ",", len); strConcat(buf, d.getPassword(), len);
    strConcat(buf, ",", len); strConcat(buf, fee, len);
    strConcat(buf, "\n", len);
}
void FileHandler::buildAppointmentLine(const Appointment& a, char* buf, int len) {
    char aId[10], pId[10], dId[10];
    intToStr(a.getId(), aId); intToStr(a.getPatientId(), pId); intToStr(a.getDoctorId(), dId);
    strCopy(buf, aId, len);
    strConcat(buf, ",", len); strConcat(buf, pId, len);
    strConcat(buf, ",", len); strConcat(buf, dId, len);
    strConcat(buf, ",", len); strConcat(buf, a.getDate(), len);
    strConcat(buf, ",", len); strConcat(buf, a.getTimeSlot(), len);
    strConcat(buf, ",", len); strConcat(buf, a.getStatus(), len);
    strConcat(buf, "\n", len);
}
void FileHandler::buildBillLine(const Bill& b, char* buf, int len) {
    char bId[10], pId[10], aId[10], amt[20];
    intToStr(b.getId(), bId); intToStr(b.getPatientId(), pId);
    intToStr(b.getAppointmentId(), aId); floatToStr(b.getAmount(), amt);
    strCopy(buf, bId, len);
    strConcat(buf, ",", len); strConcat(buf, pId, len);
    strConcat(buf, ",", len); strConcat(buf, aId, len);
    strConcat(buf, ",", len); strConcat(buf, amt, len);
    strConcat(buf, ",", len); strConcat(buf, b.getStatus(), len);
    strConcat(buf, ",", len); strConcat(buf, b.getDate(), len);
    strConcat(buf, "\n", len);
}
void FileHandler::buildPrescriptionLine(const Prescription& pr, char* buf, int len) {
    char prId[10], aId[10], pId[10], dId[10];
    intToStr(pr.getId(), prId); intToStr(pr.getAppointmentId(), aId);
    intToStr(pr.getPatientId(), pId); intToStr(pr.getDoctorId(), dId);
    strCopy(buf, prId, len);
    strConcat(buf, ",", len); strConcat(buf, aId, len);
    strConcat(buf, ",", len); strConcat(buf, pId, len);
    strConcat(buf, ",", len); strConcat(buf, dId, len);
    strConcat(buf, ",", len); strConcat(buf, pr.getDate(), len);
    strConcat(buf, ",", len); strConcat(buf, pr.getMedicines(), len);
    strConcat(buf, ",", len); strConcat(buf, pr.getNotes(), len);
    strConcat(buf, "\n", len);
}

//  Load 
void FileHandler::loadPatients(Storage<Patient>& s) {
    FILE* f = fopen(FILE_PATIENTS, "r"); if (!f) return;
    char line[400];
    while (fgets(line, sizeof(line), f)) {
        trimNewline(line); if (!line[0]) continue;
        char t[10][200]; int n = parseCSV(line, t, 10); if (n < 7) continue;
        s.add(Patient(strToInt(t[0]), t[1], strToInt(t[2]), t[3], t[4], t[5], strToFloat(t[6])));
    }
    fclose(f);
}
void FileHandler::loadDoctors(Storage<Doctor>& s) {
    FILE* f = fopen(FILE_DOCTORS, "r"); if (!f) return;
    char line[300];
    while (fgets(line, sizeof(line), f)) {
        trimNewline(line); if (!line[0]) continue;
        char t[8][200]; int n = parseCSV(line, t, 8); if (n < 6) continue;
        s.add(Doctor(strToInt(t[0]), t[1], t[2], t[3], t[4], strToFloat(t[5])));
    }
    fclose(f);
}
bool FileHandler::loadAdmin(Admin& admin) {
    FILE* f = fopen(FILE_ADMIN, "r");
    if (!f) throw FileNotFoundException(FILE_ADMIN);
    char line[200];
    while (fgets(line, sizeof(line), f)) {
        trimNewline(line); if (!line[0]) continue;
        char t[4][200]; int n = parseCSV(line, t, 4); if (n < 3) continue;
        admin = Admin(strToInt(t[0]), t[1], t[2]); fclose(f); return true;
    }
    fclose(f); return false;
}
void FileHandler::loadAppointments(Storage<Appointment>& s) {
    FILE* f = fopen(FILE_APPOINTMENTS, "r"); if (!f) return;
    char line[200];
    while (fgets(line, sizeof(line), f)) {
        trimNewline(line); if (!line[0]) continue;
        char t[8][200]; int n = parseCSV(line, t, 8); if (n < 6) continue;
        s.add(Appointment(strToInt(t[0]), strToInt(t[1]), strToInt(t[2]), t[3], t[4], t[5]));
    }
    fclose(f);
}
void FileHandler::loadBills(Storage<Bill>& s) {
    FILE* f = fopen(FILE_BILLS, "r"); if (!f) return;
    char line[200];
    while (fgets(line, sizeof(line), f)) {
        trimNewline(line); if (!line[0]) continue;
        char t[8][200]; int n = parseCSV(line, t, 8); if (n < 6) continue;
        s.add(Bill(strToInt(t[0]), strToInt(t[1]), strToInt(t[2]), strToFloat(t[3]), t[4], t[5]));
    }
    fclose(f);
}
void FileHandler::loadPrescriptions(Storage<Prescription>& s) {
    FILE* f = fopen(FILE_PRESCRIPTIONS, "r"); if (!f) return;
    char line[900];
    while (fgets(line, sizeof(line), f)) {
        trimNewline(line); if (!line[0]) continue;
        char t[8][200]; int n = parseCSV(line, t, 8); if (n < 7) continue;
        s.add(Prescription(strToInt(t[0]), strToInt(t[1]), strToInt(t[2]), strToInt(t[3]), t[4], t[5], t[6]));
    }
    fclose(f);
}

// Append
void FileHandler::appendPatient(const Patient& p) {
    FILE* f = fopen(FILE_PATIENTS, "a"); if (!f) return;
    char line[400]; buildPatientLine(p, line, 400); fputs(line, f); fclose(f);
}
void FileHandler::appendDoctor(const Doctor& d) {
    FILE* f = fopen(FILE_DOCTORS, "a"); if (!f) return;
    char line[300]; buildDoctorLine(d, line, 300); fputs(line, f); fclose(f);
}
void FileHandler::appendAppointment(const Appointment& a) {
    FILE* f = fopen(FILE_APPOINTMENTS, "a"); if (!f) return;
    char line[200]; buildAppointmentLine(a, line, 200); fputs(line, f); fclose(f);
}
void FileHandler::appendBill(const Bill& b) {
    FILE* f = fopen(FILE_BILLS, "a"); if (!f) return;
    char line[200]; buildBillLine(b, line, 200); fputs(line, f); fclose(f);
}
void FileHandler::appendPrescription(const Prescription& pr) {
    FILE* f = fopen(FILE_PRESCRIPTIONS, "a"); if (!f) return;
    char line[900]; buildPrescriptionLine(pr, line, 900); fputs(line, f); fclose(f);
}

// Rewrite 
void FileHandler::rewritePatients(Storage<Patient>& s) {
    FILE* f = fopen(FILE_PATIENTS, "w"); if (!f) return;
    char line[400]; for (int i = 0; i < s.size(); i++) { buildPatientLine(s.get(i), line, 400); fputs(line, f); }
    fclose(f);
}
void FileHandler::rewriteDoctors(Storage<Doctor>& s) {
    FILE* f = fopen(FILE_DOCTORS, "w"); if (!f) return;
    char line[300]; for (int i = 0; i < s.size(); i++) { buildDoctorLine(s.get(i), line, 300); fputs(line, f); }
    fclose(f);
}
void FileHandler::rewriteAppointments(Storage<Appointment>& s) {
    FILE* f = fopen(FILE_APPOINTMENTS, "w"); if (!f) return;
    char line[200]; for (int i = 0; i < s.size(); i++) { buildAppointmentLine(s.get(i), line, 200); fputs(line, f); }
    fclose(f);
}
void FileHandler::rewriteBills(Storage<Bill>& s) {
    FILE* f = fopen(FILE_BILLS, "w"); if (!f) return;
    char line[200]; for (int i = 0; i < s.size(); i++) { buildBillLine(s.get(i), line, 200); fputs(line, f); }
    fclose(f);
}
void FileHandler::rewritePrescriptions(Storage<Prescription>& s) {
    FILE* f = fopen(FILE_PRESCRIPTIONS, "w"); if (!f) return;
    char line[900]; for (int i = 0; i < s.size(); i++) { buildPrescriptionLine(s.get(i), line, 900); fputs(line, f); }
    fclose(f);
}

//  Update
void FileHandler::updatePatient(const Patient& p) {
    Storage<Patient> s; loadPatients(s);
    for (int i = 0; i < s.size(); i++) if (s.get(i).getId() == p.getId()) { s.get(i) = p; break; }
    rewritePatients(s);
}
void FileHandler::updateDoctor(const Doctor& d) {
    Storage<Doctor> s; loadDoctors(s);
    for (int i = 0; i < s.size(); i++) if (s.get(i).getId() == d.getId()) { s.get(i) = d; break; }
    rewriteDoctors(s);
}
void FileHandler::updateAppointment(const Appointment& a) {
    Storage<Appointment> s; loadAppointments(s);
    for (int i = 0; i < s.size(); i++) if (s.get(i).getId() == a.getId()) { s.get(i) = a; break; }
    rewriteAppointments(s);
}
void FileHandler::updateBill(const Bill& b) {
    Storage<Bill> s; loadBills(s);
    for (int i = 0; i < s.size(); i++) if (s.get(i).getId() == b.getId()) { s.get(i) = b; break; }
    rewriteBills(s);
}

// Delete
void FileHandler::deletePatient(int id) {
    Storage<Patient> s; loadPatients(s); s.removeById(id); rewritePatients(s);
}
void FileHandler::deleteDoctor(int id) {
    Storage<Doctor> s; loadDoctors(s); s.removeById(id); rewriteDoctors(s);
}
void FileHandler::deleteAppointmentsByPatient(int pid) {
    Storage<Appointment> s; loadAppointments(s);
    for (int i = s.size() - 1; i >= 0; i--) if (s.get(i).getPatientId() == pid) s.removeAt(i);
    rewriteAppointments(s);
}
void FileHandler::deleteBillsByPatient(int pid) {
    Storage<Bill> s; loadBills(s);
    for (int i = s.size() - 1; i >= 0; i--) if (s.get(i).getPatientId() == pid) s.removeAt(i);
    rewriteBills(s);
}
void FileHandler::deletePrescriptionsByPatient(int pid) {
    Storage<Prescription> s; loadPrescriptions(s);
    for (int i = s.size() - 1; i >= 0; i--) if (s.get(i).getPatientId() == pid) s.removeAt(i);
    rewritePrescriptions(s);
}

//  Security log 
void FileHandler::logSecurityEvent(const char* role, const char* id, const char* result) {
    FILE* f = fopen(FILE_SECURITY_LOG, "a"); if (!f) return;
    time_t now = time(nullptr); char ts[30];
    strftime(ts, sizeof(ts), "%d-%m-%Y %H:%M:%S", localtime(&now));
    char line[300]; strCopy(line, ts, 300);
    strConcat(line, ",", 300); strConcat(line, role, 300);
    strConcat(line, ",", 300); strConcat(line, id, 300);
    strConcat(line, ",", 300); strConcat(line, result, 300);
    strConcat(line, "\n", 300);
    fputs(line, f); fclose(f);
}
void FileHandler::displaySecurityLog() {
    FILE* f = fopen(FILE_SECURITY_LOG, "r");
    if (!f) { std::cout << "No security events logged.\n"; return; }
    char line[300]; bool empty = true;
    while (fgets(line, sizeof(line), f)) { std::cout << line; empty = false; }
    fclose(f); if (empty) std::cout << "No security events logged.\n";
}

//  Archive 
void FileHandler::archivePatient(const Patient& p,
    Storage<Appointment>& a,
    Storage<Bill>& b,
    Storage<Prescription>& pr) {
    FILE* f = fopen(FILE_DISCHARGED, "a"); if (!f) return;
    char line[900];
    buildPatientLine(p, line, 900); fputs(line, f);
    for (int i = 0; i < a.size(); i++) if (a.get(i).getPatientId() == p.getId()) { buildAppointmentLine(a.get(i), line, 900); fputs(line, f); }
    for (int i = 0; i < b.size(); i++) if (b.get(i).getPatientId() == p.getId()) { buildBillLine(b.get(i), line, 900); fputs(line, f); }
    for (int i = 0; i < pr.size(); i++) if (pr.get(i).getPatientId() == p.getId()) { buildPrescriptionLine(pr.get(i), line, 900); fputs(line, f); }
    fclose(f);
}