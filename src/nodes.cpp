#include"nodes.hpp"
#include "helpers.hpp"
#include<memory>


void PackageSender::send_package() {
    IPackageReceiver *receiver;

    if(buffer_) {
        receiver = receiver_preferences_.choose_receiver();
        receiver->receive_package(std::move(*buffer_));
        buffer_.reset();
    }
}

void ReceiverPreferences::add_receiver(IPackageReceiver* r){
    double prob = 1.0/((double)preferences_.size()+1);
    for(auto& key : preferences_){
        key.second = prob;
    }
    preferences_.insert(std::make_pair(r,prob));
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r){
    preferences_.erase(r);
    double prob = 1.0/((double)preferences_.size());
    
    if(preferences_.size() == 1)
    for(auto& key : preferences_){
        key.second = prob;
    }
}
IPackageReceiver* ReceiverPreferences::choose_receiver(void){
    double prob = pg_();
    double prob_sum = 0.0;
    for(auto& key : preferences_){
        if(prob_sum < prob && prob_sum + key.second > prob){
            return key.first;
        }
        prob_sum += key.second;
    }
    return nullptr;
}


void Ramp::deliver_goods(Time t) {
    if (!buffer_) {
        push_package(Package());
        buffer_.emplace(id_);
        t_ = t;
    }
    else {
        if (t - di_ == t_){
            push_package(Package());
        }
    }
}

void Worker::do_work(Time t) {
    if (!buffer_ && !q_->empty()) {
        buffer_.emplace(q_->pop());
        t_ = t;
    }
    else {
        if (t - pd_ == t_ - 1) {
            push_package(Package(buffer_.value().get_id()));
            buffer_.reset();
            if (!q_->empty()) {
                buffer_.emplace(q_->pop());
            }
        }
    }
}