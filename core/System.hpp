#ifndef SYSTEM_HPP
#define SYSTEM_HPP
#include <string>
#include "../classes/Patient.hpp"
#include "../data_structures/Queue.hpp"
#include "../classes/Hospital.hpp"
#include "../classes/DecisionTree.hpp"

class System {
private:

    Queue<Patient*> patientsQueue;
    Hospital hospital;
    DecisionTree decisionTree;
    bool hasPatientId(int id) const;

    void loadPatients(const std::string& filePath);
    void showQueue() const;
    void treatPatients();
    void seeDepartment() const;
    void searchPatient() const;

public:
    System();
    ~System();
    void execute();
};

#endif