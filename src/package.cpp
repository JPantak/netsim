#include "package.hpp"

std::set<ElementID> Package::assigned_IDs = {};
std::set<ElementID> Package::freed_IDs = {};

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


Package & Package::operator=(Package && p) noexcept {
    if (this == &p) return *this;

    if(id_ != -1){
        freed_IDs.insert(this->id_);
        assigned_IDs.erase(this->id_);
    }

    this->id_ = p.id_;
    assigned_IDs.insert(this->id_);

    p.id_ = -1;

    return *this;

}

Package::Package(Package && p) noexcept{
    id_ = p.id_;
    p.id_ = -1;
}

Package::~Package(){
    if(id_!=-1){
        assigned_IDs.erase(id_);
        freed_IDs.insert(id_);
    }

}
