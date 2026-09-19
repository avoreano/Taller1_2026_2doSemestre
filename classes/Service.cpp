#include "Service.hpp"
#include <iostream>

using namespace std;

Service::Service(const string& nombre) {
    this->serviceName = nombre;
}

Service::~Service() {
    patientsList.forEach([](Patient* patient) {
        delete patient;
    });
}

string Service::getName() const {
    return serviceName;
}

void Service::receivePatient(Patient* patient) {
    patientsList.pushBack(patient);
}

void Service::showStatus() const {
    cout << "=== ESTADO " << serviceName << " ===" << endl;
    cout << "Pacientes en el departamento: "
         << patientsList.getSize() << endl;

    if (patientsList.isEmpty()) {
        cout << "(sin pacientes en este servicio)" << endl;
        return;
    }

    patientsList.forEach([](Patient* patient) {
        cout << "- " << patient->getName()
             << " (" << patient->getAge() << ")" << endl;
    });
}

int Service::getPatientCount() const {
    return patientsList.getSize();
}

bool Service::hasPatientId(int id) const {
    return patientsList.exists([id](Patient* patient) {
        return patient->getId() == id;
    });
}
