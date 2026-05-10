#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include "HospitalSystem.h"
#include "Screens.h"

int main() {
   

    sf::RenderWindow window(sf::VideoMode(900, 620),
        "MediCore Hospital Management System",
        sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
        if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
            return -1;

    Audio audio;
    audio.load();

    HospitalSystem sys;
    sys.loadAllData();

    // ── All screens 
    RoleSelectScreen        roleScreen;
    LoginScreen             loginScreen;
    PatientMenuScreen       patientMenu;
    BookAppointmentScreen   bookScreen;
    CancelAppointmentScreen cancelScreen;
    ViewAppointmentsScreen  viewAppts;
    ViewBillsScreen         viewBills;
    ViewRecordsScreen       viewRecords;
    PayBillScreen           payBill;
    TopUpScreen             topUp;
    DoctorMenuScreen        doctorMenu;
    DoctorTodayScreen       doctorToday;
    MarkCompleteScreen      markComplete;
    MarkNoShowScreen        markNoShow;
    WritePrescriptionScreen writeRx;
    PatientHistoryScreen    patientHistory;
    AdminMenuScreen         adminMenu;
    AddPatientScreen        addPatient;
    AddDoctorScreen         addDoctor;
    RemoveDoctorScreen      removeDoctor;
    AdminPatientsScreen     adminPatients;
    AdminDoctorsScreen      adminDoctors;
    AdminAllApptsScreen     adminAllAppts;
    AdminUnpaidBillsScreen  adminUnpaid;
    DischargePatientScreen  discharge;
    SecurityLogScreen       securityLog;
    DailyReportScreen       dailyReport;

    // ── Initialise all screens 
    roleScreen.init(font);     loginScreen.init(font);
    patientMenu.init(font);    bookScreen.init(font);
    cancelScreen.init(font);   viewAppts.init(font);
    viewBills.init(font);      viewRecords.init(font);
    payBill.init(font);        topUp.init(font);
    doctorMenu.init(font);     doctorToday.init(font);
    markComplete.init(font);   markNoShow.init(font);
    writeRx.init(font);        patientHistory.init(font);
    adminMenu.init(font);      addPatient.init(font);
    addDoctor.init(font);      removeDoctor.init(font);
    adminPatients.init(font);  adminDoctors.init(font);
    adminAllAppts.init(font);  adminUnpaid.init(font);
    discharge.init(font);      securityLog.init(font);
    dailyReport.init(font);

    // ── Runtime state
    Screen   current = Screen::RoleSelect;
    Screen   prev = Screen::Exit;
    int      role = 0;
    Patient* lp = nullptr;
    Doctor* ld = nullptr;

    while (window.isOpen()) {

        // ── Screen transition side-effects
        if (current != prev) {
            if (current == Screen::PatientMenu && lp)
                patientMenu.setSubtitle(lp->getName(), lp->getBalance());
            if (current == Screen::DoctorMenu && ld)
                doctorMenu.setSubtitle(ld->getName(), ld->getSpecialization());
            if (current == Screen::ViewAppointments && lp)
                viewAppts.refresh(sys, lp->getId());
            if (current == Screen::ViewBills && lp)
                viewBills.refresh(sys, lp->getId());
            if (current == Screen::ViewRecords && lp)
                viewRecords.refresh(sys, lp->getId());
            if (current == Screen::PayBill && lp)
                payBill.refresh(sys, lp->getId());
            if (current == Screen::CancelAppointment && lp)
                cancelScreen.refresh(sys, lp->getId());
            if (current == Screen::DoctorToday && ld)
                doctorToday.refresh(sys, ld->getId());
            if (current == Screen::MarkComplete && ld)
                markComplete.refresh(sys, ld->getId());
            if (current == Screen::MarkNoShow && ld)
                markNoShow.refresh(sys, ld->getId());
            if (current == Screen::AdminAllAppts)
                adminAllAppts.refresh(sys);
            if (current == Screen::AdminUnpaidBills)
                adminUnpaid.refresh(sys);
            if (current == Screen::RoleSelect) {
                lp = nullptr;
                ld = nullptr;
            }
            prev = current;
        }

        // ── Event handling
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (current == Screen::Exit)
                window.close();

            switch (current) {
            case Screen::RoleSelect:
                roleScreen.handle(event, current, audio, role);
                break;
            case Screen::Login:
                loginScreen.handle(event, current, audio, sys, role, lp, ld);
                break;

                // Patient screens
            case Screen::PatientMenu:
                patientMenu.handle(event, current, audio);
                break;
            case Screen::BookAppointment:
                bookScreen.handle(event, current, audio, sys, lp);
                break;
            case Screen::CancelAppointment:
                cancelScreen.handle(event, current, audio, sys, lp);
                break;
            case Screen::ViewAppointments:
                viewAppts.handle(event, current, audio);
                break;
            case Screen::ViewBills:
                viewBills.handle(event, current, audio);
                break;
            case Screen::ViewRecords:
                viewRecords.handle(event, current, audio);
                break;
            case Screen::PayBill:
                payBill.handle(event, current, audio, sys, lp);
                break;
            case Screen::TopUp:
                topUp.handle(event, current, audio, sys, lp);
                break;

                // Doctor screens
            case Screen::DoctorMenu:
                doctorMenu.handle(event, current, audio);
                break;
            case Screen::DoctorToday:
                doctorToday.handle(event, current, audio);
                break;
            case Screen::MarkComplete:
                markComplete.handle(event, current, audio, sys, ld);
                break;
            case Screen::MarkNoShow:
                markNoShow.handle(event, current, audio, sys, ld);
                break;
            case Screen::WritePrescription:
                writeRx.handle(event, current, audio, sys, ld);
                break;
            case Screen::PatientHistory:
                patientHistory.handle(event, current, audio, sys, ld);
                break;

                // Admin screens
            case Screen::AdminMenu:
                adminMenu.handle(event, current, audio);
                break;
            case Screen::AddPatient:
                addPatient.handle(event, current, audio, sys);
                break;
            case Screen::AddDoctor:
                addDoctor.handle(event, current, audio, sys);
                break;
            case Screen::RemoveDoctor:
                removeDoctor.handle(event, current, audio, sys);
                break;
            case Screen::AdminPatients:
                adminPatients.handle(event, current, audio);
                break;
            case Screen::AdminDoctors:
                adminDoctors.handle(event, current, audio);
                break;
            case Screen::AdminAllAppts:
                adminAllAppts.handle(event, current, audio);
                break;
            case Screen::AdminUnpaidBills:
                adminUnpaid.handle(event, current, audio);
                break;
            case Screen::DischargePatient:
                discharge.handle(event, current, audio, sys);
                break;
            case Screen::SecurityLog:
                securityLog.handle(event, current, audio);
                break;
            case Screen::DailyReport:
                dailyReport.handle(event, current, audio);
                break;

            default:
                break;
            }
        }

        // ── Drawing 
        window.clear();
        switch (current) {
        case Screen::RoleSelect:        roleScreen.draw(window, font);              break;
        case Screen::Login:             loginScreen.draw(window, font, role);       break;
        case Screen::PatientMenu:       patientMenu.draw(window, font);             break;
        case Screen::BookAppointment:   bookScreen.draw(window, font);              break;
        case Screen::CancelAppointment: cancelScreen.draw(window, font, sys);       break;
        case Screen::ViewAppointments:  viewAppts.draw(window, font, sys);          break;
        case Screen::ViewBills:         viewBills.draw(window, font);               break;
        case Screen::ViewRecords:       viewRecords.draw(window, font, sys);        break;
        case Screen::PayBill:           payBill.draw(window, font);                 break;
        case Screen::TopUp:             topUp.draw(window, font);                   break;
        case Screen::DoctorMenu:        doctorMenu.draw(window, font);              break;
        case Screen::DoctorToday:       doctorToday.draw(window, font, sys);        break;
        case Screen::MarkComplete:      markComplete.draw(window, font, sys);       break;
        case Screen::MarkNoShow:        markNoShow.draw(window, font, sys);         break;
        case Screen::WritePrescription: writeRx.draw(window, font);                 break;
        case Screen::PatientHistory:    patientHistory.draw(window, font);          break;
        case Screen::AdminMenu:         adminMenu.draw(window, font);               break;
        case Screen::AddPatient:        addPatient.draw(window, font);              break;
        case Screen::AddDoctor:         addDoctor.draw(window, font);               break;
        case Screen::RemoveDoctor:      removeDoctor.draw(window, font, sys);       break;
        case Screen::AdminPatients:     adminPatients.draw(window, font, sys);      break;
        case Screen::AdminDoctors:      adminDoctors.draw(window, font, sys);       break;
        case Screen::AdminAllAppts:     adminAllAppts.draw(window, font, sys);      break;
        case Screen::AdminUnpaidBills:  adminUnpaid.draw(window, font, sys);        break;
        case Screen::DischargePatient:  discharge.draw(window, font, sys);          break;
        case Screen::SecurityLog:       securityLog.draw(window, font);             break;
        case Screen::DailyReport:       dailyReport.draw(window, font, sys);        break;
        default:                                                                     break;
        }
        window.display();
    }

    return 0;
}