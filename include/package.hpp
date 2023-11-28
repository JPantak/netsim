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

    Package(ElementID);

    Package(Package&&);

    Package & operator = (Package &&);

    ElementID get_id() const;

    ~Package();
private:
    ElementID id_;
//    static std::set<ElementID> assigned_IDs;

};

#endif //IMPLEMENTATION_NETSIM_PACKAGE_HPP
