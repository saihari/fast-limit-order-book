#ifndef DATATYPES_H
#define DATATYPES_H

#include <boost/uuid/uuid.hpp>

// aliases
using Price = float;
using Quantity = uint32_t;
using OrderId = boost::uuids::uuid;
using Volume = uint32_t;

// enums
typedef enum class Side
{
    BUY,
    SELL,
    NONE
} Side;

#endif