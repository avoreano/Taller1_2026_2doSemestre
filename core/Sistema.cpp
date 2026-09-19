#include "Sistema.hpp"
#include <iostream>

Sistema::Sistema() {}
Sistema::~Sistema() {}

void Sistema::atenderPacientes() {
    std::cout << "atenderPacientes() por implementar." << std::endl;
}

void Sistema::verDepartamento() const {
    std::cout << "verDepartamento() por implementar." << std::endl;
}

void Sistema::mostrarHistorialAtenciones() const {
    std::cout << "mostrarHistorialAtenciones() por implementar." << std::endl;
}

void Sistema::ejecutar() {
    int option = 0;
    while (option != 4) {
        std::cout << "=== INTEGRA MEDICA 2.0 ===" << std::endl;
        std::cout << "1. Atender pacientes" << std::endl;
        std::cout << "2. Ver departamento" << std::endl;
        std::cout << "3. Revisar historial de atencion" << std::endl;
        std::cout << "4. Salir" << std::endl;
        std::cout << "Seleccionar opcion: ";

        if (!(std::cin >> option)) {
            std::cin.clear();
            std::cout << "Opcion invalida, intente nuevamente." << std::endl;
            option = 0;
            continue;
        }

        switch (option) {
            case 1:
                atenderPacientes();
                break;
            case 2:
                verDepartamento();
                break;
            case 3:
                mostrarHistorialAtenciones();
                break;
            case 4:
                std::cout << "Finalizando el sistema..." << std::endl;
                std::cout << "Hasta luego :D." << std::endl;
                break;
            default:
                std::cout << "Opcion invalida, intente nuevamente." << std::endl;
        }
    }
}