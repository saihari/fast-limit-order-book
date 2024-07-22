#ifndef DATATYPES_H
#define DATATYPES_H

#include <boost/uuid/uuid.hpp>
#include <list>
#include <string>

// Enums
typedef enum class Side
{
    BUY,
    SELL,
    NONE
} Side;

typedef enum class TransactionType
{
    TRADE,
    ORDER,
    NONE
} TransactionType;

// Structs

struct Transaction
{
    Transaction(TransactionType transaction_type,
                std::string order_id,
                std::string transaction_id) : transaction_type_(transaction_type),
                                              order_id_(order_id),
                                              transaction_id_(transaction_id)
    {
    }

    // Members
    TransactionType transaction_type_;
    std::string order_id_;
    std::string transaction_id_;
};

// Aliases
using Price = float;
using Quantity = uint32_t;
using OrderId = boost::uuids::uuid;
using Volume = uint32_t;
using TransactionList = std::list<Transaction>;

#endif