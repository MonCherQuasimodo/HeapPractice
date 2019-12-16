#ifndef LeftistHeap_H
#define LeftistHeap_H

#include "iheap.h"
#include "obliqueheap.h"

///____________________Leftist_Heap______________________________///
template <typename T, class TLess = IsLess<T>>
class LeftistHeap : public ObliqueHeap<T, TLess>
{
public:
    LeftistHeap();
    LeftistHeap(T key);
    LeftistHeap(const LeftistHeap<T, TLess>& right);
    ~LeftistHeap() override;

    LeftistHeap<T, TLess> & operator=(const LeftistHeap<T, TLess>& right);
protected:
    struct _Node : ObliqueHeap<T, TLess>::Node_
    {
        size_t dist;

        _Node();
        _Node(T key_);
        _Node(const _Node& node);
        _Node& operator=(const _Node& right);
        ~_Node();

        static _Node* meld(_Node* first, _Node* second);
        static size_t getDist(_Node* _Node);
    };
    _Node* root_;
};

////////////////////////////////////////////////////////////////////
///____________________Struct__Node______________________________///
////////////////////////////////////////////////////////////////////

///____________________Constructor_Destructor____________________///
template <typename T, class TLess>
LeftistHeap<T, TLess>::_Node::_Node()
{}

template <typename T, class TLess>
LeftistHeap<T, TLess>::_Node::_Node(T key_) :
    dist(1)
{}

template <typename T, class TLess>
LeftistHeap<T, TLess>::_Node::_Node(const _Node& node) :
    dist(node.dist)
{}

template <typename T, class TLess>
LeftistHeap<T, TLess>::_Node::~_Node()
{}

template <typename T, class TLess>
typename LeftistHeap<T, TLess>::_Node& LeftistHeap<T, TLess>::_Node::operator=(const _Node& node)
{
    this->dist = node.dist;
}

///____________________Other_methods_____________________________///
template <typename T, class TLess>
typename LeftistHeap<T, TLess>::_Node* LeftistHeap<T, TLess>::_Node::meld(LeftistHeap<T, TLess>::_Node* first,
                                                                          LeftistHeap<T, TLess>::_Node* second)
{
    if (first == nullptr){
        return second;
    }
    if (second == nullptr){
        return first;
    }
    if (_Node::isLess(second->key, first->key)){
        std::swap(first, second);
    }
    first->right = meld(first->right, second);
    if (getDist(first->right) > getDist(first->left)){
        std::swap(first->right, first->left);
    }
    first->dist = getDist(first->right) + 1;
    return first;
}

template <typename T, class TLess>
size_t LeftistHeap<T, TLess>::_Node::getDist(_Node* node)
{
    return (node ? node->dist : 0);
}
////////////////////////////////////////////////////////////////////
///____________________Class_LeftistHeap_________________________///
////////////////////////////////////////////////////////////////////

///____________________Constructor_Destructor____________________///

template <typename T, class TLess>
LeftistHeap<T, TLess>::LeftistHeap() :
    root_(nullptr)
{}

template <typename T, class TLess>
LeftistHeap<T, TLess>::LeftistHeap(T key) :
    root_(new _Node(key))
{}

template <typename T, class TLess>
LeftistHeap<T, TLess>::~LeftistHeap()
{
    delete root_;
    root_ = nullptr;
}

template <typename T, class TLess>
LeftistHeap<T, TLess>::LeftistHeap(const LeftistHeap<T, TLess>& right) :
    root_(nullptr)
{
    if (right.root_){
        root_ = new _Node(*right.root_);
    }
}

template <typename T, class TLess>
LeftistHeap<T, TLess>& LeftistHeap<T, TLess>::operator=(const LeftistHeap<T, TLess>& right)
{
    if (this == &right) {
        return *this;
    }
    delete root_;
    root_ = nullptr;
    if (right.root_){
        root_ = new _Node(*right.root_);
    }
    return *this;
}

#endif /* LeftistHeap_H */
