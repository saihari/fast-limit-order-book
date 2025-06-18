#include "matching_engine/orderbook.h"
#include "matching_engine/datatypes.h"
#include <gtest/gtest.h>
#include "gtest/gtest_prod.h"
#include <regex>

namespace matching_engine
{
    class OrderBookTest : public ::testing::Test
    {
    protected:
        OrderBook ob;

        bool insert_negative_limit_test()
        {
            Price neg_price = -1.00;

            ob.insert_limit(neg_price);

            if (ob.BuyLimitMap.find(neg_price) != ob.BuyLimitMap.end())
                return false;

            if (ob.SellLimitMap.find(neg_price) != ob.SellLimitMap.end())
                return false;

            if (ob.BuyTree.size() != 0)
                return false;

            if (ob.SellTree.size() != 0)
                return false;

            return true;
        }

        bool insert_zero_limit_test()
        {
            Price zero_price = 0.00;

            ob.insert_limit(zero_price);

            if (ob.BuyLimitMap.find(zero_price) != ob.BuyLimitMap.end())
                return false;

            if (ob.SellLimitMap.find(zero_price) != ob.SellLimitMap.end())
                return false;

            if (ob.BuyTree.size() != 0)
                return false;

            if (ob.SellTree.size() != 0)
                return false;

            return true;
        }

        bool insert_valid_limit_test()
        {
            Price valid_price = 1.00;

            ob.insert_limit(valid_price);

            if (ob.BuyLimitMap.find(valid_price) == ob.BuyLimitMap.end())
                return false;

            if (ob.SellLimitMap.find(valid_price) == ob.SellLimitMap.end())
                return false;

            if (ob.BuyTree.size() != 1)
                return false;

            if (ob.SellTree.size() != 1)
                return false;

            return true;
        }

        bool insert_negative_price_buy_order_test()
        {
            Order *new_order = new Order(Side::BUY, 1, -0.5);

            Transaction t = ob.insert_order(new_order);

            if ((t.transaction_type_ == TransactionType::NONE) && (t.order_id_ == "") && (t.opposite_order_id_ == ""))
                return true;

            return false;
        }

        bool insert_negative_price_sell_order_test()
        {
            Order *new_order = new Order(Side::SELL, 1, -0.5);

            Transaction t = ob.insert_order(new_order);

            if ((t.transaction_type_ == TransactionType::NONE) && (t.order_id_ == "") && (t.opposite_order_id_ == ""))
                return true;

            return false;
        }

        // bool insert_negative_qty_buy_order_test()
        // {
        //     Order *new_order = new Order(Side::BUY, -1, 0.5);

        //     Transaction t = ob.insert_order(new_order);

        //     if ((t.transaction_type_ == TransactionType::NONE) && (t.order_id_ == "") && (t.opposite_order_id_ == ""))
        //         return true;

        //     return false;
        // }

        // bool insert_negative_qty_sell_order_test()
        // {
        //     Order *new_order = new Order(Side::SELL, -1, 0.5);

        //     Transaction t = ob.insert_order(new_order);

        //     if ((t.transaction_type_ == TransactionType::NONE) && (t.order_id_ == "") && (t.opposite_order_id_ == ""))
        //         return true;

        //     return false;
        // }

        bool insert_zero_price_buy_order_test()
        {
            Order *new_order = new Order(Side::BUY, 1, 0);

            Transaction t = ob.insert_order(new_order);

            if ((t.transaction_type_ == TransactionType::NONE) && (t.order_id_ == "") && (t.opposite_order_id_ == ""))
                return true;

            return false;
        }

        bool insert_zero_qty_buy_order_test()
        {
            Order *new_order = new Order(Side::BUY, 0, 0.5);

            Transaction t = ob.insert_order(new_order);

            if ((t.transaction_type_ == TransactionType::NONE) && (t.order_id_ == "") && (t.opposite_order_id_ == ""))
                return true;

            return false;
        }

        bool insert_zero_price_sell_order_test()
        {
            Order *new_order = new Order(Side::SELL, 1, 0);

            Transaction t = ob.insert_order(new_order);

            if ((t.transaction_type_ == TransactionType::NONE) && (t.order_id_ == "") && (t.opposite_order_id_ == ""))
                return true;

            return false;
        }

        bool insert_zero_qty_sell_order_test()
        {
            Order *new_order = new Order(Side::SELL, 0, 0.5);

            Transaction t = ob.insert_order(new_order);

            if ((t.transaction_type_ == TransactionType::NONE) && (t.order_id_ == "") && (t.opposite_order_id_ == ""))
                return true;

            return false;
        }

        bool insert_valid_buy_order_test()
        {
            Order *new_order = new Order(Side::BUY, 1, 1);

            Transaction t = ob.insert_order(new_order);

            if ((t.transaction_type_ == TransactionType::ORDER) && (t.order_id_ != "") && (t.opposite_order_id_ == ""))
                return true;

            return false;
        }

        bool insert_valid_sell_order_test()
        {
            Order *new_order = new Order(Side::SELL, 1, 1);

            Transaction t = ob.insert_order(new_order);

            if ((t.transaction_type_ == TransactionType::ORDER) && (t.order_id_ != "") && (t.opposite_order_id_ == ""))
                return true;

            return false;
        }

        bool insert_none_side_order_test()
        {
            Order *new_order = new Order(Side::NONE, 1, 1);

            Transaction t = ob.insert_order(new_order);

            if ((t.transaction_type_ == TransactionType::NONE) && (t.order_id_ == "") && (t.opposite_order_id_ == ""))
                return true;

            return false;
        }

        bool cancel_valid_order()
        {
        }
    };

    TEST_F(OrderBookTest, InsertNegativeLimit)
    {
        ASSERT_EQ(insert_negative_limit_test(), true);
    };

    TEST_F(OrderBookTest, InsertZeroLimit)
    {
        ASSERT_EQ(insert_negative_limit_test(), true);
    };

    TEST_F(OrderBookTest, InsertValidLimit)
    {
        ASSERT_EQ(insert_valid_limit_test(), true);
    };

    TEST_F(OrderBookTest, InsertNegativePriceOrder)
    {
        ASSERT_EQ(insert_negative_price_buy_order_test(), true);
        ASSERT_EQ(insert_negative_price_sell_order_test(), true);
    };

    // Negative quantity Testing is not required because the datatype of the variable is that of an unsigned int.
    // TEST_F(OrderBookTest, InsertNegativeQtyOrder)
    // {
    //     ASSERT_EQ(insert_negative_qty_buy_order_test(), true);
    //     ASSERT_EQ(insert_negative_qty_sell_order_test(), true);
    // };

    TEST_F(OrderBookTest, InsertZeroPriceOrder)
    {
        ASSERT_EQ(insert_zero_price_buy_order_test(), true);
        ASSERT_EQ(insert_zero_price_sell_order_test(), true);
    };

    TEST_F(OrderBookTest, InsertZeroQtyOrder)
    {
        ASSERT_EQ(insert_zero_qty_buy_order_test(), true);
        ASSERT_EQ(insert_zero_qty_sell_order_test(), true);
    };

    TEST_F(OrderBookTest, InsertNoneOrder)
    {
        ASSERT_EQ(insert_none_side_order_test(), true);
    };

    TEST_F(OrderBookTest, InsertValidOrder)
    {
        ASSERT_EQ(insert_valid_buy_order_test(), true) << "Valid Buy Order Failed";
        ASSERT_EQ(insert_valid_sell_order_test(), true) << "Valid Sell Order Failed";
    };
}
