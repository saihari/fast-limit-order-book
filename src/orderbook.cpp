#include "matching_engine/orderbook.h"
#include "matching_engine/datatypes.h"
#include "matching_engine/order.h"
#include <iostream>
#include <boost/intrusive/list.hpp>

namespace matching_engine
{

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

    // Order book Function Implementation
    void OrderBook::insert_limit(Price limit_price)
    {
        LimitNode *buy_node = new LimitNode(limit_price);
        LimitNode *sell_node = new LimitNode(limit_price);

        BuyLimitMap[limit_price] = buy_node;
        SellLimitMap[limit_price] = sell_node;

        BuyTree.insert_equal(*buy_node);
        SellTree.insert_equal(*sell_node);
    };

    Quantity OrderBook::get_quantity(Price limit_price, Side side)
    {

        if (limit_price == 0) // This happens when there is no best price to fill the order
            return 0;

        if (side == Side::BUY)
            return SellLimitMap[limit_price]->volume_;
        else if (side == Side::SELL)
            return BuyLimitMap[limit_price]->volume_;

        return 0;
    };

    Price OrderBook::get_best_price(Side side, Price price)
    {
        if (side == Side::SELL)
        {
            // Iterator to the highestValue node
            LimitTree::iterator highest_value_it = BuyTree.iterator_to(*BuyLimitMap[HighestBuy]);

            // If the highest value is at the begining of the tree
            if (highest_value_it == BuyTree.begin())
                return 0;

            // Iterate backwards until we find a node with volume not equal to zero
            // Not including the highest value
            LimitTree::iterator nextIt = --highest_value_it;

            while (true)
            {
                if (nextIt->volume_ != 0)
                {
                    return nextIt->limit_;
                }

                if ((nextIt == BuyTree.begin()) || (price > nextIt->limit_))
                    break;

                --nextIt;
            };

            return 0;
        }

        else if (side == Side::BUY)
        {
            // Iterator to the lowestValue node
            LimitTree::iterator lowest_value_it = SellTree.iterator_to(*SellLimitMap[LowestSell]);

            // If the lowest value is at the end of the tree
            if (lowest_value_it == SellTree.end())
                return 0;

            // Iterate forwards until we find a node with volume not equal to zero
            // Not including the highest value
            LimitTree::iterator nextIt = ++lowest_value_it;

            while (true)
            {
                // If volume != 0, return it
                if (nextIt->volume_ != 0)
                {
                    return nextIt->limit_;
                }

                if ((nextIt == SellTree.end()) || (price < nextIt->limit_))
                    break;

                ++nextIt;
            };

            return 0;
        };

        return 0;
    };

    TransactionList OrderBook::execute_order(Order *order)
    {
        Price best_price;
        Quantity availabe_qty;
        Quantity remaining_qty = order->qty;
        TransactionList t_list{};

        //  Best price for the order
        if (order->side == Side::BUY)
            best_price = LowestSell;
        else if (order->side == Side::SELL)
            best_price = HighestBuy;

        // Available quantity at Limit
        availabe_qty = get_quantity(best_price, order->side);

        // Matching Loop
        while (remaining_qty > 0)
        {
            // Step 1: Check availability at best price, else get best price and restart the loop
            if (availabe_qty == 0)
            {
                // get next best price where qty is available
                best_price = get_best_price(order->side, order->price);
                std::cout << "\nbest_price: " << best_price << std::endl;
                availabe_qty = get_quantity(best_price, order->side);

                if (order->side == Side::BUY)
                    LowestSell = best_price;
                else if (order->side == Side::SELL)
                    HighestBuy = best_price;
            };

            if (best_price == 0)
                break;

            // Step 2: Update order book (aka making the trade)
            // Get order from orders list of Limit node for respective price
            Order existing_order;
            if (order->side == Side::BUY)
                existing_order = SellLimitMap[best_price]->order_list_.front();
            else if (order->side == Side::SELL)
                existing_order = BuyLimitMap[best_price]->order_list_.front();

            // Get minimum qty this is the executed quantity
            Quantity min_qty = std::min(remaining_qty, existing_order.qty);

            // Removing the minimum quantity from existing order and current available qty at Limit
            availabe_qty -= min_qty;
            existing_order.qty -= min_qty;
            remaining_qty -= min_qty;

            //  if existing order qty is 0 then remove the order
            if (existing_order.qty == 0)
            {
                bool status = cancel_order(existing_order.order_id);
                if (!status)
                    return {};
            }

            // Removing the minimum quantity from volume at LimitNode
            // We also check if existing order has been zero as the cancel_order function
            //      automatically subtracts from LimitNode's volume
            if ((order->side == Side::BUY) & (existing_order.qty != 0))
                SellLimitMap[best_price]->volume_ -= min_qty;
            else if ((order->side == Side::SELL) & (existing_order.qty != 0))
                BuyLimitMap[best_price]->volume_ -= min_qty;

            // std::cout << "best_price: " << best_price << "\navailable qty: " << availabe_qty << "\nmin qty: " << min_qty << "\nremaining_qty: " << remaining_qty << std::endl;
            //  Record trade
            // Creating the Log and Transaction Object
            ApplicationLogger.log("Trade \tOrder Id: " + order->order_id + "\tOpposite Order Id: " + existing_order.order_id);
            Transaction t = Transaction(TransactionType::TRADE, order->order_id, existing_order.order_id);
            t_list.emplace_back(t);
        }
        std::cout << "\nremaining_qty: " << remaining_qty << std::endl;
        if (remaining_qty > 0)
        {
            //  Update order qty (case of partial fill)
            order->qty = remaining_qty;
            t_list.emplace_back(insert_order(order));
        }

        return t_list;
    };

    TransactionList OrderBook::order(Side side, Price price, Quantity qty)
    {
        // To insert an order in the order book
        Order *new_order = new Order(side, qty, price);
        std::string new_order_id = new_order->get_order_id();
        OrderMap[new_order_id] = new_order;

        if ((new_order->side == Side::BUY) && (new_order->qty > 0) && (LowestSell != 0) && (SellTree.size() != 0))
        {
            std::cout << "Executing Buy Trade" << std::endl;
            ApplicationLogger.log("Buy order executed");

            TransactionList t_list = execute_order(new_order);

            return t_list;
        }
        else if ((new_order->side == Side::SELL) && (new_order->qty > 0) && (HighestBuy != 0) && (BuyTree.size() != 0))
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

    Transaction OrderBook::insert_order(Order *new_order)
    {
        // To insert an order in the order book

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
            if (HighestBuy == 0)
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
            if (LowestSell == 0)
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
        else if (order->side == Side::SELL)
        {
            OrderList::iterator it = SellLimitMap[order->price]->order_list_.iterator_to(*order);
            SellLimitMap[order->price]->volume_ -= order->qty;
            SellLimitMap[order->price]->order_list_.erase(it);

            OrderMap.erase(order_id);
            if (OrderMap.find(order_id) != OrderMap.end())
            {
                std::cout << "remove unsuccessful" << "\n";
            };

            delete order;

            return true;
        }
        return false;
    };

    void OrderBook::print_volume(Side side)
    {
        if (side == Side::BUY)
        {
            std::cout << "Buy Side:\n";
            for (LimitTree::iterator obj = BuyTree.begin(); obj != BuyTree.end(); ++obj)
            {
                std::cout << "\tLimit: " << obj->limit_ << "\tvolume: " << obj->volume_ << "\n";
            }

            if (HighestBuy != 0)
                std::cout << "Highest Buy price:" << HighestBuy << std::endl;
            else
                std::cout << "Highest Buy price is 0" << std::endl;
        }
        else if (side == Side::SELL)
        {
            std::cout << "Sell Side:\n";
            for (LimitTree::iterator obj = SellTree.begin(); obj != SellTree.end(); ++obj)
            {
                std::cout << "\tLimit: " << obj->limit_ << "\tvolume: " << obj->volume_ << "\n";
            }

            if (LowestSell != 0)
                std::cout << "Lowest Sell price:" << LowestSell << std::endl;
            else
                std::cout << "Lowest Sell price is 0" << std::endl;
        };
    }
}