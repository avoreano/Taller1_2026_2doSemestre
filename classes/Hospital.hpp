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

    bool isValidService(const std::string& name) const;
    Service* searchService(const std::string& name) const;

    bool sendPatient(Patient* patient);
    bool hasPatientId(int id) const;

    // busca en los avl de todos los servicios. si lo encuentra deja el servicio en serviceName
    Patient* searchPatientById(int id, std::string& serviceName, std::string& route) const;
    Patient* searchPatientByName(const std::string& name, std::string& serviceName) const;

    void showServiceList() const;
    std::string getServiceNameByIndex(int index) const;
    int getServicesCount() const;

    void showServiceByName(const std::string& name) const;   // detalle del avl de un servicio
    void showAllServices() const;                            // estado de todos los servicios
};

#endif