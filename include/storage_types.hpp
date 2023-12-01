#ifndef IMPLEMENTATION_NETSIM_STORAGE_TYPES_HPP
#define IMPLEMENTATION_NETSIM_STORAGE_TYPES_HPP

#include <list>
#include "package.hpp"
using const_iterator = std::list<Package>::const_iterator;


class IPackageStockpile {
public:
    virtual void push(Package&& package) = 0;
    virtual bool empty() const = 0;
    virtual std::size_t size() const = 0;

    
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;

    virtual ~IPackageStockpile() = default;

};

enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageQueue : public IPackageStockpile {
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() const = 0;

    ~IPackageQueue() override = default;
};

class PackageQueue : public  IPackageQueue {
public:
    PackageQueue(PackageQueueType Queue): QueueType(Queue){}
    PackageQueueType get_queue_type(){return QueueType;}
    size_t size(){return Queue.size();}
    bool empty(){return Queue.empty();}
    const_iterator begin() const {return Queue.begin();}
    const_iterator end() const {return Queue.end();}
    const_iterator cbegin() const {return Queue.cbegin();}
    const_iterator cend() const {return Queue.cend();}
    Package pop();
    void push(Package&& package){Queue.push_back(std::move(package));}
    ~PackageQueue();
private:
    PackageQueueType QueueType;
    std::list<Package> Queue;

};

#endif //IMPLEMENTATION_NETSIM_STORAGE_TYPES_HPP
