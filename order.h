#ifndef ORDER_H
#define ORDER_H

#include "datatypes.h"
#include <string>
#include <ctime>
#include <boost/intrusive/list.hpp>

class Order
{
public:
    boost::intrusive::list_member_hook<> member_hook_;

    Side side;
    Quantity qty;
    Price price;
    std::string order_id;
    time_t entry_time;
    time_t exit_time;

    // Order() : side(Side::NONE), qty(0), price(0), entry_time(time(0)), exit_time(time(0)){};

    // // Copy constructor
    // Order(const Order &other)
    //     : side(other.side), qty(other.qty), price(other.price),
    //       order_id(other.order_id), entry_time(other.entry_time),
    //       exit_time(other.exit_time) {}

    // Order(Order &&) = default;

    Order(const Side side, const Quantity qty, const Price price);
    std::string get_order_id();
};

using OrderList = boost::intrusive::list<Order,
                                         boost::intrusive::member_hook<Order,
                                                                       boost::intrusive::list_member_hook<>, &Order::member_hook_>>;

#endif