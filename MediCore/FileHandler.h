#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <cstdio>
#include <ctime>
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "HospitalException.h"

static const char* FILE_PATIENTS = "patients.txt";
static const char* FILE_DOCTORS = "doctors.txt";
static const char* FILE_ADMIN = "admin.txt";
static const char* FILE_APPOINTMENTS = "appointments.txt";
static const char* FILE_BILLS = "bills.txt";
static const char* FILE_PRESCRIPTIONS = "prescriptions.txt";
static const char* FILE_SECURITY_LOG = "security_log.txt";
static const char* FILE_DISCHARGED = "discharged.txt";

class FileHandler {
public:
    static void loadPatients(Storage<Patient>& s);
    static void loadDoctors(Storage<Doctor>& s);
    static bool loadAdmin(Admin& admin);
    static void loadAppointments(Storage<Appointment>& s);
    static void loadBills(Storage<Bill>& s);
    static void loadPrescriptions(Storage<Prescription>& s);

    static void appendPatient(const Patient& p);
    static void appendDoctor(const Doctor& d);
    static void appendAppointment(const Appointment& a);
    static void appendBill(const Bill& b);
    static void appendPrescription(const Prescription& pr);

    static void updatePatient(const Patient& p);
    static void updateDoctor(const Doctor& d);
    static void updateAppointment(const Appointment& a);
    static void updateBill(const Bill& b);

    static void deletePatient(int id);
    static void deleteDoctor(int id);
    static void deleteAppointmentsByPatient(int pid);
    static void deleteBillsByPatient(int pid);
    static void deletePrescriptionsByPatient(int pid);

    static void logSecurityEvent(const char* role, const char* id, const char* result);
    static void displaySecurityLog();
    static void archivePatient(const Patient& p,
        Storage<Appointment>& a,
        Storage<Bill>& b,
        Storage<Prescription>& pr);
private:
    static void buildPatientLine(const Patient& p, char* buf, int len);
    static void buildDoctorLine(const Doctor& d, char* buf, int len);
    static void buildAppointmentLine(const Appointment& a, char* buf, int len);
    static void buildBillLine(const Bill& b, char* buf, int len);
    static void buildPrescriptionLine(const Prescription& pr, char* buf, int len);
    static void rewritePatients(Storage<Patient>& s);
    static void rewriteDoctors(Storage<Doctor>& s);
    static void rewriteAppointments(Storage<Appointment>& s);
    static void rewriteBills(Storage<Bill>& s);
    static void rewritePrescriptions(Storage<Prescription>& s);
};
#endif