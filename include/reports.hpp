//
// Created by mmich on 28.11.2023.
//

#ifndef IMPLEMENTATION_NETSIM_REPORTS_HPP
#define IMPLEMENTATION_NETSIM_REPORTS_HPP
#include<iostream>
#include "factory.hpp"

void generate_structure_report(const Factory& factory, std::ostream& os);
void generate_simulation_turn_report(const Factory& factory, std::ostream& os,Time t);

class SpecyficTurnsReportNotifier{
public:
    SpecyficTurnsReportNotifier(std::set<Time> turns) : turns_(turns) {};

    bool should_generate_report(Time t) const;
private:
    std::set<Time> turns_;
};

class IntervalReportNotifier{
public:
    IntervalReportNotifier(TimeOffset to) : to_(to) {};

    bool should_generate_report(Time t) const;
private:
    TimeOffset to_;
};

#endif //IMPLEMENTATION_NETSIM_REPORTS_HPP
