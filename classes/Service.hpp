#ifndef SERVICE_HPP
#define SERVICE_HPP
#include <string>
#include "Patient.hpp"
#include "LinkedList.hpp"

class Service {
private:
    std::string serviceName;
    LinkedList<Patient*> patientsList;

public:
    Service(const std::string& nombre);
    ~Service();

    std::string getName() const;
    void receivePatient(Patient* patient);
    void showStatus() const;
    int getPatientCount() const;
    bool hasPatientId(int id) const;
};

#endif