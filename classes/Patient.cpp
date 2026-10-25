#include "Patient.hpp"
#include <iostream>

Patient::Patient(int id, const std::string& name, int age, double temperature,
                 int bloodPressure, int pain, const std::string& painType,
                 bool trauma, bool neuroSymptoms, bool requiresSurgery,
                 int hospitalDays)
    : Person(name, age) {
    this->id = id;
    this->temperature = temperature;
    this->bloodPressure = bloodPressure;
    this->pain = pain;
    this->painType = painType;
    this->trauma = trauma;
    this->neuroSymptoms = neuroSymptoms;
    this->requiresSurgery = requiresSurgery;
    this->hospitalDays = hospitalDays;
    this->service = "";
}

Patient::~Patient() {}

int Patient::getId() const { return id; }
double Patient::getTemperature() const { return temperature; }
int Patient::getBloodPressure() const { return bloodPressure; }
int Patient::getPain() const { return pain; }
std::string Patient::getPainType() const { return painType; }
bool Patient::hasTrauma() const { return trauma; }
bool Patient::hasNeuroSymptoms() const { return neuroSymptoms; }
bool Patient::getRequiresSurgery() const { return requiresSurgery; }
int Patient::getHospitalDays() const { return hospitalDays; }
std::string Patient::getService() const { return service; }

void Patient::setService(const std::string& service) {
    this->service = service;
}

void Patient::showInfo() const {
    std::cout << "ID: " << id << std::endl;
    std::cout << "Nombre: " << getName() << std::endl;
    std::cout << "Edad: " << getAge() << std::endl;
    std::cout << "Temperatura: " << temperature << std::endl;
    std::cout << "Presion arterial: " << bloodPressure << std::endl;
    std::cout << "Dolor: " << pain << std::endl;
    std::cout << "TipoDolor: " << painType << std::endl;
    std::cout << "Trauma: " << trauma << std::endl;
    std::cout << "SintomasNeurologicos: " << neuroSymptoms << std::endl;
    std::cout << "RequiereCirugia: " << requiresSurgery << std::endl;
    std::cout << "DiasHospitalizacion: " << hospitalDays << std::endl;
}