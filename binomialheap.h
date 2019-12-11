#ifndef BINOMIALHEAP_H
#define BINOMIALHEAP_H

#include "iheap.h"

#include <algorithm>

template <typename T, class TLess = IsLess<T>>
class BinomialHeap : public IHeap <T, TLess>
{
public:
    BinomialHeap();
    BinomialHeap(T key);
    BinomialHeap(const BinomialHeap<T, TLess>& right);
    virtual ~BinomialHeap();

    BinomialHeap<T, TLess> & operator=(const BinomialHeap<T, TLess>& right);

    virtual void insert(T key);
    virtual void meld(IHeap<int, TLess>& right);

    virtual T getMin();
    virtual T extractMin();

    virtual std::vector <T> toVector() const;

    virtual bool empty();
private:
    struct BinomialTree_
    {
        T key;
        BinomialTree_* child;
        BinomialTree_* rightBrother;
        size_t degree;

        BinomialTree_();
        BinomialTree_(T key_);
        BinomialTree_(const BinomialTree_& node);
        BinomialTree_& operator=(const BinomialTree_& right);
        ~BinomialTree_();

        static BinomialTree_* combine(BinomialTree_* first,
                                      BinomialTree_* second);
        static int getDegree(BinomialTree_* tree);
        static bool isLess(T& first, T& second);
    };

    std::vector <BinomialTree_*> roots_;
    using vIter = typename std::vector<BinomialTree_*>::iterator;
    void meldTree_(size_t it, BinomialTree_* tree, BinomialTree_*& carry);
    void clearLeadingNullptr_();
};

////////////////////////////////////////////////////////////////////
///____________________Struct_Binomial_Tree______________________///
////////////////////////////////////////////////////////////////////

///____________________Constructor_Destructor____________________///
template <typename T, class TLess>
BinomialHeap<T, TLess>::BinomialTree_::BinomialTree_() :
    child(nullptr),
    rightBrother(nullptr),
    degree(0)
{}

template <typename T, class TLess>
BinomialHeap<T, TLess>::BinomialTree_::BinomialTree_(T key_) :
    key(key_),
    child(nullptr),
    rightBrother(nullptr),
    degree(0)
{}

template <typename T, class TLess>
BinomialHeap<T, TLess>::BinomialTree_::BinomialTree_(const BinomialTree_& node) :
    key(node.key),
    child(nullptr),
    rightBrother(nullptr),
    degree(node.degree)
{
    if (node.child){
        child = new BinomialTree_(*(node.child));
    }
    if (node.rightBrother){
        rightBrother = new BinomialTree_(*(node.rightBrother));
    }
}

template <typename T, class TLess>
BinomialHeap<T, TLess>::BinomialTree_::~BinomialTree_()
{
    delete child;
    delete rightBrother;
    child = nullptr;
    rightBrother = nullptr;
}

template <typename T, class TLess>
typename BinomialHeap<T, TLess>::BinomialTree_& BinomialHeap<T, TLess>::BinomialTree_::operator=(const BinomialTree_& node)
{
    if (this == &node) {
        return *this;
    }

    delete child;
    delete rightBrother;
    child = nullptr;
    rightBrother = nullptr;

    this->key = node.key;
    this->degree = node.degree;

    if (node.rightBrother){
        this->rightBrother = new BinomialTree_(*node.rightBrother);
    }
    if (node.child){
        this->child = new BinomialTree_(*node.child);
    }
    return *this;
}

///____________________Other_methods_____________________________///
template <typename T, class TLess>
typename BinomialHeap<T, TLess>::BinomialTree_* BinomialHeap<T, TLess>::BinomialTree_::combine(BinomialTree_* first,
                                                                                               BinomialTree_* second)
{
    if (first == nullptr){
        return second;
    }
    if (second == nullptr){
        return first;
    }
    if (first->degree != second->degree){
        throw "Combine different-sizes BinomialTrees";
    }
    BinomialTree_* higher = first;
    BinomialTree_* lower = second;
    if (BinomialTree_::isLess(second->key, first->key)){
        std::swap(higher, lower);
    }

    lower->rightBrother = higher->child;
    higher->child = lower;
    higher->degree++;

    return higher;
}

template <typename T, class TLess>
int BinomialHeap<T, TLess>::BinomialTree_::getDegree(BinomialTree_* tree)
{
    return (tree ? static_cast<int> (tree->degree) : -1);
}

template <typename T, class TLess>
bool BinomialHeap<T, TLess>::BinomialTree_::isLess(T& first, T& second)
{
    TLess comp;
    return comp(first, second);
}

////////////////////////////////////////////////////////////////////
///____________________Class_BinomialHeap________________________///
////////////////////////////////////////////////////////////////////

///____________________Constructor_Destructor____________________///
template <typename T, class TLess>
BinomialHeap<T, TLess>::BinomialHeap(){}

template <typename T, class TLess>
BinomialHeap<T, TLess>::BinomialHeap(T key) :
    roots_({new BinomialTree_(key)})
{}

template <typename T, class TLess>
BinomialHeap<T, TLess>::BinomialHeap(const BinomialHeap<T, TLess>& right)
{
    roots_.resize(right.roots_.size(), nullptr);
    for (size_t i = 0; i < roots_.size(); ++i){
        if (right.roots_[i])
            roots_[i] = new BinomialTree_(*right.roots_[i]);
    }
}

template <typename T, class TLess>
BinomialHeap<T, TLess>::~BinomialHeap()
{
    for (size_t i = 0; i < roots_.size(); ++i){
        delete roots_[i];
        roots_[i] = nullptr;
    }
    roots_.clear();
}

template <typename T, class TLess>
BinomialHeap<T, TLess>& BinomialHeap<T, TLess>::operator=(const BinomialHeap<T, TLess>& right)
{
    if (this == &right) {
        return *this;
    }
    for (size_t i = 0; i < roots_.size(); ++i){
        delete roots_[i];
        roots_[i] = nullptr;
    }
    roots_.resize(right.roots_.size(), nullptr);
    for (size_t i = 0; i < roots_.size(); ++i){
        if (right.roots_[i])
            roots_[i] = new BinomialTree_(*right.roots_[i]);
    }
    return *this;
}

///____________________Virtual_methods___________________________///
template <typename T, class TLess>
void BinomialHeap<T, TLess>::insert(T key)
{
    BinomialHeap<T, TLess>* add = new BinomialHeap(key);
    this->meld(*add);
}

template <typename T, class TLess>
void BinomialHeap<T, TLess>::meld(IHeap <int, TLess>& right_)
{
    BinomialHeap<T, TLess>* right = dynamic_cast<BinomialHeap<T, TLess>*> (&right_);
    if (this == right){
        return;
    }
    BinomialTree_* carry = nullptr;
    for (size_t it = 0; it < std::max(this->roots_.size(), right->roots_.size()) || carry; ++it){
        if (it == this->roots_.size()){
            this->roots_.push_back(nullptr);
        }
        this->meldTree_(it, carry, carry);
        BinomialTree_* rightTree = (right->roots_.size() > it ? right->roots_[it] : nullptr);
        this->meldTree_(it, rightTree, carry);
    }
    for (size_t i = 0; i < right->roots_.size(); ++i){
        right->roots_[i] = nullptr;
    }
    right->roots_.clear();
}

template <typename T, class TLess>
T BinomialHeap<T, TLess>::getMin()
{
    if (this->empty()){
        throw "Request to an empty heap";
    }
    BinomialTree_* curMin = nullptr;
    for (size_t it = 0; it < roots_.size(); ++it){
        if (!roots_[it]){
            continue;
        }
        if (!curMin){
            curMin = roots_[it];
            continue;
        }
        curMin = (BinomialTree_::isLess(roots_[it]->key, curMin->key) ? roots_[it] : curMin);
    }
    return curMin->key;
}

template <typename T, class TLess>
T BinomialHeap<T, TLess>::extractMin()
{
    if (this->empty()){
        throw "Request to an empty heap";
    }
    T minValue = this->getMin();
    vIter minIterator = this->roots_.begin();
    for (vIter it = minIterator; it != this->roots_.end(); ++it){
        if (*it && (*it)->key == minValue){
            minIterator = it;
            break;
        }
    }
    BinomialHeap<T, TLess>* add = new BinomialHeap<T, TLess>();
    add->roots_.reserve(static_cast<size_t>(BinomialTree_::getDegree(*minIterator)));
    BinomialTree_* cur = (*minIterator)->child;
    while (cur) {
        add->roots_.push_back(cur);
        cur = cur->rightBrother;
        add->roots_.back()->rightBrother = nullptr;
    }
    std::reverse(add->roots_.begin(), add->roots_.end());

    (*minIterator)->child = nullptr;
    (*minIterator)->rightBrother = nullptr;
    delete (*minIterator);
    *minIterator = nullptr;

    this->meld(*add);
    this->clearLeadingNullptr_();

    return minValue;
}

template <typename T, class TLess>
bool BinomialHeap<T, TLess>::empty()
{
    return (roots_.size() ? false : true);
}

template <typename T, class TLess>
std::vector <T> BinomialHeap<T, TLess>::toVector() const
{
    std::vector <T> ret;
    BinomialHeap<T, TLess> temp = *this;
    while(!temp.empty()){
        ret.push_back(temp.extractMin());
    }
    return ret;
}

///____________________Private_methods___________________________///
template <typename T, class TLess>
void BinomialHeap<T, TLess>::meldTree_(size_t it, BinomialTree_* tree, BinomialTree_*& carry)
{
    this->roots_[it] = BinomialTree_::combine(this->roots_[it], tree);
    if (BinomialTree_::getDegree(this->roots_[it]) - 1 == static_cast<int> (it)){
        carry = this->roots_[it];
        this->roots_[it] = nullptr;
    } else if (tree == carry){
        carry = nullptr;
    }
}

template <typename T, class TLess>
void BinomialHeap<T, TLess>::clearLeadingNullptr_()
{
    while (!this->roots_.empty() && !this->roots_.back()){
        this->roots_.pop_back();
    }
}

#endif /* BINOMIALHEAP_H */
