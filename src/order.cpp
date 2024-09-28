#include "matching_engine/order.h"
#include <ctime>

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace matching_engine
{
    // Generate UUID
    inline OrderId gen_order_id()
    {
        return boost::uuids::random_generator()();
    };

    // Convert UUID to string
    inline std::string uuid_to_string(const boost::uuids::uuid &uuid) { return boost::uuids::to_string(uuid); };

    // Order Constructor
    Order::Order() {}; // Default Constructor

    // Order Parameterised Constructor
    Order::Order(const Side side, const Quantity qty, const Price price) : side(side), qty(qty), price(price)
    {
        order_id = uuid_to_string(gen_order_id());
        entry_time = time(0);
    };

    // Get OrderID
    std::string Order::get_order_id()
    {
        return order_id;
    };
}

// Helper Functions
