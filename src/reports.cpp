//
// Created by mmich on 28.11.2023.
//
#include "reports.hpp"
#include <algorithm>

void generate_structure_report(const Factory& factory, std::ostream& os){
    os << "\n== LOADING RAMPS ==\n" << std::endl;
    for (NodeCollection<Ramp>::const_iterator ramp = factory.ramp_cbegin(); ramp != factory.ramp_cend(); ramp++) {
        os << "LOADING RAMP #" << (*ramp).get_id() << std::endl;
        os << "  Delivery interval: " << (*ramp).get_delivery_interval() << std::endl;
        os << "  Receivers:" << std::endl;
        std::vector<ElementID> worker_ids;
        auto iter = (*ramp).receiver_preferences_.get_preferences().begin();
        while(std::find_if(iter,(*ramp).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Worker;}) != (*ramp).receiver_preferences_.get_preferences().end()){
            iter = std::find_if(iter,(*ramp).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Worker;});
            worker_ids.push_back(iter->first->get_id());
            iter++;
        }
        std::sort(worker_ids.begin(),worker_ids.end());
        for(auto elem: worker_ids){
            os << "    worker #" << elem << std::endl;
        }
        std::vector<ElementID> storehouse_ids;
        iter = (*ramp).receiver_preferences_.get_preferences().begin();
        while(std::find_if(iter,(*ramp).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Storehouse;}) != (*ramp).receiver_preferences_.get_preferences().end()){
            iter = std::find_if(iter,(*ramp).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Storehouse;});
            storehouse_ids.push_back(iter->first->get_id());
            iter++;
        }
        std::sort(storehouse_ids.begin(), storehouse_ids.end());
        for(auto elem : storehouse_ids){
            os << "    storehouse #" << elem << std::endl;
        }
        os << std::endl;
    }
    os << "\n== WORKERS ==\n" << std::endl;
    std::vector<NodeCollection<Worker>::const_iterator> worker_ptr_col;
    for (NodeCollection<Worker>::const_iterator worker = factory.worker_cbegin(); worker != factory.worker_cend(); worker++) {
        worker_ptr_col.push_back(worker);
    }
    std::sort(worker_ptr_col.begin(),worker_ptr_col.end(),[](NodeCollection<Worker>::const_iterator a, NodeCollection<Worker>::const_iterator b){return a->get_id() < b->get_id();});
    for(auto worker: worker_ptr_col){
        os << "WORKER #" << (*worker).get_id() << std::endl;
        os << "  Processing time: " << (*worker).get_processing_duration() << std::endl;
        if(worker->get_queue()->get_queue_type() == PackageQueueType::FIFO){
            os << "  Queue type: FIFO" << std::endl;
        }
        else{
            os << "  Queue type: LIFO" << std::endl;
        }
        
        os << "  Receivers:" << std::endl;        std::vector<ElementID> worker_ids;
        auto iter = (*worker).receiver_preferences_.get_preferences().begin();
        while(std::find_if(iter,(*worker).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Worker;}) != (*worker).receiver_preferences_.get_preferences().end()){
            iter = std::find_if(iter,(*worker).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Worker;});
            worker_ids.push_back(iter->first->get_id());
            iter++;
        }
        std::vector<ElementID> storehouse_ids;
        iter = (*worker).receiver_preferences_.get_preferences().begin();
        while(std::find_if(iter,(*worker).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Storehouse;}) != (*worker).receiver_preferences_.get_preferences().end()){
            iter = std::find_if(iter,(*worker).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Storehouse;});
            storehouse_ids.push_back(iter->first->get_id());
            iter++;
        }
        std::sort(storehouse_ids.begin(), storehouse_ids.end());
        for(auto elem : storehouse_ids){
            os << "    storehouse #" << elem << std::endl;
        }
        std::sort(worker_ids.begin(),worker_ids.end());
        for(auto elem: worker_ids){
            os << "    worker #" << elem << std::endl;
        }
        os << std::endl;
    }
    os << "\n== STOREHOUSES ==\n" << std::endl;
    std::vector<ElementID> storehouses_ids;
    for(NodeCollection<Storehouse>::const_iterator storehouse = factory.storehouse_cbegin(); storehouse != factory.storehouse_cend(); storehouse++){
        storehouses_ids.push_back(storehouse->get_id());
        // os << "STOREHOUSE #" << storehouse->get_id() << std::endl << std::endl;
    }
    std::sort(storehouses_ids.begin(),storehouses_ids.end());
    for(auto elem: storehouses_ids){
        os << "STOREHOUSE #" << elem << std::endl << std::endl;
    }
}

void generate_simulation_turn_report(const Factory& factory, std::ostream& os,Time t){
    os << "=== [ Turn: " << t << " ] ===\n" << std::endl;
    os << "== WORKERS ==\n" << std::endl;
    std::vector<Worker*> Workers;
    for (NodeCollection<Ramp>::const_iterator ramp = factory.ramp_cbegin(); ramp != factory.ramp_cend(); ramp++) {
        auto iter = (*ramp).receiver_preferences_.get_preferences().begin();
        while(std::find_if(iter,(*ramp).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Worker;}) != (*ramp).receiver_preferences_.get_preferences().end()){
            iter = std::find_if(iter,(*ramp).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Worker;});
            Workers.push_back((Worker*)iter->first);
            iter++;
        }
    }
    for (NodeCollection<Worker>::const_iterator worker = factory.worker_cbegin(); worker != factory.worker_cend(); worker++) {
        auto iter = (*worker).receiver_preferences_.get_preferences().begin();
        while(std::find_if(iter,(*worker).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Worker;}) != (*worker).receiver_preferences_.get_preferences().end()){
            iter = std::find_if(iter,(*worker).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Worker;});
            Workers.push_back((Worker*)iter->first);
            iter++;
        }
    }
    std::sort(Workers.begin(),Workers.end(),[](Worker* a, Worker* b){return a->get_id() < b->get_id();});
    for(auto worker : Workers){
        os << "WORKER #" << worker->get_id() << std::endl;
        if(worker->get_processing_buffer().has_value()){
            os << "  PBuffer: #" << worker->get_processing_buffer()->get_id() << " (pt = " << worker->get_package_processing_start_time() << ")" << std::endl;
        }
        else{
            os << "  PBuffer: (empty)" << std::endl;
        }
        os << "  Queue: ";
        if (worker->begin() == worker->end()){
            os << "(empty)" << std::endl;
        }
        else{
            for(auto it = worker->begin(); it != worker->end(); it++){
                if(it == worker->begin()){
                    os << "#" << it->get_id();
                }
                else{
                    os << ", #" << it->get_id();
                }
                os << std::endl;
            }
        }
        //os << "  SBuffer: (empty)\n" << std::endl;
        if (worker->get_sending_buffer().has_value()) {
            os << "  SBuffer: #" << worker->get_sending_buffer()->get_id() << "\n" << std::endl;
        } else {
            os << "  SBuffer: (empty)\n" << std::endl;
        }

        
    }
    
    os << "\n== STOREHOUSES ==\n" << std::endl;
    std::vector<Storehouse*> Storehouses;
    for (NodeCollection<Ramp>::const_iterator ramp = factory.ramp_cbegin(); ramp != factory.ramp_cend(); ramp++) {
        auto iter = (*ramp).receiver_preferences_.get_preferences().begin();
        while(std::find_if(iter,(*ramp).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Storehouse;}) != (*ramp).receiver_preferences_.get_preferences().end()){
            iter = std::find_if(iter,(*ramp).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Storehouse;});
            Storehouses.push_back((Storehouse*)iter->first);
            iter++;
        }
    }
    for (NodeCollection<Worker>::const_iterator worker = factory.worker_cbegin(); worker != factory.worker_cend(); worker++) {
        auto iter = (*worker).receiver_preferences_.get_preferences().begin();
        while(std::find_if(iter,(*worker).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Storehouse;}) != (*worker).receiver_preferences_.get_preferences().end()){
            iter = std::find_if(iter,(*worker).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Storehouse;});
            Storehouses.push_back((Storehouse*)iter->first);
            iter++;
        }
    }
    std::sort(Storehouses.begin(),Storehouses.end(),[](Storehouse* a, Storehouse* b){return a->get_id() < b->get_id();});
    for(auto storehouse : Storehouses){
        os << "STOREHOUSE #" << storehouse->get_id() << std::endl;
        os << "  Stock: ";
        std::vector<ElementID> storehouse_ids2;
        if (storehouse->begin() == storehouse->end()){
            os << "(empty)" << std::endl;
        }
        else{
            for(auto it = storehouse->begin(); it != storehouse->end(); it++){
                storehouse_ids2.push_back(it->get_id());
            }
            std::sort(storehouse_ids2.begin(),storehouse_ids2.end());
            for(auto it = storehouse_ids2.begin(); it != storehouse_ids2.end();it++){
                if(it == storehouse_ids2.begin()){
                    os << "#" << *it;
                }
                else{
                    os << ", #" << *it;
                }
                os << std::endl;
            }
        }
    }
    os << std::endl;
   
}

bool SpecyficTurnsReportNotifier::should_generate_report(Time t) const{
    if (turns_.count(t)){
        return true;
    }
    return false;
}

bool IntervalReportNotifier::should_generate_report(Time t) const{
    if (to_ < t){
        return true;
    }
    return false;
}
