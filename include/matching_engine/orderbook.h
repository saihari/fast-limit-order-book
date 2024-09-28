#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "datatypes.h"
#include "order.h"
#include "logger/log.h"
#include <unordered_map>
#include <boost/intrusive/link_mode.hpp>
#include <boost/intrusive/rbtree.hpp>
#include <boost/intrusive/avltree.hpp>

namespace matching_engine
{
    struct LimitNode
    {
        LimitNode(Price limit_ = 0, // Quantity size_ = 0,
                  Volume volume_ = 0) : parent_(nullptr),
                                        left_(nullptr),
                                        right_(nullptr),
                                        limit_(limit_),
                                        //   size_(size_),
                                        volume_(volume_)
        {
        }

        LimitNode *parent_, *left_, *right_;
        int balance_;
        // data members
        Price limit_;
        Volume volume_;
        OrderList order_list_;

        bool operator<(LimitNode const &other) const;
        bool operator>(LimitNode const &other) const;

        Volume get_volume() const;
    };

    // Node Traits for Limit AVL Tree
    struct LimitNodeTraits
    {
        typedef LimitNode node;
        typedef LimitNode *node_ptr;
        typedef const LimitNode *const_node_ptr;
        typedef int balance;
        static node_ptr get_parent(const_node_ptr n) { return n->parent_; }
        static void set_parent(node_ptr n, node_ptr parent) { n->parent_ = parent; }
        static node_ptr get_left(const_node_ptr n) { return n->left_; }
        static void set_left(node_ptr n, node_ptr left) { n->left_ = left; }
        static node_ptr get_right(const_node_ptr n) { return n->right_; }
        static void set_right(node_ptr n, node_ptr right) { n->right_ = right; }

        static balance get_balance(const_node_ptr n) { return n->balance_; }
        static void set_balance(node_ptr n, balance c) { n->balance_ = c; }

        static balance negative() { return -1; }
        static balance zero() { return 0; }
        static balance positive() { return 1; }
    };

    // Node Value Traits for Limit AVL Tree
    struct LimitValueTraits
    {
        typedef LimitNodeTraits node_traits;
        typedef node_traits::node value_type;
        typedef node_traits::node_ptr node_ptr;
        typedef node_traits::const_node_ptr const_node_ptr;
        typedef value_type *pointer;
        typedef value_type const *const_pointer;

        static const boost::intrusive::link_mode_type link_mode = boost::intrusive::link_mode_type::normal_link;

        static node_ptr to_node_ptr(value_type &value) { return &value; }
        static const_node_ptr to_node_ptr(const value_type &value) { return &value; }
        static pointer to_value_ptr(node_ptr n) { return n; }
        static const_pointer to_value_ptr(const_node_ptr n) { return n; }
    };

    using LimitTree = boost::intrusive::avltree<LimitNode, boost::intrusive::value_traits<LimitValueTraits>>;

    class OrderBook
    {
    private:
        LimitTree BuyTree;
        LimitTree SellTree;
        std::unordered_map<Price, LimitNode *> BuyLimitMap;
        std::unordered_map<Price, LimitNode *> SellLimitMap;
        std::unordered_map<std::string, Order *> OrderMap;
        Price HighestBuy = 0;
        Price LowestSell = 0;
        logger::Logger ApplicationLogger;

    public:
        OrderBook() {};
        void insert_limit(Price limit_price);
        // std::string insert_order(Side side, Price price, Quantity qty);
        Transaction insert_order(Order *new_order);
        void print_volume(Side side);
        bool cancel_order(std::string order_id);
        TransactionList order(Side side, Price price, Quantity qty);
        TransactionList execute_order(Order *order);
        Quantity get_quantity(Price limit_price, Side side);
        Price get_best_price(Side side, Price price);
    };
}

#endif