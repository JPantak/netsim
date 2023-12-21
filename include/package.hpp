//
// Created by mmich on 28.11.2023.
//

#ifndef IMPLEMENTATION_NETSIM_PACKAGE_HPP
#define IMPLEMENTATION_NETSIM_PACKAGE_HPP

#include <ostream>
#include <set>
#include "types.hpp"

class Package{
public:
    Package();

    Package(ElementID id);

//    Package & operator = (Package &&) noexcept;
//
//    Package(Package&&) noexcept;

    Package(Package&&) = default;
    Package(const Package& p) = default;
    Package& operator = (Package&&) = default;


    ElementID get_id() const {return id_;};

    ~Package();
private:
    ElementID id_;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;

};


#endif //IMPLEMENTATION_NETSIM_PACKAGE_HPP
