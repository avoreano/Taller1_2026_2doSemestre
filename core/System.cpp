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

        // el arbol de decision decide el servicio y deja la ruta que siguio
        string route;
        string serviceName = decisionTree.evaluate(patientToTreat, route);

        if (serviceName.empty()) {
            cout << "No se pudo clasificar al paciente. Paciente descartado." << endl << endl;
            delete patientToTreat;
            continue;
        }

        patientToTreat->setService(serviceName);
        cout << "Ruta: " << route << endl;
        cout << "Servicio asignado: " << serviceName << endl;

        if (hospital.sendPatient(patientToTreat)) {
            cout << "Paciente insertado en el AVL de " << serviceName << "." << endl << endl;
        } else {
            cout << "No se pudo insertar al paciente en el AVL. Paciente descartado." << endl << endl;
            delete patientToTreat;
        }
    }
}

void System::seeDepartment() const {
    hospital.showAllServices();
    cout << endl;
    hospital.showServiceList();

    int option;
    string input;

    cout << "Seleccione un departamento para ver su AVL (0 para volver): ";

    if (!getline(cin >> ws, input) || !parseInteger(input, option)) {
        cout << "Opcion invalida." << endl;
        return;
    }

    if (option == 0) {
        return;
    }

    if (option < 1 || option > hospital.getServicesCount()) {
        cout << "Departamento invalido." << endl;
        return;
    }

    cout << endl;
    string serviceName = hospital.getServiceNameByIndex(option);
    hospital.showServiceByName(serviceName);
}

namespace {
    // muestra los datos de un paciente ya clasificado
    void showPatientDetails(const Patient* patient, const string& serviceName) {
        cout << "-------" << endl;
        cout << "ID: " << patient->getId() << endl;
        cout << "Nombre: " << patient->getName() << endl;
        cout << "Edad: " << patient->getAge() << endl;
        cout << "Departamento: " << serviceName << endl;
        cout << "T: " << patient->getTemperature() << " C"
             << " | P: " << patient->getBloodPressure() << " mmHg"
             << " | D: " << patient->getPain()
             << " | TD: " << patient->getPainType()
             << " | Tr: " << (patient->hasTrauma() ? "Si" : "No")
             << " | SN: " << (patient->hasNeuroSymptoms() ? "Si" : "No")
             << " | RC: " << (patient->getRequiresSurgery() ? "Si" : "No")
             << " | DH: ";
        if (patient->getHospitalDays() == 0) {
            cout << "Ninguno" << endl;
        } else {
            cout << patient->getHospitalDays() << endl;
        }
        cout << "-------" << endl;
    }
}

void System::searchPatient() const {
    cout << "=== BUSCAR PACIENTE ===" << endl;
    cout << "Indique el ID o Nombre del paciente: ";

    string input;
    if (!getline(cin >> ws, input)) {
        cout << "Entrada invalida." << endl;
        return;
    }

    // se quitan los espacios del final
    while (!input.empty() && isspace(static_cast<unsigned char>(input[input.size() - 1]))) {
        input.erase(input.size() - 1);
    }

    if (input.empty()) {
        cout << "Entrada invalida." << endl;
        return;
    }

    cout << "Buscando en AVL..." << endl;

    Patient* found = nullptr;
    string serviceName;
    string route;
    int id;
    bool searchById = parseInteger(input, id);

    // un numero se busca por id
    if (searchById) {
        found = hospital.searchPatientById(id, serviceName, route);
    } else {
        found = hospital.searchPatientByName(input, serviceName);
    }

    if (found == nullptr) {
        cout << "Paciente no encontrado en los AVL." << endl;

        // si todavia no fue atendido, esta en la cola y no en un avl
        bool waiting = patientsQueue.exists([&](Patient* patient) {
            if (searchById) {
                return patient->getId() == id;
            }
            return patient->getName() == input;
        });
        if (waiting) {
            cout << "El paciente esta en la cola de espera (aun no ha sido atendido)." << endl;
        }
        return;
    }

    cout << "Paciente encontrado:" << endl;
    showPatientDetails(found, serviceName);

    if (searchById) {
        cout << "Ruta de busqueda:" << endl;
        cout << route << endl;
    } else {
        cout << "Busqueda por nombre: se recorrieron los AVL de los servicios en inorden." << endl;
    }
}

void System::execute() {
    loadPatients("pacientes.txt");
    int option = 0;
    while (option != 4) {
        cout << "=== INTEGRA MEDICA 2.0 ===" << endl;
        cout << "1. Atender pacientes" << endl;
        cout << "2. Ver departamentos" << endl;
        cout << "3. Buscar paciente" << endl;
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
                searchPatient();
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