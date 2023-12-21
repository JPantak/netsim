#include "factory.hpp"
#include <istream>
#include <string>
#include <sstream>
#include <algorithm>

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

void Factory::remove_worker(ElementID id) {
    remove_receiver(Workers, id);
    Workers.remove_by_id(id);
}

void Factory::remove_storehouse(ElementID id){
    remove_receiver(Storehouses, id);
    Storehouses.remove_by_id(id);
}

template <class Node>
void Factory::remove_receiver(NodeCollection<Node> &collection, ElementID id) {
    auto ptr = dynamic_cast<IPackageReceiver*>(&(*collection.find_by_id(id)));
    for (Worker& sender: Workers) {
        sender.receiver_preferences_.remove_receiver(ptr);
    }
    if (std::is_same<Node, Worker>::value) {
        for (Ramp& sender: Ramps) {
            sender.receiver_preferences_.remove_receiver(ptr);
        }
    }
}

ParsedLineData parse_line(std::string line){
    ParsedLineData out;
    std::vector<std::string> tokens;
    std::string token;
 
    std::istringstream token_stream(line);
    char delimiter = ' ';
    char id_delimiter = '=';
    std::getline(token_stream, token, delimiter);
    if(token == "LOADING_RAMP"){
        out.type = RAMP;
    }
    if(token == "WORKER"){
        out.type = WORKER;
    }
    if(token == "STOREHOUSE"){
        out.type = STOREHOUSE;
    }
    if(token == "LINK"){
        out.type = LINK;
    }
    
    while (std::getline(token_stream, token, delimiter)) {
        std::string key = token.substr(0, token.find(id_delimiter));
        std::string id = token.substr(token.find(id_delimiter) + 1, token.size()-1);
        out.map.insert(std::make_pair(key,id));
    }
    return out;
}

std::pair<std::string, std::string> parse_type_id(std::string line){
    std::pair<std::string, std::string> result;
    std::istringstream token_stream(line);
    std::string token;

    std::getline(token_stream, token, '=');

    std::string key = token.substr(0, token.find('-'));
    std::string id = token.substr(token.find('-') + 1, token.size()-1);
    result = std::make_pair(key, id);
    return result;
}

Factory load_factory_structure(std::istream& is){
    
    Factory factory;
    std::string line;
    while (std::getline(is, line)){
        if(line[0] == ';' || line.size() == 0){
            continue;
        }
        ParsedLineData elem = parse_line(line);
        if(elem.type == RAMP){
            ElementID id = std::stoi((*elem.map.find("id")).second);
            TimeOffset di = static_cast<TimeOffset>(std::stoi((*elem.map.find("delivery-interval")).second));
            factory.add_ramp(Ramp(id,di));
        }
        if(elem.type == WORKER){
            ElementID id = std::stoi((*elem.map.find("id")).second);
            TimeOffset pt = static_cast<TimeOffset>(std::stoi((*elem.map.find("processing-time")).second));
            PackageQueueType type;
            if((*elem.map.find("queue-type")).second == "LIFO"){
                type = PackageQueueType::LIFO;
            }
            if((*elem.map.find("queue-type")).second == "FIFO"){
                type = PackageQueueType::FIFO;
            }

            std::unique_ptr<PackageQueue> q = std::make_unique<PackageQueue>(PackageQueue(type));
            factory.add_worker(Worker(id,pt,std::move(q)));
        }
        if(elem.type == STOREHOUSE){
            ElementID id = std::stoi((*elem.map.find("id")).second);
            factory.add_storehouse(Storehouse(id));
        }
        if(elem.type == LINK){
            std::pair<std::string, std::string> type_id_sender = parse_type_id((*elem.map.find("src")).second);
            std::pair<std::string, std::string> type_id_receiver = parse_type_id((*elem.map.find("dest")).second);
            if(type_id_sender.first == "ramp" && type_id_receiver.first == "store") {
                ElementID id_sender = std::stoi(type_id_sender.second);
                ElementID id_receiver = std::stoi(type_id_receiver.second);
                Storehouse& receiver = *(factory.find_storehouse_by_id(id_receiver));
                factory.find_ramp_by_id(id_sender)->receiver_preferences_.add_receiver(&receiver);
            }
            if(type_id_sender.first == "ramp" && type_id_receiver.first == "worker") {
                ElementID id_sender = std::stoi(type_id_sender.second);
                ElementID id_receiver = std::stoi(type_id_receiver.second);
                Worker& receiver = *(factory.find_worker_by_id(id_receiver));
                factory.find_ramp_by_id(id_sender)->receiver_preferences_.add_receiver(&receiver);
            }
            if(type_id_sender.first == "worker" && type_id_receiver.first == "worker") {
                ElementID id_sender = std::stoi(type_id_sender.second);
                ElementID id_receiver = std::stoi(type_id_receiver.second);
                Worker& receiver = *(factory.find_worker_by_id(id_receiver));
                factory.find_worker_by_id(id_sender)->receiver_preferences_.add_receiver(&receiver);
            }
            if(type_id_sender.first == "worker" && type_id_receiver.first == "store") {
                ElementID id_sender = std::stoi(type_id_sender.second);
                ElementID id_receiver = std::stoi(type_id_receiver.second);
                Storehouse& receiver = *(factory.find_storehouse_by_id(id_receiver));
                factory.find_worker_by_id(id_sender)->receiver_preferences_.add_receiver(&receiver);
            }
        }
	
    }
    return factory;
}

std::list<std::pair<std::string, std::string>> make_pairs(const Ramp& sender){
    std::list<std::pair<std::string, std::string>> result;

    std::string sender_type = "ramp";

    std::string sender_id = std::to_string(sender.get_id());

    std::string sender_string = sender_type + "-" + sender_id;

    std::map<IPackageReceiver*, double> receiver_list = sender.receiver_preferences_.get_preferences();
    for(auto& receiver : receiver_list) {
        ReceiverType type = receiver.first->get_receiver_type();
        std::string receiver_type;
        if (type == ReceiverType::Worker) {
            receiver_type = "worker";
        } else {
            receiver_type = "store";
        }
        ElementID receiver_id_int = receiver.first->get_id();
        std::string receiver_id = std::to_string(receiver_id_int);

        std::string receiver_string = receiver_type + "-" + receiver_id;

        std::pair<std::string, std::string> pair = std::make_pair(sender_string, receiver_string);
        result.insert(result.end(), pair);
    }

    return result;
}

std::list<std::pair<std::string, std::string>> make_pairs(const Worker& sender){
    std::list<std::pair<std::string, std::string>> result;

    std::string sender_type = "worker";

    std::string sender_id = std::to_string(sender.get_id());

    std::string sender_string = sender_type + "-" + sender_id;

    std::map<IPackageReceiver*, double> receiver_list = sender.receiver_preferences_.get_preferences();
    for(auto& receiver : receiver_list) {
        ReceiverType type = receiver.first->get_receiver_type();
        std::string receiver_type;
        if (type == ReceiverType::Worker) {
            receiver_type = "worker";
        } else {
            receiver_type = "store";
        }
        ElementID receiver_id_int = receiver.first->get_id();
        std::string receiver_id = std::to_string(receiver_id_int);

        std::string receiver_string = receiver_type + "-" + receiver_id;

        std::pair<std::string, std::string> pair = std::make_pair(sender_string, receiver_string);
        result.insert(result.end(), pair);
    }

    return result;
}

void save_factory_structure(Factory& factory, std::ostream& os){
    os << "; == LOADING RAMPS ==" << std::endl;
    os.put(os.widen('\n'));

    for(auto i = factory.ramp_cbegin(); i != factory.ramp_cend();i++){
        os << "LOADING_RAMP id=" << (*i).get_id() << " delivery-interval=" << (*i).get_delivery_interval() << "\n";
    }
    os << "; == WORKERS ==\n\n" << std::endl;
    for(auto i = factory.worker_cbegin(); i != factory.worker_cend();i++){
        std::string type;
        if ((*i).get_queue()->get_queue_type() == PackageQueueType::FIFO) {
            type = "FIFO";
        } else if ((*i).get_queue()->get_queue_type() == PackageQueueType::LIFO) {
            type = "LIFO";
        }
        os << "WORKER id=" << (*i).get_id() << " processing-time=" << (*i).get_processing_duration() << " queue-type=" << type << "\n";
    }
    os << "; == STOREHOUSES ==\n\n" << std::endl;
    for(auto i = factory.storehouse_cbegin(); i != factory.storehouse_cend();i++){
        os << "STOREHOUSE id=" << (*i).get_id() << "\n";
    }
    os << "; == LINKS ==\n\n" << std::endl;
    // Musi uwzgledniac polaczenie rampa-worker, worker-worker, worker-storehouse

    //najpierw rampy!!
    std::list<std::pair<std::string, std::string>> links;
    for (auto ramp = factory.ramp_cbegin(); ramp != factory.ramp_cend();ramp++) {
        links.merge(make_pairs(*ramp));
    }

    //potem robotnicy
    for (auto worker = factory.worker_cbegin(); worker != factory.worker_cend();worker++) {
        links.merge(make_pairs(*worker));
    }

    //zapisz do pliku format: "LINK src=worker-2 dest=store-1"
    for(auto i = links.cbegin(); i != links.cend(); i++){
        os << "LINK src=" << (*i).first << " dest=" << (*i).second <<"\n";
    }

    

}