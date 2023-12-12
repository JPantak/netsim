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

#endif
