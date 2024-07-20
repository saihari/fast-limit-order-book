// #include <iostream>
// #include <list>
// #include <unordered_map>
// #include <string>
// #include <boost/uuid/uuid.hpp>            // uuid class
// #include <boost/uuid/uuid_generators.hpp> // generators
// #include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
// #include <boost/array.hpp>
// #include <boost/version.hpp>
// #include <boost/intrusive/rbtree.hpp> // Red-Black Tree implementation

// using namespace std;

// using Price = std::uint32_t;
// using Quantity = std::uint32_t;
// using OrderId = boost::uuids::uuid;

// // Helper Functions
// inline OrderId gen_order_id() { return boost::uuids::random_generator()(); };
// inline std::string uuid_to_string(const boost::uuids::uuid &uuid) { return boost::uuids::to_string(uuid); };

// class Limit : public boost::intrusive::rbtree_node<>
// {
// private:
//     Price limit_price;
//     int size;
//     Quantity totalvolume;

// public:
//     Limit();
// };

// // Define the Red-Black Tree type
// typedef boost::intrusive::rbtree<
//     Limit,
//     boost::intrusive::rbtree_node_traits<
//         boost::intrusive::optimize_size<true>>>
//     OrderTree;

// class Order
// {
// private:
//     Side side;
//     Quantity qty;
//     Price price;
//     string order_id;

// public:
//     Order(const Side side, const Quantity qty, const Price price) : side(side), qty(qty), price(price)
//     {
//         order_id = uuid_to_string(gen_order_id());
//         // std::cout << "Order ID: " << order_id << std::endl;
//     }

//     inline string get_order_id()
//     {
//         return order_id;
//     };
// };

// int main()
// {

//     std::list<Order *> orderList;
//     std::unordered_map<std::string, Order *> orderMap;

//     orderList.emplace_back(new Order(Side::BUY, 25, 250));
//     orderMap[orderList.back()->get_order_id()] = orderList.back();

//     orderList.emplace_back(new Order(Side::SELL, 25, 2150));
//     orderMap[orderList.back()->get_order_id()] = orderList.back();

//     // Iterate over the map and print order IDs and addresses
//     for (const auto &[orderId, order] : orderMap)
//     {
//         std::cout << "Order ID: " << orderId << ", Address: " << order << std::endl;
//     }

//     // Order order(Side::BUY, 25, 250);

//     // std::cout << "HI Order id: " << order.get_order_id() << endl;

//     return 0;
// }

#include <iostream>
#include "limittree.h"
#include "datatypes.h"

int main()
{

    OrderBook ob;

    // Example executions
    ob.insert_limit(25.00);
    std::string order_id1 = ob.order(Side::BUY, 25.00, 50);
    std::cout << "Order ID to Be deleted on BUY is: " << order_id1 << "\n";
    ob.print_volume(Side::BUY);
    ob.order(Side::BUY, 25.0, 5000);
    ob.order(Side::SELL, 25.0, 50);
    ob.print_volume(Side::BUY);
    ob.print_volume(Side::SELL);
    ob.cancel_order(order_id1);

    std::cout << "Status" << "\n\n\n\n";
    ob.print_volume(Side::BUY);
    ob.print_volume(Side::SELL);

    // ob.order(Side::BUY, 25.0, 50);
    // ob.print_volume(Side::BUY);
}