#include "package.hpp"

Package::Package() {
    if (freed_IDs.empty() && assigned_IDs.empty()) {
        id_ = 1;
    } else if (!freed_IDs.empty()) {
        id_ = *freed_IDs.begin();
        freed_IDs.erase((*freed_IDs.begin()));
    } else if (!assigned_IDs.empty()) {
        id_ = *assigned_IDs.end() + 1;
    }

    assigned_IDs.insert(id_);
}

Package & Package::operator=(Package && p){
    if(id_ != -1){
        freed_IDs.insert(id_);
        assigned_IDs.erase(id_);
    }
    id_ = p.id_;
    p.id_ = -1;
    return *this;
}

Package::Package(Package && p){
    id_ = p.id_;
    p.id_ = -1;
}

Package::~Package(){
    if(id_==-1){
        assigned_IDs.erase(id_);
        freed_IDs.insert(id_);
    }
}
