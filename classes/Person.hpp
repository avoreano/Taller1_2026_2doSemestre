#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>
class Person {
private:
    std::string name;
    int age;

public:
    Person(const std::string& name, int age);
    virtual ~Person();

    std::string getName() const;
    int getAge() const;

    virtual void showInfo() const;
};

#endif
