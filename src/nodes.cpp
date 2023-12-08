#include"nodes.hpp"
#include<memory>

Storehouse::Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d){
    id_ = id;
    d_ = std::move(d);
}

void PackageSender::send_package() {
    //@TODO
}

void ReceiverPreferences::add_receiver(IPackageReceiver* r){
    double prob = 1.0;
    preferences.insert(std::make_pair(r,prob));
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r){
    preferences.erase(r);
}
// IPackageReceiver* choose_receiver(void){

// }