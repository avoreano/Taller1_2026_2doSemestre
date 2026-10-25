#ifndef SERVICE_HPP
#define SERVICE_HPP
#include <string>
#include "Patient.hpp"
#include "AVLTree.hpp"

class Service {
private:
    std::string serviceName;
    AVLTree<Patient*> patientsTree;     // pacientes del servicio, ordenados por id

public:
    Service(const std::string& name);
    ~Service();

    std::string getName() const;
    bool receivePatient(Patient* patient);      // false si el id ya existe en este avl
    void showStatus() const;                    // nombre del servicio y sus pacientes
    void showDetails() const;                   // altura, balance y los 3 recorridos
    int getPatientCount() const;
    bool hasPatientId(int id) const;

    // busca por id y deja en route los ids visitados (ruta de busqueda)
    Patient* searchPatientById(int id, std::string& route) const;
    Patient* searchPatientByName(const std::string& name) const;
};

#endif