#ifndef DATATYPES_H
#define DATATYPES_H

#include <boost/uuid/uuid.hpp>
#include <list>
#include <string>
#include <unordered_map>

namespace matching_engine
{

    // Enums
    typedef enum class Side
    {
        BUY,
        SELL,
        NONE
    } Side;

    static std::unordered_map<Side, std::string> SideStrings = {{Side::BUY, "BUY"},
                                                                {Side::SELL, "SELL"},
                                                                {Side::NONE, "NONE"}};

    typedef enum class TransactionType
    {
        TRADE,
        ORDER,
        NONE
    } TransactionType;

    static std::unordered_map<TransactionType, std::string> TransactionTypeStrings = {{TransactionType::TRADE, "TRADE"},
                                                                                      {TransactionType::ORDER, "ORDER"},
                                                                                      {TransactionType::NONE, "NONE"}};

    // Structs
    struct Transaction
    {
        Transaction(TransactionType transaction_type,
                    std::string order_id,
                    std::string opposite_order_id) : transaction_type_(transaction_type),
                                                     order_id_(order_id),
                                                     opposite_order_id_(opposite_order_id)
        {
        }

        // Members
        TransactionType transaction_type_;
        std::string order_id_;          // denotes order id of the order
        std::string opposite_order_id_; // denotes the order id with which the order was traded
    };

    // Aliases
    using Price = float;
    using Quantity = uint32_t; // is used to define number of share for an order
    using OrderId = boost::uuids::uuid;
    using TransactionId = boost::uuids::uuid;
    using Volume = uint32_t; // is used to define number of shares for buy/sell transactions at a limit price
    using TransactionList = std::list<Transaction>;

}

#endif