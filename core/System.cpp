#include "System.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

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
        cout << "ID: " << idInt << " | " << name << " | " << age << " a;os | " << condition << endl;
    }
    file.close();
    cout << "Archivo cargado correctamente: " << endl;
}

void System::treatPatients() {
    cout << "treatPatients() por implementar." << endl;
}

void System::seeDepartment() const {
    cout << "seeDepartment() por implementar." << endl;
}

void System::showRecord() const {
    cout << "showRecord() por implementar." << endl;
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