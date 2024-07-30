#include <iostream>
#include "limittree.h"
#include "datatypes.h"

int main()
{

    OrderBook ob;

    // Example executions
    ob.insert_limit(25.00);
    // ob.insert_limit(35.00);
    // ob.insert_limit(15.00);
    // ob.insert_limit(45.00);
    // ob.insert_limit(37.00);
    // ob.insert_limit(10.00);
    // ob.order(Side::BUY, 15.00, 40);
    // ob.order(Side::BUY, 25.00, 20);

    ob.order(Side::SELL, 25.0, 50);
    std::cout << "\nSell Order Placed" << "\n\n\n\n";
    ob.print_volume(Side::SELL);
    std::cout << "\n\n\n\n";

    TransactionList t_list = ob.order(Side::BUY, 25.00, 500);

    // auto it = std::next(t_list.begin(), 0); // Advance iterator to index
    // if (it != t_list.end())
    // {
    //     std::cout << "Order ID to Be deleted on BUY is: " << it->order_id_ << "\n";
    // }
    // else
    // {
    //     std::cout << "Index out of range" << std::endl;
    // }

    // ob.order(Side::BUY, 25.0, 5000);
    ob.print_volume(Side::BUY);
    std::cout << "\nBuy Order Placed" << "\n\n\n\n";

    std::cout << "\n\n\n\n OrderBook \n\n\n\n";

    ob.print_volume(Side::BUY);

    ob.print_volume(Side::SELL);
    // ob.print_volume(Side::BUY);
    // ob.print_volume(Side::SELL);
    // ob.cancel_order(order_id1);

    // std::cout << "Status" << "\n\n\n\n";
    // ob.print_volume(Side::BUY);
    // ob.print_volume(Side::SELL);

    // ob.order(Side::BUY, 25.0, 50);
    // ob.print_volume(Side::BUY);
}