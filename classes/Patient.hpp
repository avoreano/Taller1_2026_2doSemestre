#ifndef PATIENT_HPP
#define PATIENT_HPP

#include "Person.hpp"
#include <string>
class Patient : public Person {
private:
    int id;
    std::string service;

public:
    Patient(int id, const std::string& name, int age, const std::string& service);
    ~Patient() override;

    int getId() const;
    std::string getService() const;

    void showInfo() const override;
};

#endif
