#include "Sistema.hpp"
#include <iostream>

Sistema::Sistema() {}
Sistema::~Sistema() {}

void Sistema::ejecutar() {
    bool salir = false;

    while (!salir) {
        std::cout << "=== INTEGRA MEDICA 2.0 ===";
        std::cout << "1. Atender pacientes";
        std::cout << "2. Ver departamento";
        std::cout << "3. Revisar historial de atencion";
        std::cout << "4. Salir";
        std::cout << "Seleccionar opcion: ";

        int opcion;
        if (!(std::cin >> opcion)) {
            std::cin.clear();
            std::cout << "Opcion invalida, intente nuevamente.";
            continue;
        }
        std::cout << "";

        switch (opcion) {
            case 1:
                atenderPacientes();
                break;
            case 2:
                atenderPacientes();
                break;
            case 3:
                verDepartamento();
                break;
            case 4:
                mostrarHistorialAtenciones();
                break;
            case 5:
                std::cout << "Finalizando el sistema...";
                std::cout << "Hasta luego :D.";
                salir = true;
                break;
            default:
                std::cout << "Opcion invalida, intente nuevamente.";
        }
    }
}