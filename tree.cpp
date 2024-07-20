// #include <boost/intrusive/link_mode.hpp>
// #include <boost/intrusive/rbtree.hpp>
// #include <unordered_map>
// #include <iostream>
// #include <vector>

// #include "datatypes.h"

// namespace bi = boost::intrusive;
// using namespace std;

// struct LimitNode
// {
//     LimitNode(Price limit_ = 0, // Quantity size_ = 0,
//               Volume volume_ = 0) : parent_(nullptr),
//                                     left_(nullptr),
//                                     right_(nullptr),
//                                     limit_(limit_),
//                                     //   size_(size_),
//                                     volume_(volume_)
//     {
//     }

//     LimitNode *parent_, *left_, *right_;
//     int color_;
//     // data members
//     Price limit_;
//     Volume volume_;
//     // Quantity size_;

//     bool operator<(LimitNode const &other) const { return limit_ < other.limit_; }
// };

// // Define node traits
// struct LimitNodeTraits
// {
//     typedef LimitNode node;
//     typedef LimitNode *node_ptr;
//     typedef const LimitNode *const_node_ptr;
//     typedef int color;
//     static node_ptr get_parent(const_node_ptr n) { return n->parent_; }
//     static void set_parent(node_ptr n, node_ptr parent) { n->parent_ = parent; }
//     static node_ptr get_left(const_node_ptr n) { return n->left_; }
//     static void set_left(node_ptr n, node_ptr left) { n->left_ = left; }
//     static node_ptr get_right(const_node_ptr n) { return n->right_; }
//     static void set_right(node_ptr n, node_ptr right) { n->right_ = right; }
//     static color get_color(const_node_ptr n) { return n->color_; }
//     static void set_color(node_ptr n, color c) { n->color_ = c; }
//     static color black() { return color(0); }
//     static color red() { return color(1); }
// };

// struct LimitValueTraits
// {
//     typedef LimitNodeTraits node_traits;
//     typedef node_traits::node value_type;
//     typedef node_traits::node_ptr node_ptr;
//     typedef node_traits::const_node_ptr const_node_ptr;
//     typedef value_type *pointer;
//     typedef value_type const *const_pointer;

//     static const bi::link_mode_type link_mode = bi::link_mode_type::normal_link;

//     static node_ptr to_node_ptr(value_type &value) { return &value; }
//     static const_node_ptr to_node_ptr(const value_type &value) { return &value; }
//     static pointer to_value_ptr(node_ptr n) { return n; }
//     static const_pointer to_value_ptr(const_node_ptr n) { return n; }
// };

// using LimitTree = bi::rbtree<LimitNode, bi::value_traits<LimitValueTraits>>;

// class OrderBook
// {
// private:
//     LimitTree rbtree;
//     std::unordered_map<Price, LimitNode *> LimitMap;

// public:
//     OrderBook(){};
//     void insert_limit(Price limit_price)
//     {
//         LimitNode *node = new LimitNode(limit_price);

//         LimitMap[limit_price] = node;
//         rbtree.insert_equal(*node);
//     };

//     void insert_order(Price limit_price, Volume volume)
//     {
//         LimitMap[limit_price]->volume_ += volume;
//     };

//     void print_volume()
//     {
//         cout << "NEw Vol:" << rbtree.root()->volume_ << endl;
//     };
// };

// int main()
// {
//     std::vector<LimitNode> storage{{1, 0}, {2, 0}, {3, 0}, {4, 1000}, {5, 0}};

//     OrderBook ob;

//     ob.insert_limit(25);
//     ob.insert_order(25, 50);
//     ob.print_volume();
// }