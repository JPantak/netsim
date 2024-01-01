#ifndef IMPLEMENTATION_NETSIM_SIMULATION_HPP
#define IMPLEMENTATION_NETSIM_SIMULATION_HPP

#include "factory.hpp"
#include <functional>
#include <system_error>
#include "reports.hpp"

void simulate(Factory &f, TimeOffset d, std::function<void (Factory &, Time)> rf);

#endif //IMPLEMENTATION_NETSIM_SIMULATION_HPP
