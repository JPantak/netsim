//
// Created by mmich on 28.11.2023.
//

#ifndef IMPLEMENTATION_NETSIM_STORAGE_TYPES_HPP
#define IMPLEMENTATION_NETSIM_STORAGE_TYPES_HPP

#include <list>
#include "package.hpp"

class IPackageStockpile {
public:
    virtual void push(Package&& package) = 0;
    virtual bool empty() const = 0;
    virtual std::size_t size() const = 0;

    using const_iterator = std::list<Package>::const_iterator;
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
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() const = 0;

    ~IPackageQueue() override = default;
};

class PackageQueue : public  IPackageQueue {
public:
    //@TODO:
private:
    //TODO:
};

#endif //IMPLEMENTATION_NETSIM_STORAGE_TYPES_HPP
