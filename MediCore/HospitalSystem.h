#ifndef HOSPITALSYSTEM_H
#define HOSPITALSYSTEM_H
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "FileHandler.h"
#include "Validator.h"
#include "HospitalException.h"

static const char* TIME_SLOTS[] = { "09:00","10:00","11:00","12:00","13:00","14:00","15:00","16:00" };
static const int   SLOT_COUNT = 8;

class HospitalSystem {
    Storage<Patient>      patients;
    Storage<Doctor>       doctors;
    Storage<Appointment>  appointments;
    Storage<Bill>         bills;
    Storage<Prescription> prescriptions;
    Admin                 admin;
public:
    HospitalSystem();
    void loadAllData();

    // Auth
    Patient* loginPatient(int id, const char* pw);
    Doctor* loginDoctor(int id, const char* pw);
    bool     loginAdmin(int id, const char* pw);

    // Patient ops
    bool bookAppointment(int pid, int did, const char* date, const char* slot, char* msg);
    bool cancelAppointment(int pid, int apptId, char* msg);
    bool payBill(int pid, int billId, char* msg);
    bool topUpBalance(int pid, float amt, char* msg);

    // Doctor ops
    bool markComplete(int did, int apptId, char* msg);
    bool markNoShow(int did, int apptId, char* msg);
    bool writePrescription(int did, int apptId, const char* meds, const char* notes, char* msg);

    // Admin ops
    bool addDoctor(const char* name, const char* spec, const char* contact, const char* pw, float fee, char* msg);
    bool removeDoctor(int did, char* msg);
    bool dischargePatient(int pid, char* msg);

    // Queries
    int getAppointmentsForPatient(int pid, Appointment* out, int max);
    int getBillsForPatient(int pid, Bill* out, int max);
    int getPrescriptionsForPatient(int pid, Prescription* out, int max);
    int getTodayAppointmentsForDoctor(int did, Appointment* out, int max);
    int getDoctorsBySpec(const char* spec, Doctor* out, int max);
    int getAvailableSlots(int did, const char* date, char slots[][6], int max);
    int getUnpaidBills(Bill* out, int max);
    int getAllAppointments(Appointment* out, int max);

    // Getters
    Patient* findPatient(int id) { return patients.findById(id); }
    Doctor* findDoctor(int id) { return doctors.findById(id); }
    Storage<Patient>& getPatients() { return patients; }
    Storage<Doctor>& getDoctors() { return doctors; }
    Storage<Appointment>& getAppointments() { return appointments; }
    Storage<Bill>& getBills() { return bills; }
    Storage<Prescription>& getPrescriptions() { return prescriptions; }

    void getTodayString(char* buf);
    int  getCurrentYear();
    bool isSlotTaken(int did, const char* date, const char* slot);
};
#endif