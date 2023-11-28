//
// Created by mmich on 28.11.2023.
//

#ifndef IMPLEMENTATION_NETSIM_PACKAGE_HPP
#define IMPLEMENTATION_NETSIM_PACKAGE_HPP

#include <ostream>
#include "types.hpp"

class Package{
public:
    Package();

    Package(ElementID id) : id_(id){}

    Package(Package&&);

    Package & operator = (Package &&);

    ElementID get_id() const {return id_;};

    ~Package();
private:
    ElementID id_;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;

};

#endif //IMPLEMENTATION_NETSIM_PACKAGE_HPP
