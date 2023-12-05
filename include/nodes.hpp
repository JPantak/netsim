//
// Created by mmich on 28.11.2023.
//

#ifndef IMPLEMENTATION_NETSIM_NODES_HPP
#define IMPLEMENTATION_NETSIM_NODES_HPP

#include <iostream>
#include <memory>
#include "types.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"
#include "optional"

class Ramp : public PackageSender{
public:
    Ramp(ElementID id, TimeOffset di) : id_(id), di_(di) {};

    void deliver_goods(Time t);

    TimeOffset get_delivery_interval(void) const {return di_;};

    ElementID get_id(void) const {return id_;}
private:
    ElementID id_;
    TimeOffset di_;
};

class Worker{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : id_(id), pd_(pd), q_(std::move(q)){};

    void do_work(Time t);

    TimeOffset get_processing_duration(void) const {return pd_;}

    //Time get_package_processing_start_time(void) const {return }

private:
    ElementID id_;
    TimeOffset pd_;
    std::unique_ptr<IPackageQueue> q_;
};
class Storehouse: public IPackageStockpile{
    public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d);
    private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};

class PackageSender {
public:
    //ReceiverPreferences receiver_preferences_; @TODO

    PackageSender(PackageSender &&p_sender) = default;

    void send_package();

    const std::optional<Package> &get_sending_buffer() const {return buffer_;}
private:
    std::optional<Package> buffer_ = std::nullopt;
protected:
    void push_package(Package &&p) {buffer_.emplace(p.get_id()); }
};

#endif //IMPLEMENTATION_NETSIM_NODES_HPP
