#include "System.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

System::System() {}
System::~System() {
    Patient* patient = nullptr;

    while (patientsQueue.dequeue(patient)) {
        delete patient;
    }
}

bool parseInteger(const string& text, int& value) {
    try {
        size_t position;
        value = stoi(text, &position);
        return position == text.size();
    } catch (...) {
        return false;
    }
}

void System::loadPatients(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cout << "Error al abrir el archivo: " << filePath << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        stringstream ss(line);
        string id, name, ageStr, condition;
        getline(ss, id, ';');
        getline(ss, name, ';');
        getline(ss, ageStr, ';');
        getline(ss, condition, ';');

        int idInt;
        int age;

        string extra;

        if (getline(ss, extra, ';') ||
            id.empty() ||
            name.empty() ||
            ageStr.empty() ||
            condition.empty() ||
            !parseInteger(id, idInt) ||
            !parseInteger(ageStr, age) ||
            idInt < 1 ||
            age < 0) {
            cout << "Linea invalida: " << line << endl;
            continue;
        }
        if (!hospital.isValidService(condition)) {
            cout << "Servicio invalido para el paciente " << idInt << ": " << condition << endl;
            continue;
        }
    
        if (hasPatientId(idInt)) {
            cout << "Paciente duplicado con ID: " << idInt << endl;
            continue;
        }

        Patient* newPatient = new Patient(idInt, name, age, condition);
        patientsQueue.enqueue(newPatient);

    }
    file.close();
}

bool System::hasPatientId(int id) const {
    if (patientsQueue.exists([id](Patient* patient) {
        return patient->getId() == id;
    })) {
        return true;
    }

    return hospital.hasPatientId(id);
}

void System::showQueue() const {
    cout << "=== PACIENTES EN ESPERA ===" << endl;

    if (patientsQueue.isEmpty()) {
        cout << "No hay pacientes en espera." << endl;
        return;
    }

    Node<Patient*>* current = patientsQueue.getFront();
    int number = 1;

    while (current != nullptr) {
        cout << number << ". "
             << current->data->getId() << "- "
             << current->data->getName() << endl;

        current = current->next;
        number++;
    }
}

void System::treatPatients() {
    if (patientsQueue.isEmpty()) {
        cout << "No hay pacientes para atender." << endl;
        return;
    }

    showQueue();

    int amount;
    string input;

    cout << "Indique la cantidad de pacientes a atender: ";

    if (!getline(cin >> ws, input) || !parseInteger(input, amount)) {
        cout << "Cantidad invalida." << endl;
        return;
    }

    if (amount < 1 || amount > patientsQueue.getSize()) {
        cout << "Cantidad invalida." << endl;
        return;
    }

    cout << "=== ATENDIENDO PACIENTES ===" << endl;

    for (int i = 0; i < amount; i++) {
        Patient* patientToTreat = nullptr;
        patientsQueue.dequeue(patientToTreat);

        patientToTreat->showInfo();

        if (hospital.sendPatient(patientToTreat)) {
            attentionHistory.push(patientToTreat);

            cout << "Paciente enviado a "
                 << patientToTreat->getService() << "." << endl;
        } else {
            delete patientToTreat;
            cout << "Servicio invalido para el paciente. Paciente descartado." << endl;
        }
    }
}

void System::seeDepartment() const {
    hospital.showServiceList();

int option;
    string input;

    cout << "Seleccionar opcion: ";

    if (!getline(cin >> ws, input) || !parseInteger(input, option)) {
        cout << "Opcion invalida." << endl;
        return;
    }

    if (option < 1 || option > hospital.getServicesCount()) {
        cout << "Departamento invalido." << endl;
        return;
    }

    string serviceName = hospital.getServiceNameByIndex(option);
    hospital.showServiceByName(serviceName);
}

void System::showRecord() const {
    cout << "=== HISTORIAL DE ULTIMAS ATENCIONES DEL HOSPITAL ===" << endl;

    if (attentionHistory.isEmpty()) {
        cout << "No hay atenciones registradas." << endl;
        return;
    }

    Node<Patient*>* current = attentionHistory.getTop();

    while (current != nullptr) {
        Patient* patient = current->data;

        cout << "Nombre: " << patient->getName()
             << " | Edad: " << patient->getAge()
             << " | Departamento: " << patient->getService()
             << endl;

        current = current->next;
    }
}

void System::execute() {
    loadPatients("pacientes.txt");
    int option = 0;
    while (option != 4) {
        cout << "=== INTEGRA MEDICA 2.0 ===" << endl;
        cout << "1. Atender pacientes" << endl;
        cout << "2. Ver departamento" << endl;
        cout << "3. Revisar historial de atencion" << endl;
        cout << "4. Salir" << endl;
        cout << "Seleccionar opcion: ";

        string input;

        if (!getline(cin >> ws, input)) {
            cout << "Entrada finalizada." << endl;
            break;
        }

        if (!parseInteger(input, option)) {
            cout << "Opcion invalida, intente nuevamente." << endl;
            option = 0;
            continue;
        }

        switch (option) {
            case 1:
                treatPatients();
                break;
            case 2:
                seeDepartment();
                break;
            case 3:
                showRecord();
                break;
            case 4:
                cout << "Finalizando el sistema..." << endl;
                cout << "Hasta luego :D." << endl;
                break;
            default:
                cout << "Opcion invalida, intente nuevamente." << endl;
        }
    }
}