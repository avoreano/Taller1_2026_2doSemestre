#ifndef SYSTEM_HPP
#define SYSTEM_HPP
#include <string>

class System {
private:
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
