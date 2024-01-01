//
// Created by mmich on 28.11.2023.
//

#ifndef IMPLEMENTATION_NETSIM_REPORTS_HPP
#define IMPLEMENTATION_NETSIM_REPORTS_HPP
#include<iostream>
#include "factory.hpp"

void generate_structure_report(const Factory& factory, std::ostream& os);
void generate_simulation_turn_report(const Factory& factory, std::ostream& os,Time t);

class SpecyficTurnsReportNotifier{};
class IntervalReportNotifier{};

#endif //IMPLEMENTATION_NETSIM_REPORTS_HPP
