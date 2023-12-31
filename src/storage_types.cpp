#include "storage_types.hpp"

Package PackageQueue::pop(){
    Package elem;
    if(QueueType == PackageQueueType::LIFO){
        elem = std::move(Queue.back());
        Queue.pop_back();
        return elem;
    }
    elem = std::move(Queue.front());
    Queue.pop_front();
    return elem;
}

PackageQueue::~PackageQueue(){

}