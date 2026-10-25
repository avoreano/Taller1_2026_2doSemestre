#ifndef PATIENT_HPP
#define PATIENT_HPP

#include "Person.hpp"
#include <string>

class Patient : public Person {
private:
    int id;
    double temperature;
    int bloodPressure;
    int pain;
    std::string painType;
    bool trauma;
    bool neuroSymptoms;
    bool requiresSurgery;
    int hospitalDays;
    std::string service;

public:
    Patient(int id, const std::string& name, int age, double temperature,
            int bloodPressure, int pain, const std::string& painType,
            bool trauma, bool neuroSymptoms, bool requiresSurgery,
            int hospitalDays);
    ~Patient() override;

    int getId() const;
    double getTemperature() const;
    int getBloodPressure() const;
    int getPain() const;
    std::string getPainType() const;
    bool hasTrauma() const;
    bool hasNeuroSymptoms() const;
    bool getRequiresSurgery() const;
    int getHospitalDays() const;
    std::string getService() const;

    void setService(const std::string& service);

    void showInfo() const override;
};

#endif