#ifndef ObliqueHeap_H
#define ObliqueHeap_H

#include "iheap.h"

///____________________Oblique_Heap______________________________///
template <typename T, class TLess = IsLess<T>>
class ObliqueHeap : public IHeap <T, TLess>
{
public:
    ObliqueHeap();
    ObliqueHeap(T key);
    ObliqueHeap(const ObliqueHeap<T, TLess>& right);
    ~ObliqueHeap() override;

    ObliqueHeap<T, TLess> & operator=(const ObliqueHeap<T, TLess>& right);

    virtual void insert(T key) override;
    virtual void meld(IHeap<int, TLess>& right) override;

    virtual T getMin() const override;
    virtual T extractMin() override;

    virtual std::vector <T> toVector() const;

    virtual bool empty() const override;
private:
    struct Node_
    {
        T key;
        Node_* left;
        Node_* right;

        Node_();
        Node_(T key_);
        Node_(const Node_& node);
        Node_& operator=(const Node_& right);
        ~Node_();

        static Node_* meld(Node_* first, Node_* second);
        static bool isLess(T& first, T& second);
    };
    Node_* root_;
};

////////////////////////////////////////////////////////////////////
///____________________Struct_Node_______________________________///
////////////////////////////////////////////////////////////////////

///____________________Constructor_Destructor____________________///
template <typename T, class TLess>
ObliqueHeap<T, TLess>::Node_::Node_() :
    left(nullptr),
    right(nullptr)
{}

template <typename T, class TLess>
ObliqueHeap<T, TLess>::Node_::Node_(T key_) :
    key(key_),
    left(nullptr),
    right(nullptr)
{}

template <typename T, class TLess>
ObliqueHeap<T, TLess>::Node_::Node_(const Node_& node) :
    key(node.key),
    left(nullptr),
    right(nullptr)
{
    if (node.left){
        left = new Node_(*(node.left));
    }
    if (node.right){
        right = new Node_(*(node.right));
    }
}

template <typename T, class TLess>
ObliqueHeap<T, TLess>::Node_::~Node_()
{
    delete left;
    delete right;
    left = nullptr;
    right = nullptr;
};

template <typename T, class TLess>
typename ObliqueHeap<T, TLess>::Node_& ObliqueHeap<T, TLess>::Node_::operator=(const Node_& node)
{
    if (this == &node) {
        return *this;
    }

    delete left;
    delete right;
    left = nullptr;
    right = nullptr;

    this->key = node.key;

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
typename ObliqueHeap<T, TLess>::Node_* ObliqueHeap<T, TLess>::Node_::meld(ObliqueHeap<T, TLess>::Node_* first,
                                                                          ObliqueHeap<T, TLess>::Node_* second)
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
    std::swap(first->right, first->left);
    return first;
}

template <typename T, class TLess>
bool ObliqueHeap<T, TLess>::Node_::isLess(T& first, T& second)
{
    TLess comp;
    return comp(first, second);
}

////////////////////////////////////////////////////////////////////
///____________________Class_ObliqueHeap_________________________///
////////////////////////////////////////////////////////////////////

///____________________Constructor_Destructor____________________///
template <typename T, class TLess>
ObliqueHeap<T, TLess>::ObliqueHeap() :
    root_(nullptr)
{}

template <typename T, class TLess>
ObliqueHeap<T, TLess>::ObliqueHeap(T key) :
    root_(new Node_(key))
{}

template <typename T, class TLess>
ObliqueHeap<T, TLess>::~ObliqueHeap()
{
    delete root_;
    root_ = nullptr;
}

template <typename T, class TLess>
ObliqueHeap<T, TLess>::ObliqueHeap(const ObliqueHeap<T, TLess>& right) :
    root_(nullptr)
{
    if (right.root_){
        root_ = new Node_(*right.root_);
    }
}

template <typename T, class TLess>
ObliqueHeap<T, TLess>& ObliqueHeap<T, TLess>::operator=(const ObliqueHeap<T, TLess>& right)
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
void ObliqueHeap<T, TLess>::insert(T key)
{
    ObliqueHeap<T, TLess>* add = new ObliqueHeap(key);
    this->meld(*add);
}

template <typename T, class TLess>
void ObliqueHeap<T, TLess>::meld(IHeap <int, TLess>& right_)
{
    ObliqueHeap<T, TLess>* right = nullptr;
    try{
        right = dynamic_cast<ObliqueHeap<T, TLess>*> (&right_);
    } catch (const std::bad_cast &e){
        std::cerr << e.what();
    }
    if (this == &right_){
        return;
    }
    this->root_ = Node_::meld(this->root_, right->root_);
    right->root_ = nullptr;
}

template <typename T, class TLess>
T ObliqueHeap<T, TLess>::getMin() const
{
    if (this->empty()){
        throw "Request to an empty heap";
    }
    return this->root_->key;
}

template <typename T, class TLess>
T ObliqueHeap<T, TLess>::extractMin()
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
bool ObliqueHeap<T, TLess>::empty() const
{
    return this->root_ == nullptr;
}

template <typename T, class TLess>
std::vector <T> ObliqueHeap<T, TLess>::toVector() const
{
    std::vector <T> ret;
    ObliqueHeap<T, TLess> temp = *this;
    while(!temp.empty()){
        ret.push_back(temp.extractMin());
    }
    return ret;
}
#endif /* ObliqueHeap_H */
