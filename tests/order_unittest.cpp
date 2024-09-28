#include "matching_engine/order.h"
#include "matching_engine/datatypes.h"
#include <gtest/gtest.h>
#include <regex>

using namespace matching_engine;

bool isValidUUID(const std::string &str)
{
    std::regex uuidRegex("^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[1-5][0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$");
    return std::regex_match(str, uuidRegex);
}

TEST(OrderTest, CheckBuyOrder)
{
    Side side = Side::BUY;
    Quantity qty = 25;
    Price price = 300.00;

    Order *new_order = new Order(side, qty, price);

    // Check Data Values
    EXPECT_FLOAT_EQ(new_order->qty, qty);
    EXPECT_EQ(new_order->price, price);
    EXPECT_TRUE(isValidUUID(new_order->order_id));
};

TEST(OrderTest, CheckSellOrder)
{
    Side side = Side::SELL;
    Quantity qty = 25;
    Price price = 300.00;

    Order *new_order = new Order(side, qty, price);

    // Check Data Values
    EXPECT_FLOAT_EQ(new_order->qty, qty);
    EXPECT_EQ(new_order->price, price);
    EXPECT_TRUE(isValidUUID(new_order->order_id));
};
