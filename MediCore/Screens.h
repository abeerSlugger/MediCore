#ifndef SCREENS_H
#define SCREENS_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "HospitalSystem.h"
#include "Button.h"
#include "TextBox.h"
#include "UIHelper.h"

// ── Screen enum — every screen has its own value ─────────────
enum class Screen {
    RoleSelect, Login,
    // Patient
    PatientMenu, BookAppointment, CancelAppointment,
    ViewAppointments, ViewRecords, ViewBills, PayBill, TopUp,
    // Doctor
    DoctorMenu, DoctorToday, MarkComplete, MarkNoShow,
    WritePrescription, PatientHistory,
    // Admin
    AdminMenu, AddDoctor, RemoveDoctor, AddPatient,
    AdminPatients, AdminDoctors, AdminAllAppts,
    AdminUnpaidBills, DischargePatient, SecurityLog, DailyReport,
    Exit
};

// ── Audio ─────────────────────────────────────────────────────
struct Audio {
    sf::SoundBuffer clickBuf, successBuf, errorBuf;
    sf::Sound       click, success, error;
    sf::Music       bgMusic;
    void load(); void playClick(); void playSuccess(); void playError();
};

// ── RoleSelectScreen ─────────────────────────────────────────
class RoleSelectScreen {
    Button btnPatient, btnDoctor, btnAdmin, btnExit;
public:
    void init(sf::Font& f);
    void handle(sf::Event& e, Screen& cur, Audio& audio, int& role);
    void draw(sf::RenderWindow& win, sf::Font& f);
};

// ── LoginScreen ──────────────────────────────────────────────
class LoginScreen {
    TextBox tbId, tbPw; Button btnLogin, btnBack;
    char msg[200]; int failCount; bool locked;
    void checkLock(int role, int id);
public:
    LoginScreen();
    void init(sf::Font& f); void reset();
    void handle(sf::Event& e, Screen& cur, Audio& audio,
        HospitalSystem& sys, int role, Patient*& lp, Doctor*& ld);
    void draw(sf::RenderWindow& win, sf::Font& f, int role);
};

// ── PatientMenuScreen ────────────────────────────────────────
class PatientMenuScreen {
    Button btns[8]; char subtitle[150];
public:
    void init(sf::Font& f);
    void setSubtitle(const char* name, float bal);
    void handle(sf::Event& e, Screen& cur, Audio& audio);
    void draw(sf::RenderWindow& win, sf::Font& f);
};

// ── CancelAppointmentScreen ──────────────────────────────────
class CancelAppointmentScreen {
    TextBox tbApptId; Button btnCancel, btnBack;
    char msg[200]; bool msgIsError;
    Appointment appts[50]; int count;
public:
    CancelAppointmentScreen();
    void init(sf::Font& f);
    void refresh(HospitalSystem& sys, int pid);
    void handle(sf::Event& e, Screen& cur, Audio& audio, HospitalSystem& sys, Patient* p);
    void draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys);
};

// ── ViewRecordsScreen ────────────────────────────────────────
class ViewRecordsScreen {
    Button btnBack; Prescription prescriptions[50]; int count;
public:
    ViewRecordsScreen();
    void init(sf::Font& f);
    void refresh(HospitalSystem& sys, int pid);
    void handle(sf::Event& e, Screen& cur, Audio& audio);
    void draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys);
};

// ── BookAppointmentScreen ────────────────────────────────────
class BookAppointmentScreen {
    TextBox tbSpec, tbDocId, tbDate, tbSlot;
    Button btnSearch, btnBook, btnBack;
    char msg[200]; bool msgIsError;
    Doctor foundDocs[20]; int foundCount;
    char availSlots[8][6]; int slotCount;
    void refreshSlots(HospitalSystem& sys);
public:
    BookAppointmentScreen();
    void init(sf::Font& f); void reset();
    void handle(sf::Event& e, Screen& cur, Audio& audio, HospitalSystem& sys, Patient* p);
    void draw(sf::RenderWindow& win, sf::Font& f);
};

// ── ViewAppointmentsScreen ───────────────────────────────────
class ViewAppointmentsScreen {
    Button btnBack; Appointment appts[50]; int count;
public:
    ViewAppointmentsScreen();
    void init(sf::Font& f);
    void refresh(HospitalSystem& sys, int pid);
    void handle(sf::Event& e, Screen& cur, Audio& audio);
    void draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys);
};

// ── ViewBillsScreen ──────────────────────────────────────────
class ViewBillsScreen {
    Button btnBack; Bill billArr[50]; int count;
public:
    ViewBillsScreen();
    void init(sf::Font& f);
    void refresh(HospitalSystem& sys, int pid);
    void handle(sf::Event& e, Screen& cur, Audio& audio);
    void draw(sf::RenderWindow& win, sf::Font& f);
};

// ── PayBillScreen ────────────────────────────────────────────
class PayBillScreen {
    TextBox tbBillId; Button btnPay, btnBack;
    char msg[200]; bool msgIsError; Bill unpaid[50]; int unpaidCount;
public:
    PayBillScreen();
    void init(sf::Font& f);
    void refresh(HospitalSystem& sys, int pid);
    void handle(sf::Event& e, Screen& cur, Audio& audio, HospitalSystem& sys, Patient* p);
    void draw(sf::RenderWindow& win, sf::Font& f);
};

// ── TopUpScreen ──────────────────────────────────────────────
class TopUpScreen {
    TextBox tbAmount; Button btnTopUp, btnBack;
    char msg[200]; bool msgIsError;
public:
    TopUpScreen();
    void init(sf::Font& f);
    void handle(sf::Event& e, Screen& cur, Audio& audio, HospitalSystem& sys, Patient* p);
    void draw(sf::RenderWindow& win, sf::Font& f);
};

// ── DoctorMenuScreen ─────────────────────────────────────────
class DoctorMenuScreen {
    Button btns[6]; char subtitle[150];
public:
    void init(sf::Font& f);
    void setSubtitle(const char* name, const char* spec);
    void handle(sf::Event& e, Screen& cur, Audio& audio);
    void draw(sf::RenderWindow& win, sf::Font& f);
};

// ── DoctorTodayScreen ────────────────────────────────────────
class DoctorTodayScreen {
    Button btnBack; Appointment appts[20]; int count;
public:
    DoctorTodayScreen();
    void init(sf::Font& f);
    void refresh(HospitalSystem& sys, int did);
    void handle(sf::Event& e, Screen& cur, Audio& audio);
    void draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys);
};
class MarkCompleteScreen {
    TextBox tbApptId; Button btnMark, btnBack;
    char msg[200]; bool msgIsError; Appointment appts[20]; int count;
public:
    MarkCompleteScreen();
    void init(sf::Font& f);
    void refresh(HospitalSystem& sys, int did);
    void handle(sf::Event& e, Screen& cur, Audio& audio, HospitalSystem& sys, Doctor* d);
    void draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys);
};

// ── MarkNoShowScreen ─────────────────────────────────────────
class MarkNoShowScreen {
    TextBox tbApptId; Button btnMark, btnBack;
    char msg[200]; bool msgIsError; Appointment appts[20]; int count;
public:
    MarkNoShowScreen();
    void init(sf::Font& f);
    void refresh(HospitalSystem& sys, int did);
    void handle(sf::Event& e, Screen& cur, Audio& audio, HospitalSystem& sys, Doctor* d);
    void draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys);
};

// ── PatientHistoryScreen ─────────────────────────────────────
class PatientHistoryScreen {
    TextBox tbPatientId; Button btnSearch, btnBack;
    char msg[200]; bool msgIsError; Prescription prescriptions[50]; int count;
public:
    PatientHistoryScreen();
    void init(sf::Font& f);
    void handle(sf::Event& e, Screen& cur, Audio& audio, HospitalSystem& sys, Doctor* d);
    void draw(sf::RenderWindow& win, sf::Font& f);
};

// ── WritePrescriptionScreen ──────────────────────────────────
class WritePrescriptionScreen {
    TextBox tbApptId, tbMeds, tbNotes; Button btnSave, btnBack;
    char msg[200]; bool msgIsError;
public:
    WritePrescriptionScreen();
    void init(sf::Font& f);
    void handle(sf::Event& e, Screen& cur, Audio& audio, HospitalSystem& sys, Doctor* d);
    void draw(sf::RenderWindow& win, sf::Font& f);
};

// ── AdminMenuScreen ──────────────────────────────────────────
class AdminMenuScreen {
    Button btns[11];
public:
    void init(sf::Font& f);
    void handle(sf::Event& e, Screen& cur, Audio& audio);
    void draw(sf::RenderWindow& win, sf::Font& f);
};

// ── AddPatientScreen ─────────────────────────────────────────
class AddPatientScreen {
    TextBox tbName, tbAge, tbGender, tbContact, tbPw, tbBalance;
    Button btnAdd, btnBack; char msg[200]; bool msgIsError;
public:
    AddPatientScreen();
    void init(sf::Font& f);
    void handle(sf::Event& e, Screen& cur, Audio& audio, HospitalSystem& sys);
    void draw(sf::RenderWindow& win, sf::Font& f);
};

// ── AddDoctorScreen ──────────────────────────────────────────
class AddDoctorScreen {
    TextBox tbName, tbSpec, tbContact, tbPw, tbFee;
    Button btnAdd, btnBack; char msg[200]; bool msgIsError;
public:
    AddDoctorScreen();
    void init(sf::Font& f);
    void handle(sf::Event& e, Screen& cur, Audio& audio, HospitalSystem& sys);
    void draw(sf::RenderWindow& win, sf::Font& f);
};

// ── RemoveDoctorScreen ───────────────────────────────────────
class RemoveDoctorScreen {
    TextBox tbDocId; Button btnRemove, btnBack;
    char msg[200]; bool msgIsError;
public:
    RemoveDoctorScreen();
    void init(sf::Font& f);
    void handle(sf::Event& e, Screen& cur, Audio& audio, HospitalSystem& sys);
    void draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys);
};

// ── AdminPatientsScreen ──────────────────────────────────────
class AdminPatientsScreen {
    Button btnBack;
public:
    void init(sf::Font& f);
    void handle(sf::Event& e, Screen& cur, Audio& audio);
    void draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys);
};

// ── AdminDoctorsScreen ───────────────────────────────────────
class AdminDoctorsScreen {
    Button btnBack;
public:
    AdminDoctorsScreen();
    void init(sf::Font& f);
    void handle(sf::Event& e, Screen& cur, Audio& audio);
    void draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys);
};

// ── AdminAllApptsScreen ──────────────────────────────────────
class AdminAllApptsScreen {
    Button btnBack; Appointment appts[100]; int count;
public:
    AdminAllApptsScreen();
    void init(sf::Font& f);
    void refresh(HospitalSystem& sys);
    void handle(sf::Event& e, Screen& cur, Audio& audio);
    void draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys);
};

// ── AdminUnpaidBillsScreen ───────────────────────────────────
class AdminUnpaidBillsScreen {
    Button btnBack; Bill bills[100]; int count;
public:
    AdminUnpaidBillsScreen();
    void init(sf::Font& f);
    void refresh(HospitalSystem& sys);
    void handle(sf::Event& e, Screen& cur, Audio& audio);
    void draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys);
};

// ── DischargePatientScreen ───────────────────────────────────
class DischargePatientScreen {
    TextBox tbPatientId; Button btnDischarge, btnBack;
    char msg[200]; bool msgIsError;
public:
    DischargePatientScreen();
    void init(sf::Font& f);
    void handle(sf::Event& e, Screen& cur, Audio& audio, HospitalSystem& sys);
    void draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys);
};

// ── DailyReportScreen ────────────────────────────────────────
class DailyReportScreen {
    Button btnBack;
public:
    DailyReportScreen();
    void init(sf::Font& f);
    void handle(sf::Event& e, Screen& cur, Audio& audio);
    void draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys);
};

// ── SecurityLogScreen ────────────────────────────────────────
class SecurityLogScreen {
    Button btnBack;
public:
    void init(sf::Font& f);
    void handle(sf::Event& e, Screen& cur, Audio& audio);
    void draw(sf::RenderWindow& win, sf::Font& f);
};

#endif