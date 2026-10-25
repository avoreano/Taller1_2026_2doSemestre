#include "System.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

namespace {
    bool parseInteger(const string& text, int& value) {
        try {
            size_t position;
            value = stoi(text, &position);
            return position == text.size();
        } catch (...) {
            return false;
        }
    }

    bool parseDecimal(const string& text, double& value) {
        try {
            size_t position;
            value = stod(text, &position);
            return position == text.size();
        } catch (...) {
            return false;
        }
    }

    bool isValidPainType(const string& painType) {
        return painType == "pecho" || painType == "cabeza" || painType == "abdomen" ||
               painType == "extremidad" || painType == "otro";
    }
}

System::System() {}
System::~System() {
    Patient* patient = nullptr;

    while (patientsQueue.dequeue(patient)) {
        delete patient;
    }
}

void System::loadPatients(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cout << "Error al abrir el archivo: " << filePath << endl;
        return;
    }

    string line;
    int loaded = 0;
    int rejected = 0;

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        stringstream ss(line);
        string id, name, ageStr, temperatureStr, bloodPressureStr, painStr, painType, traumaStr, neuroSymptomsStr, requiresSurgeryStr, hospitalDaysStr;
        getline(ss, id, ';');
        getline(ss, name, ';');
        getline(ss, ageStr, ';');
        getline(ss, temperatureStr, ';');
        getline(ss, bloodPressureStr, ';');
        getline(ss, painStr, ';');
        getline(ss, painType, ';');
        getline(ss, traumaStr, ';');
        getline(ss, neuroSymptomsStr, ';');
        getline(ss, requiresSurgeryStr, ';');
        getline(ss, hospitalDaysStr, ';');

        string extra;
        bool hasExtra = static_cast<bool>(getline(ss, extra, ';'));

        for (size_t i = 0; i < painType.size(); i++) {
            painType[i] = static_cast<char>(tolower(static_cast<unsigned char>(painType[i])));
        }

        int idInt, age, bloodPressure, pain, hospitalDays, trauma, neuroSymptoms, requiresSurgery;
        double temperature;

        if (hasExtra ||
            id.empty() || name.empty() || ageStr.empty() || temperatureStr.empty() ||
            bloodPressureStr.empty() || painStr.empty() || painType.empty() ||
            traumaStr.empty() || neuroSymptomsStr.empty() || requiresSurgeryStr.empty() ||
            hospitalDaysStr.empty() ||
            !parseInteger(id, idInt) ||
            !parseInteger(ageStr, age) ||
            !parseDecimal(temperatureStr, temperature) ||
            !parseInteger(bloodPressureStr, bloodPressure) ||
            !parseInteger(painStr, pain) ||
            !parseInteger(traumaStr, trauma) ||
            !parseInteger(neuroSymptomsStr, neuroSymptoms) ||
            !parseInteger(requiresSurgeryStr, requiresSurgery) ||
            !parseInteger(hospitalDaysStr, hospitalDays)) {
            cout << "Linea invalida (formato): " << line << endl;
            rejected++;
            continue;
        }

        if (idInt < 1 ||
            age < 0 || age > 120 ||
            temperature < 30.0 || temperature > 45.0 ||
            bloodPressure < 50 || bloodPressure > 250 ||
            pain < 0 || pain > 10 ||
            !isValidPainType(painType) ||
            (trauma != 0 && trauma != 1) ||
            (neuroSymptoms != 0 && neuroSymptoms != 1) ||
            (requiresSurgery != 0 && requiresSurgery != 1) ||
            hospitalDays < 0) {
            cout << "Linea invalida (valores fuera de rango): " << line << endl;
            rejected++;
            continue;
        }

        if (hasPatientId(idInt)) {
            cout << "Paciente duplicado con ID: " << idInt << endl;
            rejected++;
            continue;
        }

        Patient* newPatient = new Patient(idInt, name, age, temperature, bloodPressure,
                                          pain, painType, trauma == 1, neuroSymptoms == 1,
                                          requiresSurgery == 1, hospitalDays);
        patientsQueue.enqueue(newPatient);
        loaded++;
    }
    file.close();

    cout << "Carga finalizada: " << loaded << " paciente(s) cargado(s), "
         << rejected << " linea(s) rechazada(s)." << endl;
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