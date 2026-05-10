#include "Screens.h"
#include <cstdio>

// ============================================================
//  Audio
// ============================================================
void Audio::load() {
    if (clickBuf.loadFromFile("click.wav"))     click.setBuffer(clickBuf);
    if (successBuf.loadFromFile("success.wav")) success.setBuffer(successBuf);
    if (errorBuf.loadFromFile("error.wav"))     error.setBuffer(errorBuf);
    if (bgMusic.openFromFile("bg_music.ogg")) {
        bgMusic.setLoop(true); bgMusic.setVolume(20); bgMusic.play();
    }
}
void Audio::playClick() { click.play(); }
void Audio::playSuccess() { success.play(); }
void Audio::playError() { error.play(); }

// ============================================================
//  RoleSelectScreen
// ============================================================
void RoleSelectScreen::init(sf::Font& f) {
    btnPatient = Button(330, 160, 240, 48, "1.  Patient", f);
    btnDoctor = Button(330, 225, 240, 48, "2.  Doctor", f);
    btnAdmin = Button(330, 290, 240, 48, "3.  Admin", f);
    btnExit = Button(330, 355, 240, 48, "4.  Exit", f);
}
void RoleSelectScreen::handle(sf::Event& e, Screen& cur, Audio& audio, int& role) {
    if (btnPatient.isClicked(e)) { audio.playClick(); role = 1; cur = Screen::Login; }
    if (btnDoctor.isClicked(e)) { audio.playClick(); role = 2; cur = Screen::Login; }
    if (btnAdmin.isClicked(e)) { audio.playClick(); role = 3; cur = Screen::Login; }
    if (btnExit.isClicked(e)) { cur = Screen::Exit; }
}
void RoleSelectScreen::draw(sf::RenderWindow& win, sf::Font& f) {
    drawBackground(win);
    drawHeader(win, f, "MediCore Hospital Management System", "Select your role to continue");
    btnPatient.draw(win); btnDoctor.draw(win); btnAdmin.draw(win); btnExit.draw(win);
}

// ============================================================
//  LoginScreen
// ============================================================
LoginScreen::LoginScreen() : failCount(0), locked(false) { msg[0] = '\0'; }
void LoginScreen::init(sf::Font& f) {
    tbId = TextBox(330, 220, 240, 36, f, 10);
    tbPw = TextBox(330, 300, 240, 36, f, 20, true);
    tbId.setLabel("ID", f); tbPw.setLabel("Password", f);
    btnLogin = Button(330, 360, 240, 44, "Login", f);
    btnBack = Button(330, 420, 240, 36, "Back", f, 13);
}
void LoginScreen::reset() {
    tbId.clear(); tbPw.clear(); failCount = 0; locked = false; msg[0] = '\0';
}
void LoginScreen::checkLock(int role, int id) {
    char rs[10], is[10];
    strCopy(rs, role == 1 ? "Patient" : role == 2 ? "Doctor" : "Admin", 10);
    intToStr(id, is);
    if (failCount >= 3) {
        locked = true; strCopy(msg, "Account locked. Contact admin.", 200);
        FileHandler::logSecurityEvent(rs, is, "LOCKED");
    }
    else {
        char buf[100]; strCopy(buf, "Wrong credentials. Attempt ", 100);
        char n[4]; intToStr(failCount, n);
        strConcat(buf, n, 100); strConcat(buf, " of 3.", 100);
        strCopy(msg, buf, 200); FileHandler::logSecurityEvent(rs, is, "FAILED");
    }
}
void LoginScreen::handle(sf::Event& e, Screen& cur, Audio& audio,
    HospitalSystem& sys, int role, Patient*& lp, Doctor*& ld) {
    tbId.handleEvent(e); tbPw.handleEvent(e);
    if (btnBack.isClicked(e)) { audio.playClick(); reset(); cur = Screen::RoleSelect; return; }
    if (btnLogin.isClicked(e)) {
        if (locked) { audio.playError(); return; }
        audio.playClick(); int id = strToInt(tbId.getText());
        if (role == 1) {
            lp = sys.loginPatient(id, tbPw.getText());
            if (lp) { audio.playSuccess(); reset(); cur = Screen::PatientMenu; }
            else { failCount++; audio.playError(); checkLock(role, id); }
        }
        else if (role == 2) {
            ld = sys.loginDoctor(id, tbPw.getText());
            if (ld) { audio.playSuccess(); reset(); cur = Screen::DoctorMenu; }
            else { failCount++; audio.playError(); checkLock(role, id); }
        }
        else {
            if (sys.loginAdmin(id, tbPw.getText())) { audio.playSuccess(); reset(); cur = Screen::AdminMenu; }
            else { failCount++; audio.playError(); checkLock(role, id); }
        }
    }
}
void LoginScreen::draw(sf::RenderWindow& win, sf::Font& f, int role) {
    drawBackground(win);
    const char* sub = role == 1 ? "Patient Login" : role == 2 ? "Doctor Login" : "Admin Login";
    drawHeader(win, f, "MediCore — Login", sub);
    tbId.draw(win); tbPw.draw(win); btnLogin.draw(win); btnBack.draw(win);
    if (msg[0]) drawMessage(win, f, msg, true);
}

// ============================================================
//  PatientMenuScreen
// ============================================================
void PatientMenuScreen::init(sf::Font& f) {
    const char* lbl[] = { "1. Book Appointment","2. Cancel Appointment",
                        "3. View Appointments","4. Medical Records",
                        "5. View Bills","6. Pay Bill",
                        "7. Top Up Balance","8. Logout" };
    for (int i = 0; i < 8; i++)
        btns[i] = Button((i % 2 == 0) ? 80 : 490, 110 + (i / 2) * 70, 330, 48, lbl[i], f, 14);
}
void PatientMenuScreen::setSubtitle(const char* name, float bal) {
    char bb[20]; floatToStr(bal, bb);
    strCopy(subtitle, name, 150); strConcat(subtitle, "  |  Balance: PKR ", 150);
    strConcat(subtitle, bb, 150);
}
void PatientMenuScreen::handle(sf::Event& e, Screen& cur, Audio& audio) {
    // FIX: each button maps to its own screen
    Screen t[] = {
        Screen::BookAppointment,
        Screen::CancelAppointment,   // FIX: was missing
        Screen::ViewAppointments,
        Screen::ViewRecords,          // FIX: was missing
        Screen::ViewBills,
        Screen::PayBill,
        Screen::TopUp,
        Screen::RoleSelect
    };
    for (int i = 0; i < 8; i++)
        if (btns[i].isClicked(e)) { audio.playClick(); cur = t[i]; return; }
}
void PatientMenuScreen::draw(sf::RenderWindow& win, sf::Font& f) {
    drawBackground(win);
    drawHeader(win, f, "Patient Menu", subtitle);
    for (int i = 0; i < 8; i++) btns[i].draw(win);
}

// ============================================================
//  CancelAppointmentScreen  ← NEW PROPER IMPLEMENTATION
// ============================================================
CancelAppointmentScreen::CancelAppointmentScreen() :msgIsError(false), count(0) { msg[0] = '\0'; }
void CancelAppointmentScreen::init(sf::Font& f) {
    tbApptId = TextBox(30, 360, 160, 34, f, 10);
    tbApptId.setLabel("Appointment ID to cancel", f);
    btnCancel = Button(210, 360, 160, 34, "Cancel Appointment", f, 12);
    btnBack = Button(30, 560, 100, 34, "Back", f, 13);
}
void CancelAppointmentScreen::refresh(HospitalSystem& sys, int pid) {
    count = 0; msg[0] = '\0'; tbApptId.clear();
    // only pending appointments
    Appointment tmp[50]; int n = sys.getAppointmentsForPatient(pid, tmp, 50);
    for (int i = 0; i < n; i++)
        if (strCmp(tmp[i].getStatus(), "pending") == 0) appts[count++] = tmp[i];
}
void CancelAppointmentScreen::handle(sf::Event& e, Screen& cur, Audio& audio,
    HospitalSystem& sys, Patient* p) {
    tbApptId.handleEvent(e);
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::PatientMenu; return; }
    if (btnCancel.isClicked(e)) {
        audio.playClick();
        char out[200];
        bool ok = sys.cancelAppointment(p->getId(), strToInt(tbApptId.getText()), out);
        strCopy(msg, out, 200); msgIsError = !ok;
        if (ok) { audio.playSuccess(); refresh(sys, p->getId()); }
        else      audio.playError();
    }
}
void CancelAppointmentScreen::draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys) {
    drawBackground(win);
    drawHeader(win, f, "Cancel Appointment", "Your pending appointments");
    if (count == 0) {
        win.draw(makeText("You have no pending appointments.", f, 14, Color::SUBTEXT, 30, 110));
    }
    else {
        const char* hdr[] = { "ID","Doctor","Date","Slot" };
        float xs[] = { 30,90,260,390 };
        drawTableHeader(win, f, hdr, xs, 4, 90);
        for (int i = 0; i < count && i < 12; i++) {
            char id[8]; intToStr(appts[i].getId(), id);
            Doctor* d = sys.findDoctor(appts[i].getDoctorId());
            const char* v[] = { id,d ? d->getName() : "Unknown",appts[i].getDate(),appts[i].getTimeSlot() };
            drawTableRow(win, f, v, xs, 4, 120 + i * 27, i % 2);
        }
    }
    tbApptId.draw(win); btnCancel.draw(win); btnBack.draw(win);
    drawMessage(win, f, msg, msgIsError);
}

// ============================================================
//  ViewRecordsScreen  ← NEW PROPER IMPLEMENTATION
// ============================================================
ViewRecordsScreen::ViewRecordsScreen() :count(0) {}
void ViewRecordsScreen::init(sf::Font& f) { btnBack = Button(30, 560, 100, 34, "Back", f, 13); }
void ViewRecordsScreen::refresh(HospitalSystem& sys, int pid) {
    count = sys.getPrescriptionsForPatient(pid, prescriptions, 50);
}
void ViewRecordsScreen::handle(sf::Event& e, Screen& cur, Audio& audio) {
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::PatientMenu; }
}
void ViewRecordsScreen::draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys) {
    drawBackground(win);
    drawHeader(win, f, "Medical Records", "Most recent first");
    if (count == 0) {
        win.draw(makeText("No medical records found.", f, 14, Color::SUBTEXT, 30, 110));
    }
    else {
        const char* hdr[] = { "Date","Doctor","Medicines","Notes" };
        float xs[] = { 30,130,280,560 };
        drawTableHeader(win, f, hdr, xs, 4, 90);
        for (int i = 0; i < count && i < 12; i++) {
            Doctor* d = sys.findDoctor(prescriptions[i].getDoctorId());
            const char* v[] = {
                prescriptions[i].getDate(),
                d ? d->getName() : "Unknown",
                prescriptions[i].getMedicines(),
                prescriptions[i].getNotes()
            };
            drawTableRow(win, f, v, xs, 4, 120 + i * 27, i % 2);
        }
    }
    btnBack.draw(win);
}

// ============================================================
//  BookAppointmentScreen
// ============================================================
BookAppointmentScreen::BookAppointmentScreen() :msgIsError(false), foundCount(0), slotCount(0) { msg[0] = '\0'; }
void BookAppointmentScreen::init(sf::Font& f) {
    tbSpec = TextBox(30, 110, 200, 34, f, 50);
    tbDocId = TextBox(250, 110, 120, 34, f, 10);
    tbDate = TextBox(390, 110, 150, 34, f, 11);
    tbSlot = TextBox(560, 110, 100, 34, f, 6);
    tbSpec.setLabel("Specialization", f); tbDocId.setLabel("Doctor ID", f);
    tbDate.setLabel("Date DD-MM-YYYY", f); tbSlot.setLabel("Slot e.g.09:00", f);
    btnSearch = Button(675, 110, 100, 34, "Search", f, 13);
    btnBook = Button(675, 155, 100, 34, "Book", f, 13);
    btnBack = Button(30, 560, 100, 34, "Back", f, 13);
}
void BookAppointmentScreen::reset() {
    tbSpec.clear(); tbDocId.clear(); tbDate.clear(); tbSlot.clear();
    msg[0] = '\0'; foundCount = 0; slotCount = 0;
}
void BookAppointmentScreen::refreshSlots(HospitalSystem& sys) {
    int dId = strToInt(tbDocId.getText());
    if (dId > 0 && tbDate.getText()[0])
        slotCount = sys.getAvailableSlots(dId, tbDate.getText(), availSlots, 8);
}
void BookAppointmentScreen::handle(sf::Event& e, Screen& cur, Audio& audio,
    HospitalSystem& sys, Patient* p) {
    tbSpec.handleEvent(e); tbDocId.handleEvent(e); tbDate.handleEvent(e); tbSlot.handleEvent(e);
    if (btnBack.isClicked(e)) { audio.playClick(); reset(); cur = Screen::PatientMenu; return; }
    if (btnSearch.isClicked(e)) {
        audio.playClick();
        foundCount = sys.getDoctorsBySpec(tbSpec.getText(), foundDocs, 20);
        if (foundCount == 0) { strCopy(msg, "No doctors found.", 200); msgIsError = true; }
        else { msg[0] = '\0'; refreshSlots(sys); }
    }
    if (btnBook.isClicked(e)) {
        audio.playClick(); char out[200];
        bool ok = sys.bookAppointment(p->getId(), strToInt(tbDocId.getText()),
            tbDate.getText(), tbSlot.getText(), out);
        strCopy(msg, out, 200); msgIsError = !ok;
        if (ok) { audio.playSuccess(); refreshSlots(sys); }
        else audio.playError();
    }
}
void BookAppointmentScreen::draw(sf::RenderWindow& win, sf::Font& f) {
    drawBackground(win);
    drawHeader(win, f, "Book Appointment", "Search doctor then fill details");
    tbSpec.draw(win); tbDocId.draw(win); tbDate.draw(win); tbSlot.draw(win);
    btnSearch.draw(win); btnBook.draw(win); btnBack.draw(win);
    if (foundCount > 0) {
        const char* hdr[] = { "ID","Name","Specialization","Fee(PKR)" };
        float xs[] = { 30,90,260,500 };
        drawTableHeader(win, f, hdr, xs, 4, 165);
        for (int i = 0; i < foundCount && i < 6; i++) {
            char id[8], fee[12];
            intToStr(foundDocs[i].getId(), id); floatToStr(foundDocs[i].getFee(), fee);
            const char* v[] = { id,foundDocs[i].getName(),foundDocs[i].getSpecialization(),fee };
            drawTableRow(win, f, v, xs, 4, 195 + i * 28, i % 2);
        }
    }
    if (slotCount > 0) {
        win.draw(makeText("Available:", f, 13, Color::SUBTEXT, 30, 380));
        char line[100]; line[0] = '\0';
        for (int i = 0; i < slotCount; i++) { strConcat(line, availSlots[i], 100); strConcat(line, "  ", 100); }
        win.draw(makeText(line, f, 13, Color::ACCENT, 130, 380));
    }
    drawMessage(win, f, msg, msgIsError);
}

// ============================================================
//  ViewAppointmentsScreen
// ============================================================
ViewAppointmentsScreen::ViewAppointmentsScreen() :count(0) {}
void ViewAppointmentsScreen::init(sf::Font& f) { btnBack = Button(30, 560, 100, 34, "Back", f, 13); }
void ViewAppointmentsScreen::refresh(HospitalSystem& sys, int pid) { count = sys.getAppointmentsForPatient(pid, appts, 50); }
void ViewAppointmentsScreen::handle(sf::Event& e, Screen& cur, Audio& audio) {
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::PatientMenu; }
}
void ViewAppointmentsScreen::draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys) {
    drawBackground(win); drawHeader(win, f, "My Appointments", "Sorted by date");
    if (count == 0) { win.draw(makeText("No appointments found.", f, 14, Color::SUBTEXT, 30, 110)); }
    else {
        const char* hdr[] = { "ID","Doctor","Date","Slot","Status" };
        float xs[] = { 30,90,260,400,490 };
        drawTableHeader(win, f, hdr, xs, 5, 90);
        for (int i = 0; i < count && i < 15; i++) {
            char id[8]; intToStr(appts[i].getId(), id);
            Doctor* d = sys.findDoctor(appts[i].getDoctorId());
            const char* v[] = { id,d ? d->getName() : "Unknown",appts[i].getDate(),appts[i].getTimeSlot(),appts[i].getStatus() };
            drawTableRow(win, f, v, xs, 5, 120 + i * 27, i % 2);
        }
    }
    btnBack.draw(win);
}

// ============================================================
//  ViewBillsScreen
// ============================================================
ViewBillsScreen::ViewBillsScreen() :count(0) {}
void ViewBillsScreen::init(sf::Font& f) { btnBack = Button(30, 560, 100, 34, "Back", f, 13); }
void ViewBillsScreen::refresh(HospitalSystem& sys, int pid) { count = sys.getBillsForPatient(pid, billArr, 50); }
void ViewBillsScreen::handle(sf::Event& e, Screen& cur, Audio& audio) {
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::PatientMenu; }
}
void ViewBillsScreen::draw(sf::RenderWindow& win, sf::Font& f) {
    drawBackground(win); drawHeader(win, f, "My Bills", "");
    if (count == 0) { win.draw(makeText("No bills found.", f, 14, Color::SUBTEXT, 30, 110)); }
    else {
        const char* hdr[] = { "Bill ID","Appt ID","Amount","Status","Date" };
        float xs[] = { 30,120,210,340,460 };
        drawTableHeader(win, f, hdr, xs, 5, 90);
        float total = 0;
        for (int i = 0; i < count && i < 15; i++) {
            char bId[8], aId[8], amt[15];
            intToStr(billArr[i].getId(), bId); intToStr(billArr[i].getAppointmentId(), aId);
            floatToStr(billArr[i].getAmount(), amt);
            if (strCmp(billArr[i].getStatus(), "unpaid") == 0) total += billArr[i].getAmount();
            const char* v[] = { bId,aId,amt,billArr[i].getStatus(),billArr[i].getDate() };
            drawTableRow(win, f, v, xs, 5, 120 + i * 27, i % 2);
        }
        char tl[60]; strCopy(tl, "Outstanding: PKR ", 60);
        char tb[20]; floatToStr(total, tb); strConcat(tl, tb, 60);
        win.draw(makeText(tl, f, 13, Color::ACCENT, 30, 530));
    }
    btnBack.draw(win);
}

// ============================================================
//  PayBillScreen
// ============================================================
PayBillScreen::PayBillScreen() :msgIsError(false), unpaidCount(0) { msg[0] = '\0'; }
void PayBillScreen::init(sf::Font& f) {
    tbBillId = TextBox(30, 310, 140, 34, f, 10);
    tbBillId.setLabel("Bill ID to pay", f);
    btnPay = Button(190, 310, 100, 34, "Pay", f, 13);
    btnBack = Button(30, 560, 100, 34, "Back", f, 13);
}
void PayBillScreen::refresh(HospitalSystem& sys, int pid) {
    // Only store unpaid bills for this patient
    Bill tmp[50]; int total = sys.getBillsForPatient(pid, tmp, 50);
    unpaidCount = 0;
    for (int i = 0; i < total; i++)
        if (strCmp(tmp[i].getStatus(), "unpaid") == 0) unpaid[unpaidCount++] = tmp[i];
    msg[0] = '\0'; tbBillId.clear();
}
void PayBillScreen::handle(sf::Event& e, Screen& cur, Audio& audio, HospitalSystem& sys, Patient* p) {
    tbBillId.handleEvent(e);
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::PatientMenu; return; }
    if (btnPay.isClicked(e)) {
        audio.playClick(); char out[200];
        bool ok = sys.payBill(p->getId(), strToInt(tbBillId.getText()), out);
        strCopy(msg, out, 200); msgIsError = !ok;
        if (ok) { audio.playSuccess(); refresh(sys, p->getId()); }
        else audio.playError();
    }
}
void PayBillScreen::draw(sf::RenderWindow& win, sf::Font& f) {
    drawBackground(win); drawHeader(win, f, "Pay Bill", "Unpaid bills shown below");
    const char* hdr[] = { "Bill ID","Amount (PKR)","Date" };
    float xs[] = { 30,150,310 };
    drawTableHeader(win, f, hdr, xs, 3, 90);
    int shown = 0;
    for (int i = 0; i < unpaidCount && shown < 10; i++) {
        if (strCmp(unpaid[i].getStatus(), "unpaid") == 0) {
            char bId[8], amt[15];
            intToStr(unpaid[i].getId(), bId); floatToStr(unpaid[i].getAmount(), amt);
            const char* v[] = { bId,amt,unpaid[i].getDate() };
            drawTableRow(win, f, v, xs, 3, 120 + shown * 27, shown % 2); shown++;
        }
    }
    if (shown == 0) win.draw(makeText("No unpaid bills.", f, 14, Color::SUBTEXT, 30, 120));
    tbBillId.draw(win); btnPay.draw(win); btnBack.draw(win);
    drawMessage(win, f, msg, msgIsError);
}

// ============================================================
//  TopUpScreen
// ============================================================
TopUpScreen::TopUpScreen() :msgIsError(false) { msg[0] = '\0'; }
void TopUpScreen::init(sf::Font& f) {
    tbAmount = TextBox(330, 250, 240, 36, f, 10);
    tbAmount.setLabel("Amount (PKR)", f);
    btnTopUp = Button(330, 310, 240, 44, "Add to Balance", f);
    btnBack = Button(330, 370, 240, 36, "Back", f, 13);
}
void TopUpScreen::handle(sf::Event& e, Screen& cur, Audio& audio, HospitalSystem& sys, Patient* p) {
    tbAmount.handleEvent(e);
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::PatientMenu; return; }
    if (btnTopUp.isClicked(e)) {
        audio.playClick(); char out[200];
        bool ok = sys.topUpBalance(p->getId(), strToFloat(tbAmount.getText()), out);
        strCopy(msg, out, 200); msgIsError = !ok;
        if (ok) { audio.playSuccess(); tbAmount.clear(); }
        else audio.playError();
    }
}
void TopUpScreen::draw(sf::RenderWindow& win, sf::Font& f) {
    drawBackground(win); drawHeader(win, f, "Top Up Balance", "Add funds to your account");
    tbAmount.draw(win); btnTopUp.draw(win); btnBack.draw(win);
    drawMessage(win, f, msg, msgIsError);
}

// ============================================================
//  DoctorMenuScreen  — FIX: each button goes to correct screen
// ============================================================
void DoctorMenuScreen::init(sf::Font& f) {
    const char* lbl[] = {
        "1. Today's Appointments","2. Mark Complete",
        "3. Mark No-Show",        "4. Write Prescription",
        "5. Patient History",     "6. Logout"
    };
    for (int i = 0; i < 6; i++)
        btns[i] = Button((i % 2 == 0) ? 80 : 490, 130 + (i / 2) * 80, 330, 50, lbl[i], f, 14);
}
void DoctorMenuScreen::setSubtitle(const char* name, const char* spec) {
    strCopy(subtitle, "Dr. ", 150); strConcat(subtitle, name, 150);
    strConcat(subtitle, "  |  ", 150); strConcat(subtitle, spec, 150);
}
void DoctorMenuScreen::handle(sf::Event& e, Screen& cur, Audio& audio) {
    // FIX: proper mapping — each option goes to its own screen
    if (btns[0].isClicked(e)) { audio.playClick(); cur = Screen::DoctorToday;          return; }
    if (btns[1].isClicked(e)) { audio.playClick(); cur = Screen::MarkComplete;          return; }
    if (btns[2].isClicked(e)) { audio.playClick(); cur = Screen::MarkNoShow;            return; }
    if (btns[3].isClicked(e)) { audio.playClick(); cur = Screen::WritePrescription;     return; }
    if (btns[4].isClicked(e)) { audio.playClick(); cur = Screen::PatientHistory;        return; }
    if (btns[5].isClicked(e)) { audio.playClick(); cur = Screen::RoleSelect;            return; }
}
void DoctorMenuScreen::draw(sf::RenderWindow& win, sf::Font& f) {
    drawBackground(win); drawHeader(win, f, "Doctor Menu", subtitle);
    for (int i = 0; i < 6; i++) btns[i].draw(win);
}

// ============================================================
//  DoctorTodayScreen
// ============================================================
DoctorTodayScreen::DoctorTodayScreen() : count(0) {}
void DoctorTodayScreen::init(sf::Font& f) {
    btnBack = Button(30, 560, 100, 34, "Back", f, 13);
}
void DoctorTodayScreen::refresh(HospitalSystem& sys, int did) {
    count = sys.getTodayAppointmentsForDoctor(did, appts, 20);
}
void DoctorTodayScreen::handle(sf::Event& e, Screen& cur, Audio& audio) {
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::DoctorMenu; }
}
void DoctorTodayScreen::draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys) {
    drawBackground(win); drawHeader(win, f, "Today's Appointments", "");
    if (count == 0) { win.draw(makeText("No appointments today.", f, 14, Color::SUBTEXT, 30, 100)); }
    else {
        const char* hdr[] = { "ID","Patient","Slot","Status" };
        float xs[] = { 30,90,280,390 };
        drawTableHeader(win, f, hdr, xs, 4, 90);
        for (int i = 0; i < count; i++) {
            char id[8]; intToStr(appts[i].getId(), id);
            Patient* p = sys.findPatient(appts[i].getPatientId());
            const char* v[] = { id,p ? p->getName() : "Unknown",appts[i].getTimeSlot(),appts[i].getStatus() };
            drawTableRow(win, f, v, xs, 4, 120 + i * 27, i % 2);
        }
    }
    btnBack.draw(win);
}

// ============================================================
//  MarkCompleteScreen  ← NEW PROPER SCREEN
// ============================================================
MarkCompleteScreen::MarkCompleteScreen() :msgIsError(false), count(0) { msg[0] = '\0'; }
void MarkCompleteScreen::init(sf::Font& f) {
    tbApptId = TextBox(30, 390, 160, 34, f, 10);
    tbApptId.setLabel("Appointment ID", f);
    btnMark = Button(210, 390, 160, 34, "Mark Complete", f, 12);
    btnBack = Button(30, 560, 100, 34, "Back", f, 13);
}
void MarkCompleteScreen::refresh(HospitalSystem& sys, int did) {
    count = sys.getTodayAppointmentsForDoctor(did, appts, 20); msg[0] = '\0'; tbApptId.clear();
}
void MarkCompleteScreen::handle(sf::Event& e, Screen& cur, Audio& audio,
    HospitalSystem& sys, Doctor* d) {
    tbApptId.handleEvent(e);
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::DoctorMenu; return; }
    if (btnMark.isClicked(e)) {
        audio.playClick(); char out[200];
        bool ok = sys.markComplete(d->getId(), strToInt(tbApptId.getText()), out);
        strCopy(msg, out, 200); msgIsError = !ok;
        if (ok) { audio.playSuccess(); refresh(sys, d->getId()); }
        else audio.playError();
    }
}
void MarkCompleteScreen::draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys) {
    drawBackground(win); drawHeader(win, f, "Mark Appointment Complete", "Today's pending appointments");
    if (count == 0) { win.draw(makeText("No appointments today.", f, 14, Color::SUBTEXT, 30, 100)); }
    else {
        const char* hdr[] = { "ID","Patient","Slot","Status" };
        float xs[] = { 30,90,280,390 };
        drawTableHeader(win, f, hdr, xs, 4, 90);
        for (int i = 0; i < count; i++) {
            char id[8]; intToStr(appts[i].getId(), id);
            Patient* p = sys.findPatient(appts[i].getPatientId());
            const char* v[] = { id,p ? p->getName() : "Unknown",appts[i].getTimeSlot(),appts[i].getStatus() };
            drawTableRow(win, f, v, xs, 4, 120 + i * 27, i % 2);
        }
    }
    tbApptId.draw(win); btnMark.draw(win); btnBack.draw(win);
    drawMessage(win, f, msg, msgIsError);
}

// ============================================================
//  MarkNoShowScreen  ← NEW PROPER SCREEN
// ============================================================
MarkNoShowScreen::MarkNoShowScreen() :msgIsError(false), count(0) { msg[0] = '\0'; }
void MarkNoShowScreen::init(sf::Font& f) {
    tbApptId = TextBox(30, 390, 160, 34, f, 10);
    tbApptId.setLabel("Appointment ID", f);
    btnMark = Button(210, 390, 160, 34, "Mark No-Show", f, 12);
    btnBack = Button(30, 560, 100, 34, "Back", f, 13);
}
void MarkNoShowScreen::refresh(HospitalSystem& sys, int did) {
    count = sys.getTodayAppointmentsForDoctor(did, appts, 20); msg[0] = '\0'; tbApptId.clear();
}
void MarkNoShowScreen::handle(sf::Event& e, Screen& cur, Audio& audio,
    HospitalSystem& sys, Doctor* d) {
    tbApptId.handleEvent(e);
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::DoctorMenu; return; }
    if (btnMark.isClicked(e)) {
        audio.playClick(); char out[200];
        bool ok = sys.markNoShow(d->getId(), strToInt(tbApptId.getText()), out);
        strCopy(msg, out, 200); msgIsError = !ok;
        if (ok) { audio.playSuccess(); refresh(sys, d->getId()); }
        else audio.playError();
    }
}
void MarkNoShowScreen::draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys) {
    drawBackground(win); drawHeader(win, f, "Mark No-Show", "Today's pending appointments");
    if (count == 0) { win.draw(makeText("No appointments today.", f, 14, Color::SUBTEXT, 30, 100)); }
    else {
        const char* hdr[] = { "ID","Patient","Slot","Status" };
        float xs[] = { 30,90,280,390 };
        drawTableHeader(win, f, hdr, xs, 4, 90);
        for (int i = 0; i < count; i++) {
            char id[8]; intToStr(appts[i].getId(), id);
            Patient* p = sys.findPatient(appts[i].getPatientId());
            const char* v[] = { id,p ? p->getName() : "Unknown",appts[i].getTimeSlot(),appts[i].getStatus() };
            drawTableRow(win, f, v, xs, 4, 120 + i * 27, i % 2);
        }
    }
    tbApptId.draw(win); btnMark.draw(win); btnBack.draw(win);
    drawMessage(win, f, msg, msgIsError);
}

// ============================================================
//  PatientHistoryScreen  ← NEW PROPER SCREEN
// ============================================================
PatientHistoryScreen::PatientHistoryScreen() :msgIsError(false), count(0) { msg[0] = '\0'; }
void PatientHistoryScreen::init(sf::Font& f) {
    tbPatientId = TextBox(30, 130, 160, 34, f, 10);
    tbPatientId.setLabel("Patient ID", f);
    btnSearch = Button(210, 130, 100, 34, "Search", f, 13);
    btnBack = Button(30, 560, 100, 34, "Back", f, 13);
}
void PatientHistoryScreen::handle(sf::Event& e, Screen& cur, Audio& audio,
    HospitalSystem& sys, Doctor* d) {
    tbPatientId.handleEvent(e);
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::DoctorMenu; return; }
    if (btnSearch.isClicked(e)) {
        audio.playClick();
        int pid = strToInt(tbPatientId.getText());
        Patient* p = sys.findPatient(pid);
        if (!p) { strCopy(msg, "Patient not found.", 200); msgIsError = true; audio.playError(); return; }
        // check has at least one completed appointment with this doctor
        bool hasAppt = false;
        Storage<Appointment>& appts = sys.getAppointments();
        for (int i = 0; i < appts.size(); i++)
            if (appts.get(i).getPatientId() == pid &&
                appts.get(i).getDoctorId() == d->getId() &&
                strCmp(appts.get(i).getStatus(), "completed") == 0) {
                hasAppt = true; break;
            }
        if (!hasAppt) {
            strCopy(msg, "Access denied. No completed appointments with this patient.", 200);
            msgIsError = true; audio.playError(); return;
        }
        // load prescriptions written by this doctor for this patient
        count = 0; msg[0] = '\0';
        Storage<Prescription>& presc = sys.getPrescriptions();
        for (int i = 0; i < presc.size() && count < 50; i++)
            if (presc.get(i).getPatientId() == pid && presc.get(i).getDoctorId() == d->getId())
                prescriptions[count++] = presc.get(i);
        if (count == 0) { strCopy(msg, "No prescriptions written for this patient.", 200); msgIsError = false; }
        audio.playSuccess();
    }
}
void PatientHistoryScreen::draw(sf::RenderWindow& win, sf::Font& f) {
    drawBackground(win); drawHeader(win, f, "Patient Medical History", "Your patients only");
    tbPatientId.draw(win); btnSearch.draw(win); btnBack.draw(win);
    if (count > 0) {
        const char* hdr[] = { "Date","Medicines","Notes" };
        float xs[] = { 30,150,500 };
        drawTableHeader(win, f, hdr, xs, 3, 190);
        for (int i = 0; i < count && i < 12; i++) {
            const char* v[] = { prescriptions[i].getDate(),prescriptions[i].getMedicines(),prescriptions[i].getNotes() };
            drawTableRow(win, f, v, xs, 3, 220 + i * 27, i % 2);
        }
    }
    drawMessage(win, f, msg, msgIsError);
}

// ============================================================
//  WritePrescriptionScreen
// ============================================================
WritePrescriptionScreen::WritePrescriptionScreen() :msgIsError(false) { msg[0] = '\0'; }
void WritePrescriptionScreen::init(sf::Font& f) {
    tbApptId = TextBox(30, 130, 160, 34, f, 10);
    tbMeds = TextBox(30, 220, 840, 34, f, 499);
    tbNotes = TextBox(30, 310, 840, 34, f, 299);
    tbApptId.setLabel("Appointment ID (must be completed)", f);
    tbMeds.setLabel("Medicines e.g. Paracetamol 500mg;Amoxicillin 250mg", f);
    tbNotes.setLabel("Notes (max 300 chars)", f);
    btnSave = Button(30, 380, 140, 40, "Save", f);
    btnBack = Button(30, 560, 100, 34, "Back", f, 13);
}
void WritePrescriptionScreen::handle(sf::Event& e, Screen& cur, Audio& audio,
    HospitalSystem& sys, Doctor* d) {
    tbApptId.handleEvent(e); tbMeds.handleEvent(e); tbNotes.handleEvent(e);
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::DoctorMenu; return; }
    if (btnSave.isClicked(e)) {
        audio.playClick(); char out[200];
        bool ok = sys.writePrescription(d->getId(), strToInt(tbApptId.getText()),
            tbMeds.getText(), tbNotes.getText(), out);
        strCopy(msg, out, 200); msgIsError = !ok;
        if (ok) { audio.playSuccess(); tbApptId.clear(); tbMeds.clear(); tbNotes.clear(); }
        else audio.playError();
    }
}
void WritePrescriptionScreen::draw(sf::RenderWindow& win, sf::Font& f) {
    drawBackground(win); drawHeader(win, f, "Write Prescription", "For completed appointments only");
    tbApptId.draw(win); tbMeds.draw(win); tbNotes.draw(win);
    btnSave.draw(win); btnBack.draw(win); drawMessage(win, f, msg, msgIsError);
}

// ============================================================
//  AdminMenuScreen  — FIX: proper mapping for all 10 buttons
// ============================================================
void AdminMenuScreen::init(sf::Font& f) {
    const char* lbl[] = {
        "1. Add Patient",       "2. Add Doctor",
        "3. Remove Doctor",     "4. All Patients",
        "5. All Doctors",       "6. All Appointments",
        "7. Unpaid Bills",      "8. Discharge Patient",
        "9. Security Log",      "10. Daily Report",
        "11. Logout"
    };
    // 6 rows of 2 buttons, last row has 1 button
    for (int i = 0; i < 10; i++)
        btns[i] = Button((i % 2 == 0) ? 60 : 480, 90 + (i / 2) * 66, 340, 44, lbl[i], f, 13);
    btns[10] = Button(60, 90 + 5 * 66, 760, 44, lbl[10], f, 13);
}
void AdminMenuScreen::handle(sf::Event& e, Screen& cur, Audio& audio) {
    if (btns[0].isClicked(e)) { audio.playClick(); cur = Screen::AddPatient;       return; }
    if (btns[1].isClicked(e)) { audio.playClick(); cur = Screen::AddDoctor;        return; }
    if (btns[2].isClicked(e)) { audio.playClick(); cur = Screen::RemoveDoctor;     return; }
    if (btns[3].isClicked(e)) { audio.playClick(); cur = Screen::AdminPatients;    return; }
    if (btns[4].isClicked(e)) { audio.playClick(); cur = Screen::AdminDoctors;     return; }
    if (btns[5].isClicked(e)) { audio.playClick(); cur = Screen::AdminAllAppts;    return; }
    if (btns[6].isClicked(e)) { audio.playClick(); cur = Screen::AdminUnpaidBills; return; }
    if (btns[7].isClicked(e)) { audio.playClick(); cur = Screen::DischargePatient; return; }
    if (btns[8].isClicked(e)) { audio.playClick(); cur = Screen::SecurityLog;      return; }
    if (btns[9].isClicked(e)) { audio.playClick(); cur = Screen::DailyReport;      return; }
    if (btns[10].isClicked(e)) { audio.playClick(); cur = Screen::RoleSelect;       return; }
}
void AdminMenuScreen::draw(sf::RenderWindow& win, sf::Font& f) {
    drawBackground(win); drawHeader(win, f, "Admin Panel", "MediCore — Full Access");
    for (int i = 0; i < 11; i++) btns[i].draw(win);
}

// ============================================================
//  AddPatientScreen  ← NEW (patient registration)
// ============================================================
AddPatientScreen::AddPatientScreen() :msgIsError(false) { msg[0] = '\0'; }
void AddPatientScreen::init(sf::Font& f) {
    tbName = TextBox(30, 130, 200, 34, f, 100);
    tbAge = TextBox(250, 130, 80, 34, f, 3);
    tbGender = TextBox(350, 130, 80, 34, f, 2);
    tbContact = TextBox(450, 130, 160, 34, f, 12);
    tbPw = TextBox(30, 230, 200, 34, f, 20, true);
    tbBalance = TextBox(250, 230, 140, 34, f, 10);
    tbName.setLabel("Full Name", f);
    tbAge.setLabel("Age", f);
    tbGender.setLabel("M/F", f);
    tbContact.setLabel("Contact (11 digits)", f);
    tbPw.setLabel("Password (min 6)", f);
    tbBalance.setLabel("Initial Balance", f);
    btnAdd = Button(30, 310, 160, 40, "Register Patient", f, 13);
    btnBack = Button(30, 560, 100, 34, "Back", f, 13);
}
void AddPatientScreen::handle(sf::Event& e, Screen& cur, Audio& audio, HospitalSystem& sys) {
    tbName.handleEvent(e); tbAge.handleEvent(e); tbGender.handleEvent(e);
    tbContact.handleEvent(e); tbPw.handleEvent(e); tbBalance.handleEvent(e);
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::AdminMenu; return; }
    if (btnAdd.isClicked(e)) {
        audio.playClick();
        if (!Validator::isValidContact(tbContact.getText())) {
            strCopy(msg, "Invalid contact: must be 11 digits.", 200); msgIsError = true; audio.playError(); return;
        }
        if (!Validator::isValidPassword(tbPw.getText())) {
            strCopy(msg, "Password must be at least 6 chars.", 200); msgIsError = true; audio.playError(); return;
        }
        if (!Validator::isPositiveInt(tbAge.getText())) {
            strCopy(msg, "Age must be a positive number.", 200); msgIsError = true; audio.playError(); return;
        }
        // Add patient
        int newId = sys.getPatients().maxId() + 1;
        float bal = strToFloat(tbBalance.getText());
        if (bal < 0) bal = 0;
        Patient p(newId, tbName.getText(), strToInt(tbAge.getText()),
            tbGender.getText(), tbContact.getText(), tbPw.getText(), bal);
        sys.getPatients().add(p);
        FileHandler::appendPatient(p);
        char idBuf[10]; intToStr(newId, idBuf);
        strCopy(msg, "Patient registered! ID: ", 200); strConcat(msg, idBuf, 200);
        msgIsError = false; audio.playSuccess();
        tbName.clear(); tbAge.clear(); tbGender.clear();
        tbContact.clear(); tbPw.clear(); tbBalance.clear();
    }
}
void AddPatientScreen::draw(sf::RenderWindow& win, sf::Font& f) {
    drawBackground(win); drawHeader(win, f, "Register New Patient", "Fill all fields");
    tbName.draw(win); tbAge.draw(win); tbGender.draw(win); tbContact.draw(win);
    tbPw.draw(win); tbBalance.draw(win);
    btnAdd.draw(win); btnBack.draw(win); drawMessage(win, f, msg, msgIsError);
}

// ============================================================
//  AddDoctorScreen
// ============================================================
AddDoctorScreen::AddDoctorScreen() :msgIsError(false) { msg[0] = '\0'; }
void AddDoctorScreen::init(sf::Font& f) {
    tbName = TextBox(30, 130, 260, 34, f, 50);
    tbSpec = TextBox(310, 130, 260, 34, f, 50);
    tbContact = TextBox(590, 130, 200, 34, f, 12);
    tbPw = TextBox(30, 230, 200, 34, f, 20, true);
    tbFee = TextBox(250, 230, 160, 34, f, 10);
    tbName.setLabel("Name", f); tbSpec.setLabel("Specialization", f);
    tbContact.setLabel("Contact (11 digits)", f);
    tbPw.setLabel("Password (min 6)", f); tbFee.setLabel("Fee (PKR)", f);
    btnAdd = Button(30, 310, 160, 40, "Add Doctor", f);
    btnBack = Button(30, 560, 100, 34, "Back", f, 13);
}
void AddDoctorScreen::handle(sf::Event& e, Screen& cur, Audio& audio, HospitalSystem& sys) {
    tbName.handleEvent(e); tbSpec.handleEvent(e); tbContact.handleEvent(e);
    tbPw.handleEvent(e); tbFee.handleEvent(e);
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::AdminMenu; return; }
    if (btnAdd.isClicked(e)) {
        audio.playClick();
        if (!Validator::isValidContact(tbContact.getText())) { strCopy(msg, "Invalid contact.", 200); msgIsError = true; audio.playError(); return; }
        if (!Validator::isValidPassword(tbPw.getText())) { strCopy(msg, "Password min 6 chars.", 200); msgIsError = true; audio.playError(); return; }
        if (!Validator::isPositiveFloat(tbFee.getText())) { strCopy(msg, "Fee must be positive.", 200); msgIsError = true; audio.playError(); return; }
        char out[200];
        sys.addDoctor(tbName.getText(), tbSpec.getText(), tbContact.getText(),
            tbPw.getText(), strToFloat(tbFee.getText()), out);
        strCopy(msg, out, 200); msgIsError = false; audio.playSuccess();
        tbName.clear(); tbSpec.clear(); tbContact.clear(); tbPw.clear(); tbFee.clear();
    }
}
void AddDoctorScreen::draw(sf::RenderWindow& win, sf::Font& f) {
    drawBackground(win); drawHeader(win, f, "Add Doctor", "Fill all fields");
    tbName.draw(win); tbSpec.draw(win); tbContact.draw(win); tbPw.draw(win); tbFee.draw(win);
    btnAdd.draw(win); btnBack.draw(win); drawMessage(win, f, msg, msgIsError);
}

// ============================================================
//  RemoveDoctorScreen  ← NEW
// ============================================================
RemoveDoctorScreen::RemoveDoctorScreen() :msgIsError(false) { msg[0] = '\0'; }
void RemoveDoctorScreen::init(sf::Font& f) {
    tbDocId = TextBox(30, 360, 160, 34, f, 10);
    tbDocId.setLabel("Doctor ID to remove", f);
    btnRemove = Button(210, 360, 140, 34, "Remove", f, 13);
    btnBack = Button(30, 560, 100, 34, "Back", f, 13);
}
void RemoveDoctorScreen::handle(sf::Event& e, Screen& cur, Audio& audio, HospitalSystem& sys) {
    tbDocId.handleEvent(e);
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::AdminMenu; return; }
    if (btnRemove.isClicked(e)) {
        audio.playClick(); char out[200];
        bool ok = sys.removeDoctor(strToInt(tbDocId.getText()), out);
        strCopy(msg, out, 200); msgIsError = !ok;
        if (ok) { audio.playSuccess(); tbDocId.clear(); }
        else audio.playError();
    }
}
void RemoveDoctorScreen::draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys) {
    drawBackground(win); drawHeader(win, f, "Remove Doctor", "");
    const char* hdr[] = { "ID","Name","Specialization","Fee" };
    float xs[] = { 30,90,260,460 };
    drawTableHeader(win, f, hdr, xs, 4, 90);
    Storage<Doctor>& docs = sys.getDoctors();
    for (int i = 0; i < docs.size() && i < 14; i++) {
        char id[8], fee[12];
        intToStr(docs.get(i).getId(), id); floatToStr(docs.get(i).getFee(), fee);
        const char* v[] = { id,docs.get(i).getName(),docs.get(i).getSpecialization(),fee };
        drawTableRow(win, f, v, xs, 4, 120 + i * 27, i % 2);
    }
    tbDocId.draw(win); btnRemove.draw(win); btnBack.draw(win);
    drawMessage(win, f, msg, msgIsError);
}

// ============================================================
//  AdminDoctorsScreen
// ============================================================
AdminDoctorsScreen::AdminDoctorsScreen() {}
void AdminDoctorsScreen::init(sf::Font& f) { btnBack = Button(30, 560, 100, 34, "Back", f, 13); }
void AdminDoctorsScreen::handle(sf::Event& e, Screen& cur, Audio& audio) {
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::AdminMenu; }
}
void AdminDoctorsScreen::draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys) {
    drawBackground(win); drawHeader(win, f, "All Doctors", "");
    const char* hdr[] = { "ID","Name","Specialization","Contact","Fee" };
    float xs[] = { 30,90,260,440,580 };
    drawTableHeader(win, f, hdr, xs, 5, 90);
    Storage<Doctor>& docs = sys.getDoctors();
    for (int i = 0; i < docs.size() && i < 14; i++) {
        char id[8], fee[12];
        intToStr(docs.get(i).getId(), id); floatToStr(docs.get(i).getFee(), fee);
        const char* v[] = { id,docs.get(i).getName(),docs.get(i).getSpecialization(),docs.get(i).getContact(),fee };
        drawTableRow(win, f, v, xs, 5, 120 + i * 27, i % 2);
    }
    btnBack.draw(win);
}

// ============================================================
//  AdminAllApptsScreen
// ============================================================
AdminAllApptsScreen::AdminAllApptsScreen() :count(0) {}
void AdminAllApptsScreen::init(sf::Font& f) { btnBack = Button(30, 560, 100, 34, "Back", f, 13); }
void AdminAllApptsScreen::refresh(HospitalSystem& sys) { count = sys.getAllAppointments(appts, 100); }
void AdminAllApptsScreen::handle(sf::Event& e, Screen& cur, Audio& audio) {
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::AdminMenu; }
}
void AdminAllApptsScreen::draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys) {
    drawBackground(win); drawHeader(win, f, "All Appointments", "Sorted by date descending");
    const char* hdr[] = { "ID","Patient","Doctor","Date","Slot","Status" };
    float xs[] = { 30,80,210,360,460,530 };
    drawTableHeader(win, f, hdr, xs, 6, 90);
    for (int i = 0; i < count && i < 14; i++) {
        char id[8]; intToStr(appts[i].getId(), id);
        Patient* p = sys.findPatient(appts[i].getPatientId());
        Doctor* d = sys.findDoctor(appts[i].getDoctorId());
        const char* v[] = { id,p ? p->getName() : "?",d ? d->getName() : "?",
                         appts[i].getDate(),appts[i].getTimeSlot(),appts[i].getStatus() };
        drawTableRow(win, f, v, xs, 6, 120 + i * 27, i % 2);
    }
    btnBack.draw(win);
}

// ============================================================
//  AdminUnpaidBillsScreen
// ============================================================
AdminUnpaidBillsScreen::AdminUnpaidBillsScreen() :count(0) {}
void AdminUnpaidBillsScreen::init(sf::Font& f) { btnBack = Button(30, 560, 100, 34, "Back", f, 13); }
void AdminUnpaidBillsScreen::refresh(HospitalSystem& sys) { count = sys.getUnpaidBills(bills, 100); }
void AdminUnpaidBillsScreen::handle(sf::Event& e, Screen& cur, Audio& audio) {
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::AdminMenu; }
}
void AdminUnpaidBillsScreen::draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys) {
    drawBackground(win); drawHeader(win, f, "Unpaid Bills", "");
    const char* hdr[] = { "Bill ID","Patient","Amount (PKR)","Date" };
    float xs[] = { 30,110,280,430 };
    drawTableHeader(win, f, hdr, xs, 4, 90);
    for (int i = 0; i < count && i < 14; i++) {
        char bId[8], amt[15]; intToStr(bills[i].getId(), bId); floatToStr(bills[i].getAmount(), amt);
        Patient* p = sys.findPatient(bills[i].getPatientId());
        const char* v[] = { bId,p ? p->getName() : "Unknown",amt,bills[i].getDate() };
        drawTableRow(win, f, v, xs, 4, 120 + i * 27, i % 2);
    }
    btnBack.draw(win);
}

// ============================================================
//  DischargePatientScreen
// ============================================================
DischargePatientScreen::DischargePatientScreen() :msgIsError(false) { msg[0] = '\0'; }
void DischargePatientScreen::init(sf::Font& f) {
    tbPatientId = TextBox(30, 360, 160, 34, f, 10);
    tbPatientId.setLabel("Patient ID to discharge", f);
    btnDischarge = Button(210, 360, 160, 34, "Discharge", f, 13);
    btnBack = Button(30, 560, 100, 34, "Back", f, 13);
}
void DischargePatientScreen::handle(sf::Event& e, Screen& cur, Audio& audio, HospitalSystem& sys) {
    tbPatientId.handleEvent(e);
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::AdminMenu; return; }
    if (btnDischarge.isClicked(e)) {
        audio.playClick(); char out[200];
        bool ok = sys.dischargePatient(strToInt(tbPatientId.getText()), out);
        strCopy(msg, out, 200); msgIsError = !ok;
        if (ok) { audio.playSuccess(); tbPatientId.clear(); }
        else audio.playError();
    }
}
void DischargePatientScreen::draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys) {
    drawBackground(win); drawHeader(win, f, "Discharge Patient", "No unpaid bills or pending appointments allowed");
    Storage<Patient>& pts = sys.getPatients();
    const char* hdr[] = { "ID","Name","Contact","Balance" };
    float xs[] = { 30,90,260,430 };
    drawTableHeader(win, f, hdr, xs, 4, 90);
    for (int i = 0; i < pts.size() && i < 14; i++) {
        char id[8], bal[15]; intToStr(pts.get(i).getId(), id); floatToStr(pts.get(i).getBalance(), bal);
        const char* v[] = { id,pts.get(i).getName(),pts.get(i).getContact(),bal };
        drawTableRow(win, f, v, xs, 4, 120 + i * 27, i % 2);
    }
    tbPatientId.draw(win); btnDischarge.draw(win); btnBack.draw(win);
    drawMessage(win, f, msg, msgIsError);
}

// ============================================================
//  DailyReportScreen
// ============================================================
DailyReportScreen::DailyReportScreen() {}
void DailyReportScreen::init(sf::Font& f) { btnBack = Button(30, 560, 100, 34, "Back", f, 13); }
void DailyReportScreen::handle(sf::Event& e, Screen& cur, Audio& audio) {
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::AdminMenu; }
}
void DailyReportScreen::draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys) {
    drawBackground(win); drawHeader(win, f, "Daily Report", "");
    char today[12]; sys.getTodayString(today);
    char titleLine[60]; strCopy(titleLine, "Report for: ", 60); strConcat(titleLine, today, 60);
    win.draw(makeText(titleLine, f, 14, Color::ACCENT, 30, 90));

    // Count appointments by status
    int total = 0, pending = 0, completed = 0, noshow = 0, cancelled = 0; float revenue = 0;
    Storage<Appointment>& appts = sys.getAppointments();
    Storage<Bill>& bills = sys.getBills();
    for (int i = 0; i < appts.size(); i++) {
        if (strCmp(appts.get(i).getDate(), today) == 0) {
            total++;
            if (strCmp(appts.get(i).getStatus(), "pending") == 0)   pending++;
            if (strCmp(appts.get(i).getStatus(), "completed") == 0) completed++;
            if (strCmp(appts.get(i).getStatus(), "noshow") == 0)    noshow++;
            if (strCmp(appts.get(i).getStatus(), "cancelled") == 0) cancelled++;
        }
    }
    for (int i = 0; i < bills.size(); i++)
        if (strCmp(bills.get(i).getDate(), today) == 0 && strCmp(bills.get(i).getStatus(), "paid") == 0)
            revenue += bills.get(i).getAmount();

    char line[100];
    char tb[10]; intToStr(total, tb);    strCopy(line, "Total Appointments: ", 100); strConcat(line, tb, 100);
    win.draw(makeText(line, f, 13, Color::TEXT_COLOR, 30, 125));
    intToStr(pending, tb);   strCopy(line, "  Pending: ", 100);   strConcat(line, tb, 100);
    win.draw(makeText(line, f, 13, Color::SUBTEXT, 30, 150));
    intToStr(completed, tb); strCopy(line, "  Completed: ", 100); strConcat(line, tb, 100);
    win.draw(makeText(line, f, 13, Color::SUCCESS, 30, 175));
    intToStr(noshow, tb);    strCopy(line, "  No-Show: ", 100);   strConcat(line, tb, 100);
    win.draw(makeText(line, f, 13, Color::ERROR_C, 30, 200));
    intToStr(cancelled, tb); strCopy(line, "  Cancelled: ", 100); strConcat(line, tb, 100);
    win.draw(makeText(line, f, 13, Color::SUBTEXT, 30, 225));
    char rev[20]; floatToStr(revenue, rev);
    strCopy(line, "Revenue Collected (paid): PKR ", 100); strConcat(line, rev, 100);
    win.draw(makeText(line, f, 13, Color::ACCENT, 30, 260));
    btnBack.draw(win);
}

// ============================================================
//  AdminPatientsScreen
// ============================================================
void AdminPatientsScreen::init(sf::Font& f) { btnBack = Button(30, 560, 100, 34, "Back", f, 13); }
void AdminPatientsScreen::handle(sf::Event& e, Screen& cur, Audio& audio) {
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::AdminMenu; }
}
void AdminPatientsScreen::draw(sf::RenderWindow& win, sf::Font& f, HospitalSystem& sys) {
    drawBackground(win); drawHeader(win, f, "All Patients", "");
    const char* hdr[] = { "ID","Name","Age","Gender","Contact","Balance" };
    float xs[] = { 30,80,240,295,345,500 };
    drawTableHeader(win, f, hdr, xs, 6, 90);
    Storage<Patient>& pts = sys.getPatients();
    for (int i = 0; i < pts.size() && i < 14; i++) {
        char id[8], age[6], bal[15];
        intToStr(pts.get(i).getId(), id); intToStr(pts.get(i).getAge(), age);
        floatToStr(pts.get(i).getBalance(), bal);
        const char* v[] = { id,pts.get(i).getName(),age,pts.get(i).getGender(),pts.get(i).getContact(),bal };
        drawTableRow(win, f, v, xs, 6, 120 + i * 27, i % 2);
    }
    btnBack.draw(win);
}

// ============================================================
//  SecurityLogScreen
// ============================================================
void SecurityLogScreen::init(sf::Font& f) { btnBack = Button(30, 560, 100, 34, "Back", f, 13); }
void SecurityLogScreen::handle(sf::Event& e, Screen& cur, Audio& audio) {
    if (btnBack.isClicked(e)) { audio.playClick(); cur = Screen::AdminMenu; }
}
void SecurityLogScreen::draw(sf::RenderWindow& win, sf::Font& f) {
    drawBackground(win); drawHeader(win, f, "Security Log", "Failed login attempts");
    FILE* fp = fopen("security_log.txt", "r");
    if (!fp) { win.draw(makeText("No security events logged.", f, 14, Color::SUBTEXT, 30, 110)); btnBack.draw(win); return; }
    char line[300]; int row = 0;
    while (fgets(line, sizeof(line), fp) && row < 16) { trimNewline(line); win.draw(makeText(line, f, 12, Color::TEXT_COLOR, 30, 100 + row * 25)); row++; }
    fclose(fp); btnBack.draw(win);
}