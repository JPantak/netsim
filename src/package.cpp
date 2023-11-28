//
// Created by mmich on 28.11.2023.
//

#include "package.hpp"

Package::Package(Package && p){
    id_ = p.id_;
    p.id_ = -1;
}

Package & Package::operator=(Package && p){
    if(id_ != -1){
        freed_IDs.insert(id_);
        assigned_IDs.erase(id_);
    }
    id_ = p.id_;
    p.id_ = -1;
}