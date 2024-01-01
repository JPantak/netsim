#include "simulation.hpp"

void simulate(Factory &f, TimeOffset d, std::function<void (Factory &, Time)> rf) {
    if (!f.is_consistent()) {
        throw std::logic_error("Factory network is inconsistent!!!");
    }

    for (Time current_simulation_time = 0; current_simulation_time < d; ++current_simulation_time) {
        f.do_delivieries(current_simulation_time);
        f.do_package_passing();
        f.do_work(current_simulation_time);
        rf(f, current_simulation_time);
    }
}