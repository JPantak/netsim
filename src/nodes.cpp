#include"nodes.hpp"
#include<memory>

Storehouse::Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d){
    id_ = id;
    d_ = std::move(d);
}