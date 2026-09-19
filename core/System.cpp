#include "System.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

System::System() {}
System::~System() {}

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

        int idInt = stoi(id);
        int age = stoi(ageStr);

        Patient* newPatient = new Patient(idInt, name, age, condition);
        patientsQueue.enqueue(newPatient);
    }
    file.close();
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
    cout << "Indique la cantidad de pacientes a atender: ";

    if (!(cin >> amount)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max());

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
        }
    }
}

void System::seeDepartment() const {
    hospital.showServiceList();

    int option;
    cout << "Seleccionar opcion: ";
    if (!(cin >> option)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max());

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

    if (!(cin >> option)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max());

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