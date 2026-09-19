#ifndef SYSTEM_HPP
#define SYSTEM_HPP
#include <string>
#include "../classes/Patient.hpp"
#include "../data_structures/Queue.hpp"

class System {
private:

    Queue<Patient*> patientsQueue;
    void loadPatients(const std::string& filePath);
    void showQueue() const;
    void treatPatients();
    void seeDepartment() const;
    void showRecord() const;

public:
    System();
    ~System();
    void execute();
};

#endif
