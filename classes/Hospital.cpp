#include "Hospital.hpp"
#include <iostream>

using namespace std;

Hospital::Hospital() {
    const string servicesNames[8] = {
        "Urgencias", "Medicina General", "Cardiologia", "Neurologia",
        "Traumatologia", "Cirugia", "Pediatria", "Hospitalizacion"
    };

    for (const string& name : servicesNames) {
        servicesList.pushBack(new Service(name));
    }
}

Hospital::~Hospital() {
    servicesList.forEach([](Service* service) {
        delete service;
    });
}

bool Hospital::isValidService(const string& name) const {
    return searchService(name) != nullptr;
}

Service* Hospital::searchService(const string& name) const {
    string objective = name;
    Node<Service*>* actual = servicesList.getHead();
    while (actual != nullptr) {
        if (actual->data->getName() == objective) {
            return actual->data;
        }
        actual = actual->next;
    }
    return nullptr;
}

bool Hospital::sendPatient(Patient* patient) {
    Service* service = searchService(patient->getService());
    if (service == nullptr) {
        return false;
    }
    service->receivePatient(patient);
    return true;
}

bool Hospital::hasPatientId(int id) const {
    return servicesList.exists([id](Service* service) {
        return service->hasPatientId(id);
    });
}

void Hospital::showServiceList() const {
    cout << "=== DEPARTAMENTOS/SERVICIOS ===\n";
    int indice = 1;
    servicesList.forEach([&indice](Service* service) {
        cout << indice << ". " << service->getName() << "\n";
        indice++;
    });
}

string Hospital::getServiceNameByIndex(int indice) const {
    Node<Service*>* actual = servicesList.getHead();
    int contador = 1;
    while (actual != nullptr) {
        if (contador == indice) {
            return actual->data->getName();
        }
        actual = actual->next;
        contador++;
    }
    return "";
}

int Hospital::getServicesCount() const {
    return servicesList.getSize();
}

void Hospital::showServiceByName(const std::string& name) const {
    Service* service = searchService(name);
    if (service == nullptr) {
        std::cout << "Service not found.\n";
        return;
    }
    service->showStatus();
}

void Hospital::showAllServices() const {
    servicesList.forEach([](Service* service) {
        service->showStatus();
        std::cout << "\n";
    });
}
