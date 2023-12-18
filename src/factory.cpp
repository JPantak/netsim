#include "factory.hpp"


bool Factory::has_reachable_storehouse(const PackageSender *sender, std::map<const PackageSender *, NodeColor> &node_color) {
    if (node_color[sender] == NodeColor::Verified) {
        return true;
    }

    node_color[sender] = NodeColor::Visited;

    if (sender->receiver_preferences_.get_preferences().empty()){
        throw std::logic_error("Exception: This sender has no receivers!!!");
    }

    bool has_receivers = false;
    for (auto& [rec, pri] : sender->receiver_preferences_.get_preferences()) {
        if (rec->get_receiver_type() == ReceiverType::Storehouse) {
            has_receivers = true;
        } else if (rec->get_receiver_type() == ReceiverType::Worker) {
            IPackageReceiver* receiver_ptr = rec;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);
            if (sendrecv_ptr == sender) {
                continue;
            }
            has_receivers = true;
            if (node_color[sendrecv_ptr] == NodeColor::Univisited) {
                has_reachable_storehouse(sendrecv_ptr, node_color);
            }
        }
    }
    node_color[sender] = NodeColor::Verified;

    if (has_receivers) {
        return true;
    }
    else throw std::logic_error("Exception: Sender has no receivers!!!!");
}

bool Factory::is_consistent(void){
    std::map<const PackageSender*, NodeColor> node_color;

    for (Worker &worker : Workers) {
        const PackageSender *worker_ptr = &worker;
        node_color[worker_ptr] = NodeColor::Univisited;
    }

    for (Ramp &ramp : Ramps) {
        const PackageSender *ramp_ptr = &ramp;
        node_color[ramp_ptr] = NodeColor::Univisited;

        //Checking for exception from auxiliary function
        try {
            has_reachable_storehouse(ramp_ptr, node_color);
        } catch (std::logic_error &error) {
            return false;
        }
    }
    return true;
};

void Factory::do_delivieries(Time t) {
    for (auto& ramp : Ramps) {
        ramp.deliver_goods(t);
    }
};

void Factory::do_package_passing(void) {
    for (auto& ramp : Ramps) {
        ramp.send_package();
    }

    for(auto& worker : Workers) {
        worker.send_package();
    }
};

void Factory::do_work(Time t) {
    for (auto& worker : Workers) {
        worker.do_work(t);
    }
};