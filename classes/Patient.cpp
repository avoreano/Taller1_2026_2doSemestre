#include "Patient.hpp"
#include <iostream>

Patient::Patient(int id, const std::string& name, int age, const std::string& service)
    : Person(name, age) {
    this->id = id;
    this->service = service;
}

Patient::~Patient() {}

int Patient::getId() const {
    return id;
}

std::string Patient::getService() const {
    return service;
}

void Patient::showInfo() const {
    std::cout << "ID: " << id << std::endl;
    std::cout << "Nombre: " << getName() << std::endl;
    std::cout << "Edad: " << getAge() << std::endl;
    std::cout << "Servicio: " << getService() << std::endl;
}
