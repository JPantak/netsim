#ifndef IMPLEMENTATION_NETSIM_FACTORY_HPP
#define IMPLEMENTATION_NETSIM_FACTORY_HPP

#include <vector>
#include <algorithm>
#include "types.hpp"

template<typename Node>
class NodeCollection {
public:
    using container_t = typename std::vector<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node&& node) {node_ -> std::move(node);}

    iterator find_by_id(ElementID id) {return std::find_if(container_.begin(), container_.end(),
                                                           [id](ElementID i){return (i == id);});};

    const_iterator find_by_id(ElementID id) const {return std::find_if(container_.begin(), container_.end(),
                                                                [id](ElementID i){return (i == id);});};

    void remove_by_id(ElementID id) {
        auto iter = find_by_id(id);
        if (iter != container_.end()){
            container_.erase(iter);
        }
    }

    iterator begin() {return container_.begin();}
    iterator end() {return container_.end();}
    const_iterator cbegin() const {return container_.cbegin();}
    const_iterator cend() const {return container_.cend();}
    const_iterator begin() const {return container_.begin();}
    const_iterator end() const {return container_.end();}
private:
    Node node_;
    container_t container_;
};

class Factory{
public:
    void add_ramp(Ramp&& ramp) {Ramps.add(std::move(ramp));}
    void remove_ramp(ElementID id) {Ramps.remove_by_id(id);}

    void add_worker(Worker&& worker) {Workers.add(std::move(worker));}
    void remove_worker(ElementID id) {Workers.remove_by_id(id);}

    void add_storehouse(Storehouse&& storehouse){Storehouses.add(std::move(storehouse));}
    void remove_storehouse(ElementID id){Storehouses.remove_by_id(id);}

    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return Ramps.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {return Ramps.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator ramp_cbegin() {return Ramps.cbegin();}
    NodeCollection<Ramp>::const_iterator ramp_cend() {return Ramps.end();}

    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id){return Workers.find_by_id(id);}
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const {return Workers.find_by_id(id);}
    NodeCollection<Worker>::const_iterator worker_cbegin() {return Workers.cbegin();}
    NodeCollection<Worker>::const_iterator worker_cend() {return Workers.cend();}

    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id){return Storehouses.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const {return Storehouses.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() {return Storehouses.cbegin();}
    NodeCollection<Storehouse>::const_iterator storehouse_cend() {return Storehouses.cend();}
    
    bool is_consistent(void);
    void do_delivieries(Time t);
    void do_package_passing(void);
    void do_work(Time t);

private:
    void remove_receiver(NodeCollection<Ramp>& collection, ElementID id) {collection.remove_by_id(id);}
    void remove_receiver(NodeCollection<Worker>& collection, ElementID id) {collection.remove_by_id(id);}
    void remove_receiver(NodeCollection<Storehouse>& collection, ElementID id) {collection.remove_by_id(id);}
    NodeCollection<Ramp> Ramps;
    NodeCollection<Worker> Workers;
    NodeCollection<Storehouse> Storehouses;
};

#endif
