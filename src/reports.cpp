//
// Created by mmich on 28.11.2023.
//
#include "reports.hpp"
#include <algorithm>

void generate_structure_report(Factory& factory, std::ostream& os){
    os << "== LOADING RAMPS ==\n" << std::endl;
    for (auto ramp = factory.ramp_cbegin(); ramp != factory.ramp_cend(); ramp++) {
        os << "LOADING RAMP #" << (*ramp).get_id() << std::endl;
        os << "  Delivery interval: " << (*ramp).get_delivery_interval() << std::endl;
        os << "  Receivers: " << std::endl;
        std::map<IPackageReceiver*, double> worker_map;
        auto iter = (*ramp).receiver_preferences_.get_preferences().begin();
        while(std::find_if(iter,(*ramp).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Worker;}) != (*ramp).receiver_preferences_.get_preferences().end()){
            iter = std::find_if(iter,(*ramp).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Worker;});
            worker_map.insert(*iter);
        }
        std::sort(worker_map.begin(),worker_map.end(),[](std::pair<IPackageReceiver*, double> a, std::pair<IPackageReceiver*, double> b){return a.first->get_id() > b.first->get_id();});
        for(auto worker_pair : worker_map){
            os << "    worker #" << worker_pair.first->get_id() << std::endl;
        }
        std::map<IPackageReceiver*, double> storehouse_map;
        iter = (*ramp).receiver_preferences_.get_preferences().begin();
        while(std::find_if(iter,(*ramp).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Storehouse;}) != (*ramp).receiver_preferences_.get_preferences().end()){
            iter = std::find_if(iter,(*ramp).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Storehouse;});
            storehouse_map.insert(*iter);
        }
        std::sort(storehouse_map.begin(), storehouse_map.end(),[](std::pair<IPackageReceiver*, double> a, std::pair<IPackageReceiver*, double> b){return a.first->get_id() > b.first->get_id();});
        for(auto storehouse_pair : storehouse_map){
            os << "    storehouse #" << storehouse_pair.first->get_id() << std::endl;
        }
    }
    os << "== WORKERS ==\n" << std::endl;
    for (auto worker = factory.worker_cbegin(); worker != factory.worker_cend(); worker++) {
        os << "WORKER #" << (*worker).get_id() << std::endl;
        os << "  Processing time: " << (*worker).get_package_processing_start_time() << std::endl;
        os << "  Queue type: " << (*worker).get_queue() << std::endl;
        os << "  Receivers: " << std::endl;
        std::map<IPackageReceiver*, double> worker_map;
        auto iter = (*worker).receiver_preferences_.get_preferences().begin();
        while(std::find_if(iter,(*worker).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Worker;}) != (*worker).receiver_preferences_.get_preferences().end()){
            iter = std::find_if(iter,(*worker).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Worker;});
            worker_map.insert(*iter);
        }
        std::sort(worker_map.begin(),worker_map.end(),[](std::pair<IPackageReceiver*, double> a, std::pair<IPackageReceiver*, double> b){return a.first->get_id() > b.first->get_id();});
        for(auto worker_pair : worker_map){
            os << "    worker #" << worker_pair.first->get_id() << std::endl;
        }
        std::map<IPackageReceiver*, double> storehouse_map;
        iter = (*worker).receiver_preferences_.get_preferences().begin();
        while(std::find_if(iter,(*worker).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Storehouse;}) != (*worker).receiver_preferences_.get_preferences().end()){
            iter = std::find_if(iter,(*worker).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Storehouse;});
            storehouse_map.insert(*iter);
        }
        std::sort(storehouse_map.begin(), storehouse_map.end(),[](std::pair<IPackageReceiver*, double> a, std::pair<IPackageReceiver*, double> b){return a.first->get_id() > b.first->get_id();});
        for(auto storehouse_pair : storehouse_map){
            os << "    storehouse #" << storehouse_pair.first->get_id() << std::endl;
        }
    }
    os << "== STOREHOUSES ==\n" << std::endl;
    
    for(auto storehouse = factory.storehouse_cbegin(); storehouse != factory.storehouse_cend(); storehouse++){
        os << "  STOREHOUSE #" << storehouse->get_id() << std::endl;
    }
        

}

// void generate_simulation_turn_report(const Factory& factory, std::ostream& os,Time t){

// }