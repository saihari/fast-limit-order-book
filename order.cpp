#include "order.h"
#include <ctime>

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

// Helper Functions
inline OrderId gen_order_id() { return boost::uuids::random_generator()(); };
inline std::string uuid_to_string(const boost::uuids::uuid &uuid) { return boost::uuids::to_string(uuid); };

// Order Constructor
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