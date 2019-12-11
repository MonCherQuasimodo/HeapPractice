#ifndef STDHEAP_H
#define STDHEAP_H

#include <set>

#include "iheap.h"

template <typename T, class TLess = IsLess<T>>
class STDHeap : public IHeap <T, TLess>
{
public:
    STDHeap();
    STDHeap(T key);
    virtual ~STDHeap();

    virtual void insert(T key);
    virtual void meld(IHeap<int, TLess>& right);

    virtual T getMin();
    virtual T extractMin();

    virtual std::vector <T> toVector() const;

    virtual bool empty();
private:
    using mSetIter = typename std::multiset <T, TLess>::iterator;
    std::multiset <T, TLess> heap_;
};

///____________________Constructor_Destructor____________________///
template <typename T, class TLess>
STDHeap<T, TLess>::STDHeap(){}

template <typename T, class TLess>
STDHeap<T, TLess>::STDHeap(T key)
{
    heap_.insert(key);
}

template <typename T, class TLess>
STDHeap<T, TLess>::~STDHeap(){}

///____________________Virtual_methods___________________________///
template <typename T, class TLess>
void STDHeap<T, TLess>::insert(T key)
{
    heap_.insert(key);
}

template <typename T, class TLess>
void STDHeap<T, TLess>::meld(IHeap <int, TLess>& right)
{
    STDHeap<T, TLess> *temp = dynamic_cast<STDHeap<T, TLess>*> (&right);
    if (this == temp){
        return;
    }
    mSetIter it = temp->heap_.begin();
    while (it != temp->heap_.end()){
        this->insert(*it);
        it++;
    }
    temp->heap_.clear();
}

template <typename T, class TLess>
T STDHeap<T, TLess>::getMin()
{
    if (this->empty()){
        throw "Request to an empty STDHeap";
    }
    return *heap_.begin();
}

template <typename T, class TLess>
T STDHeap<T, TLess>::extractMin()
{
    if (this->empty()){
        throw "Request to an empty STDHeap";
    }
    T ans = this->getMin();
    heap_.erase(heap_.begin());
    return ans;
}

template <typename T, class TLess>
bool STDHeap<T, TLess>::empty()
{
    return (heap_.size() ? false : true);
}

template <typename T, class TLess>
std::vector <T> STDHeap<T, TLess>::toVector() const
{
    std::vector <T> ret;
    STDHeap temp = *this;
    while(!temp.empty()){
        ret.push_back(temp.extractMin());
    }
    return ret;
}
#endif /* STDHEAP_H */
