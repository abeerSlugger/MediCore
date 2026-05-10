# MediCore Hospital Management System

## Project Overview
MediCore Hospital Management System is a console-based Object Oriented Programming (OOP) project developed in C++.  
The system manages hospital operations including:

- Patient Management
- Doctor Management
- Appointment Booking
- Billing System
- Prescriptions & Medical Records
- Security Logging
- Daily Reports

The project follows all assignment constraints provided in the OOP Spring 2026 project description.

---

## Features

### Patient Features
- Login Authentication
- Book Appointments
- Cancel Appointments
- View Appointments
- View Medical Records
- View Bills
- Pay Bills
- Top Up Balance

### Doctor Features
- Login Authentication
- View Today's Appointments
- Mark Appointments Complete
- Mark No-Show Appointments
- Write Prescriptions
- View Patient Medical History

### Admin Features
- Add Doctor
- Remove Doctor
- View All Patients
- View All Doctors
- View All Appointments
- View Unpaid Bills
- Discharge Patient
- View Security Logs
- Generate Daily Reports

---

## OOP Concepts Implemented

- Abstraction
- Encapsulation
- Inheritance
- Polymorphism
- Operator Overloading
- Templates
- Exception Handling
- File Handling

---

## Operator Overloading

The following operators are overloaded:

| Class | Operator | Purpose |
|------|------|------|
| Patient | += | Add balance |
| Patient | -= | Deduct balance |
| Patient | == | Compare patient IDs |
| Doctor | == | Compare doctor IDs |
| Appointment | == | Detect scheduling conflicts |
| Patient | << | Display patient details |
| Doctor | << | Display doctor details |
| Appointment | << | Display appointment details |

---

## Project Structure

```text
├── main.cpp
├── Person.h
├── Person.cpp
├── Patient.h
├── Patient.cpp
├── Doctor.h
├── Doctor.cpp
├── Admin.h
├── Admin.cpp
├── Appointment.h
├── Appointment.cpp
├── Bill.h
├── Bill.cpp
├── Prescription.h
├── Prescription.cpp
├── Storage.h
├── FileHandler.h
├── FileHandler.cpp
├── Validator.h
├── Validator.cpp
├── HospitalException.h
├── HospitalException.cpp
├── FileNotFoundException.h
├── FileNotFoundException.cpp
├── InvalidInputException.h
├── InvalidInputException.cpp
├── InsufficientFundsException.h
├── InsufficientFundsException.cpp
├── SlotUnavailableException.h
├── SlotUnavailableException.cpp
├── patients.txt
├── doctors.txt
├── admin.txt
├── appointments.txt
├── prescriptions.txt
├── bills.txt
├── security_log.txt
├── discharged.txt
└── README.md


## GitHub Repository

https://github.com/abeerSlugger/MediCore/tree/master/MediCore

---

## Developed By

- Name: Abeera Ahmed
- Roll Number: 25L-0860
- Course: Object Oriented Programming (OOP)
- Semester: Spring 2026
- Section: BSCS-2B

---