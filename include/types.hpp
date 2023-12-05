//
// Created by mmich on 28.11.2023.
//

#ifndef IMPLEMENTATION_NETSIM_TYPES_HPP
#define IMPLEMENTATION_NETSIM_TYPES_HPP

#include <functional>

using ElementID = int;

using Time = unsigned int;

using TimeOffset = unsigned int;

using ProbabilityGenerator = std::function<double(void)>;

#endif //IMPLEMENTATION_NETSIM_TYPES_HPP
