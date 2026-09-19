#include "Person.hpp"
#include <iostream>

Person::Person(const std::string& name, int age){
    this->name = name;
    this->age = age;
}

Person::~Person() {}

std::string Person::getName() const {
    return name;
}

int Person::getAge() const {
    return age;
}

void Person::showInfo() const {
    std::cout << "Nombre: " << name << " | Edad: " << age << std::endl;
}
