#ifndef HOSPITAL_HPP
#define HOSPITAL_HPP

#include <string>
#include "Service.hpp"
#include "Patient.hpp"
#include "LinkedList.hpp"

class Hospital {
private:
    LinkedList<Service*> servicesList;

public:
    Hospital();
    ~Hospital();

    bool isValidService(const std::string& nombre) const;
    Service* searchService(const std::string& nombre) const;

    bool sendPatient(Patient* paciente);
    bool hasPatientId(int id) const;

    void showServiceList() const;
    std::string getServiceNameByIndex(int index) const;
    int getServicesCount() const;

    void showServiceByName(const std::string& nombre) const;
    void showAllServices() const;
};

#endif
