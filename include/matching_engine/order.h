#ifndef ORDER_H
#define ORDER_H

#include "datatypes.h"
#include <string>
#include <ctime>
#include <boost/intrusive/list.hpp>

namespace matching_engine
{
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

        Order();
        Order(const Side side, const Quantity qty, const Price price);
        std::string get_order_id();
    };

    using OrderList = boost::intrusive::list<Order,
                                             boost::intrusive::member_hook<Order,
                                                                           boost::intrusive::list_member_hook<>, &Order::member_hook_>>;
}

#endif