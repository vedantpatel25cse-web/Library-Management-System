// Person.h
// Base class for people in the system (Member derives from it).
#ifndef PERSON_H
#define PERSON_H

#include <string>

class Person {
protected:
    int         id;
    std::string name;
    std::string phone;

    void printCore() const;   // prints ID, name and phone as part of a table row

public:
    Person();
    Person(int id, const std::string& name, const std::string& phone);
    virtual ~Person();                       // virtual: safe deletion through Person*

    virtual void display() const;            // overridden by Member (runtime polymorphism)

    int                getId() const    { return id; }
    const std::string& getName() const  { return name; }
    const std::string& getPhone() const { return phone; }

    void setName(const std::string& n);
    void setPhone(const std::string& p);
};

#endif
