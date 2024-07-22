#include "limittree.h"
#include "datatypes.h"
#include "order.h"
#include <iostream>
#include <boost/intrusive/list.hpp>

// Limit Node Supporting Functions
bool LimitNode::operator<(LimitNode const &other) const
{
    return limit_ < other.limit_;
}

bool LimitNode::operator>(LimitNode const &other) const
{
    return limit_ > other.limit_;
}

Volume LimitNode::get_volume() const
{
    return volume_;
}

// Half Order book Function Implementation
void OrderBook::insert_limit(Price limit_price)
{
    LimitNode *buy_node = new LimitNode(limit_price);
    LimitNode *sell_node = new LimitNode(limit_price);

    BuyLimitMap[limit_price] = buy_node;
    SellLimitMap[limit_price] = sell_node;

    BuyTree.insert_equal(*buy_node);
    SellTree.insert_equal(*sell_node);
};

TransactionList OrderBook::execute_order(Order *order)
{
    Price best_price;
    Quantity remaining_qty = order->qty;
    TransactionList t_list{};

    if (order->side == Side::BUY)
    {
        best_price = LowestSell;
    }
    else if (order->side == Side::SELL)
    {
        best_price = HighestBuy;
    }

    // Matching Loop
    // while (remaining_qty >= 0)
    // {
    // }

    return t_list;
};

TransactionList OrderBook::order(Side side, Price price, Quantity qty)
{
    // To insert an order in the order book
    Order *new_order = new Order(side, qty, price);
    std::string new_order_id = new_order->get_order_id();
    OrderMap[new_order_id] = new_order;

    if ((new_order->side == Side::BUY) && (new_order->qty > 0) && (LowestSell != NULL) && (SellTree.size() != 0))
    {
        std::cout << "Executing Buy Trade" << std::endl;
        ApplicationLogger.log("Buy order executed");

        TransactionList t_list = execute_order(new_order);

        return t_list;
    }
    else if ((new_order->side == Side::SELL) && (new_order->qty > 0) && (HighestBuy != NULL) && (BuyTree.size() != 0))
    {
        std::cout << "Executing Sell Trade" << std::endl;
        ApplicationLogger.log("Sell order executed");

        TransactionList t_list = execute_order(new_order);

        return t_list;
    }
    else
    {
        TransactionList t_list = {insert_order(new_order)};
        return t_list;
    }
};

// std::string OrderBook::insert_order(Side side, Price price, Quantity qty)
Transaction OrderBook::insert_order(Order *new_order)
{
    // To insert an order in the order book
    // Order *new_order = new Order(side, qty, price);
    // std::string new_order_id = new_order->get_order_id();
    // OrderMap[new_order_id] = new_order;

    if (new_order->side == Side::BUY)
    {
        // Check if limit is present else insert it
        if (BuyLimitMap.find(new_order->price) == BuyLimitMap.end())
        {
            insert_limit(new_order->price);
        }

        // Insert buy order
        BuyLimitMap[new_order->price]->order_list_.push_back(*new_order);
        BuyLimitMap[new_order->price]->volume_ += new_order->qty;

        // Update HighestBuy pointer if required
        if (HighestBuy == NULL)
        {
            HighestBuy = new_order->price;
        }
        else if (HighestBuy < new_order->price)
        {
            HighestBuy = new_order->price;
        }
    }
    else if (new_order->side == Side::SELL)
    {
        // Check if limit is present else insert it.
        if (SellLimitMap.find(new_order->price) == SellLimitMap.end())
        {
            insert_limit(new_order->price);
        }

        // Insert sell order
        SellLimitMap[new_order->price]->order_list_.push_back(*new_order);
        SellLimitMap[new_order->price]->volume_ += new_order->qty;

        // Update LowestSell pointer if required
        if (LowestSell == NULL)
        {
            LowestSell = new_order->price;
        }
        else if (LowestSell > new_order->price)
        {
            LowestSell = new_order->price;
        }
    };

    // Creating the Log and Transaction Object
    ApplicationLogger.log("Order Id: " + new_order->order_id);
    Transaction t = Transaction(TransactionType::ORDER, new_order->order_id, "");

    return t;
};

bool OrderBook::cancel_order(std::string order_id)
{
    if (OrderMap.find(order_id) == OrderMap.end())
    {
        return true;
    }

    Order *order = OrderMap[order_id];

    if (order->side == Side::BUY)
    {
        OrderList::iterator it = BuyLimitMap[order->price]->order_list_.iterator_to(*order);
        BuyLimitMap[order->price]->volume_ -= order->qty;
        BuyLimitMap[order->price]->order_list_.erase(it);

        OrderMap.erase(order_id);
        if (OrderMap.find(order_id) != OrderMap.end())
        {
            std::cout << "remove unsuccessful" << "\n";
        };

        delete order;

        return true;
    }
    else
    {
        return true;
    }
    return false;
};

void OrderBook::print_volume(Side side)
{
    if (side == Side::BUY)
    {
        std::cout << "NEw Buy Vol:" << BuyTree.root()->get_volume() << std::endl;
        for (const auto &obj : BuyTree.root()->order_list_)
        {
            std::cout << "order id: " << obj.order_id << "\tprice: " << obj.price << "\tqty: " << obj.qty << "\n";
        }

        if (HighestBuy != NULL)
            std::cout << "Highest Buy price:" << HighestBuy << std::endl;
        else
            std::cout << "Highest Buy price is NULL" << std::endl;
    }
    else if (side == Side::SELL)
    {
        std::cout << "NEw Sell Vol:" << SellTree.root()->get_volume() << std::endl;
        for (const auto &obj : SellTree.root()->order_list_)
        {
            std::cout << "order id: " << obj.order_id << "\tprice: " << obj.price << "\tqty: " << obj.qty << "\n";
        }

        if (LowestSell != NULL)
            std::cout << "Lowest Sell price:" << LowestSell << std::endl;
        else
            std::cout << "Lowest Sell price is NULL" << std::endl;
    };
}