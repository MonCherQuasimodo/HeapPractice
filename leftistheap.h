#ifndef LeftistHeap_H
#define LeftistHeap_H

#include "iheap.h"

///____________________Leftist_Heap______________________________///
template <typename T, class TLess = IsLess<T>>
class LeftistHeap : public IHeap <T, TLess>
{
public:
    LeftistHeap();
    LeftistHeap(T key);
    LeftistHeap(const LeftistHeap<T, TLess>& right);
    virtual ~LeftistHeap();

    LeftistHeap<T, TLess> & operator=(const LeftistHeap<T, TLess>& right);

    virtual void insert(T key);
    virtual void meld(IHeap<int, TLess>& right);

    virtual T getMin();
    virtual T extractMin();

    virtual std::vector <T> toVector() const;

    virtual bool empty();
private:
    struct Node_
    {
        T key;
        Node_* left;
        Node_* right;
        size_t dist;

        Node_();
        Node_(T key_);
        Node_(const Node_& node);
        Node_& operator=(const Node_& right);
        ~Node_();

        static Node_* meld(Node_* first, Node_* second);
        static bool isLess(T& first, T& second);
        static size_t getDist(Node_* Node_);
    };
    Node_* root_;
};

////////////////////////////////////////////////////////////////////
///____________________Struct_Node_______________________________///
////////////////////////////////////////////////////////////////////

///____________________Constructor_Destructor____________________///
template <typename T, class TLess>
LeftistHeap<T, TLess>::Node_::Node_() :
    left(nullptr),
    right(nullptr),
    dist(0)
{}

template <typename T, class TLess>
LeftistHeap<T, TLess>::Node_::Node_(T key_) :
    key(key_),
    left(nullptr),
    right(nullptr),
    dist(1)
{}

template <typename T, class TLess>
LeftistHeap<T, TLess>::Node_::Node_(const Node_& node) :
    key(node.key),
    left(nullptr),
    right(nullptr),
    dist(node.dist)
{
    if (node.left){
        left = new Node_(*(node.left));
    }
    if (node.right){
        right = new Node_(*(node.right));
    }
}

template <typename T, class TLess>
LeftistHeap<T, TLess>::Node_::~Node_()
{
    delete left;
    delete right;
    left = nullptr;
    right = nullptr;
};

template <typename T, class TLess>
typename LeftistHeap<T, TLess>::Node_& LeftistHeap<T, TLess>::Node_::operator=(const Node_& node)
{
    if (this == &node) {
        return *this;
    }

    delete left;
    delete right;
    left = nullptr;
    right = nullptr;

    this->key = node.key;
    this->dist = node.dist;

    if (node.right){
        this->right = new Node_(*node.right);
    }
    if (node.left){
        this->left = new Node_(*node.left);
    }
    return *this;
}

///____________________Other_methods_____________________________///
template <typename T, class TLess>
typename LeftistHeap<T, TLess>::Node_* LeftistHeap<T, TLess>::Node_::meld(LeftistHeap<T, TLess>::Node_* first,
                                                                          LeftistHeap<T, TLess>::Node_* second)
{
    if (first == nullptr){
        return second;
    }
    if (second == nullptr){
        return first;
    }
    if (Node_::isLess(second->key, first->key)){
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
bool LeftistHeap<T, TLess>::Node_::isLess(T& first, T& second)
{
    TLess comp;
    return comp(first, second);
}

template <typename T, class TLess>
size_t LeftistHeap<T, TLess>::Node_::getDist(Node_* node)
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
    root_(new Node_(key))
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
        root_ = new Node_(*right.root_);
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
        root_ = new Node_(*right.root_);
    }
    return *this;
}

///____________________Virtual_methods___________________________///
template <typename T, class TLess>
void LeftistHeap<T, TLess>::insert(T key)
{
    LeftistHeap<T, TLess>* add = new LeftistHeap(key);
    this->meld(*add);
}

template <typename T, class TLess>
void LeftistHeap<T, TLess>::meld(IHeap <int, TLess>& right_)
{
    LeftistHeap<T, TLess>* right = dynamic_cast<LeftistHeap<T, TLess>*> (&right_);
    if (this == &right_){
        return;
    }
    this->root_ = Node_::meld(this->root_, right->root_);
    right->root_ = nullptr;
}

template <typename T, class TLess>
T LeftistHeap<T, TLess>::getMin()
{
    if (this->empty()){
        throw "Request to an empty heap";
    }
    return this->root_->key;
}

template <typename T, class TLess>
T LeftistHeap<T, TLess>::extractMin()
{
    if (this->empty()){
        throw "Request to an empty heap";
    }

    T minValue = this->getMin();
    Node_* left = this->root_->left;
    Node_* right = this->root_->right;

    this->root_->left = nullptr;
    this->root_->right = nullptr;
    delete this->root_;
    this->root_ = Node_::meld(left, right);

    return minValue;
}

template <typename T, class TLess>
bool LeftistHeap<T, TLess>::empty()
{
    return (this->root_ ? false : true);
}

template <typename T, class TLess>
std::vector <T> LeftistHeap<T, TLess>::toVector() const
{
    std::vector <T> ret;
    LeftistHeap<T, TLess> temp = *this;
    while(!temp.empty()){
        ret.push_back(temp.extractMin());
    }
    return ret;
}
#endif /* LeftistHeap_H */
