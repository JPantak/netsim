//
// Created by mmich on 28.11.2023.
//

#ifndef IMPLEMENTATION_NETSIM_NODES_HPP
#define IMPLEMENTATION_NETSIM_NODES_HPP

#include "types.hpp"

class Ramp{
public:
    Ramp(ElementID id, TimeOffset di) : id_(id), di_(di) {};

    void deliver_goods(Time t);

    TimeOffset get_delivery_interval(void) const {return di_;};

    ElementID get_id(void) const {return id_;}
private:
    ElementID id_;
    TimeOffset di_;
};

#endif //IMPLEMENTATION_NETSIM_NODES_HPP
