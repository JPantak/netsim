//
// Created by mmich on 28.11.2023.
//

#ifndef IMPLEMENTATION_NETSIM_NODES_HPP
#define IMPLEMENTATION_NETSIM_NODES_HPP

#include <iostream>
#include <memory>
#include <map>
#include "types.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"
#include "optional"

enum class ReceiverType {
    Worker,
    Storehouse
};

class IPackageReceiver  {
public:
    IPackageReceiver() = default;

    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;

    virtual void receive_package(Package &&p) = 0;

    virtual ElementID get_id() const = 0;


    virtual ReceiverType get_receiver_type() const = 0;


    virtual ~IPackageReceiver() = default;
private:


};

class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
    ReceiverPreferences(ProbabilityGenerator pg = probability_generator): pg_(std::move(pg)){};
    const_iterator begin() const {return preferences_.cbegin();}
    const_iterator end() const {return preferences_.cend();};
    const_iterator cbegin() const {return preferences_.cbegin();}
    const_iterator cend() const  {return preferences_.cend();};
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver(void);
    const preferences_t& get_preferences(void) const  {return preferences_;};
private:
    ProbabilityGenerator pg_;
    preferences_t preferences_;

};


class PackageSender {
public:
    ReceiverPreferences receiver_preferences_;

    PackageSender() = default;

    PackageSender(PackageSender &&p_sender) = default;

    void send_package();

    const std::optional<Package> &get_sending_buffer() const {return buffer_;}
private:
    std::optional<Package> buffer_ = std::nullopt;
protected:
    void push_package(Package &&p) {buffer_.emplace(p.get_id()); }
};


class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di) : PackageSender(), id_(id), di_(di) {};

    void deliver_goods(Time t);

    TimeOffset get_delivery_interval() const {return di_;};

    ElementID get_id() const {return id_;}
private:
    ElementID id_;
    TimeOffset di_;
    Time t_;
    std::optional<Package> buffer_ = std::nullopt;
};

class Worker : public PackageSender, public IPackageReceiver{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : PackageSender(), id_(id), pd_(pd), q_(std::move(q)){};

    void do_work(Time t);

    IPackageQueue* get_queue(void) const {return q_.get();}

    TimeOffset get_processing_duration() const {return pd_;}

    Time get_package_processing_start_time() const {return t_;}

    void receive_package(Package&& p) override {q_ -> push(std::move(p));}

    ElementID get_id() const override {return id_;}

    std::optional<Package>& get_processing_buffer(void) {return buffer_;}

    IPackageStockpile::const_iterator cbegin() const override {return q_->cbegin();}

    IPackageStockpile::const_iterator cend() const override {return q_->cend();}

    IPackageStockpile::const_iterator begin() const override {return q_->begin();}

    IPackageStockpile::const_iterator end() const override {return q_->end();}

    ReceiverType get_receiver_type() const override { return ReceiverType::Worker; }


private:
    ElementID id_;
    TimeOffset pd_;
    Time t_;
    std::unique_ptr<IPackageQueue> q_;
    std::optional<Package> buffer_ = std::nullopt;
};

class Storehouse: public IPackageReceiver{
    public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO)) : id_(id), d_(std::move(d)) {};
    void receive_package(Package &&p) override {d_->push(std::move(p));};
    ElementID get_id() const override { return id_; }
    IPackageStockpile::const_iterator begin() const override {return d_->cbegin();}
    IPackageStockpile::const_iterator end() const override {return d_->cend();};
    IPackageStockpile::const_iterator cbegin() const override {return d_->cbegin();}
    IPackageStockpile::const_iterator cend() const override {return d_->cend();}
    //~Storehouse() = default;

    ReceiverType get_receiver_type() const override { return ReceiverType::Storehouse; }

    private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};

#endif //IMPLEMENTATION_NETSIM_NODES_HPP
