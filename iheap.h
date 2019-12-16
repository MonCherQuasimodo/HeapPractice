#ifndef IHEAP_H
#define IHEAP_H

#include <iostream>
#include <vector>

namespace typesHeap
{
    enum types {STD = 0, BINOMIAL = 1, LEFTIST = 2, OBLIQUE = 3};
    std::vector <std::string> toString = {"STD", "BINOMIAL", "LEFTIST", "OBLIQUE"};
}

template <typename T>
class IsLess
{
public:
    bool operator()(const T& left, const T& right) const
  	{
  		return left < right;
  	}
};

template <typename T, class TLess = IsLess<T>>
class IHeap
{
public:
    virtual ~IHeap() {}

    virtual void insert(T key) = 0;
    virtual void meld(IHeap& right) = 0;

    virtual T getMin() const = 0;

    virtual T extractMin() = 0;
    virtual std::vector <T> toVector() const = 0;
    virtual bool empty() const = 0;
};

#endif /* IHEAP_H */
