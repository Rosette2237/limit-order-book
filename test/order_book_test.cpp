#define DROGON_TEST_MAIN
#include "core/order_book.hpp"
#include <drogon/drogon_test.h>

using core::order_book;
using core::side;

DROGON_TEST(price_priority)
{
    order_book book;
    book.add(side::sell, 101, 1);
    auto best_sell = book.add(side::sell, 100, 1);
    auto result = book.add(side::buy, 101, 1);

    REQUIRE(result.trades.size() == 1);
    CHECK(result.trades[0].sell == best_sell.id);
    CHECK(result.trades[0].price == 100);
}

DROGON_TEST(fifo)
{
    order_book book;
    auto first = book.add(side::sell, 100, 1);
    book.add(side::sell, 100, 1);
    auto result = book.add(side::buy, 100, 1);

    REQUIRE(result.trades.size() == 1);
    CHECK(result.trades[0].sell == first.id);
}

DROGON_TEST(partial_fill)
{
    order_book book;
    book.add(side::sell, 100, 10);
    auto result = book.add(side::buy, 100, 4);

    REQUIRE(result.trades.size() == 1);
    CHECK(result.trades[0].qty == 4);

    auto snapshot = book.top();

    REQUIRE(snapshot.asks.size() == 1);
    CHECK(snapshot.asks[0].qty == 6);
}

DROGON_TEST(cross_levels)
{
    order_book book;
    book.add(side::sell, 100, 2);
    book.add(side::sell, 101, 3);
    auto result = book.add(side::buy, 101, 5);

    REQUIRE(result.trades.size() == 2);
    CHECK(result.trades[0].price == 100);
    CHECK(result.trades[1].price == 101);
}

DROGON_TEST(cancel)
{
    order_book book;
    auto order = book.add(side::buy, 99, 5);
    CHECK(book.cancel(order.id));
    CHECK(book.top().bids.empty());
    CHECK(!book.cancel(order.id));
}


DROGON_TEST(invalid_cancel)
{
    order_book book;
    CHECK(!book.cancel(42));

    auto order = book.add(side::buy, 99, 5);
    CHECK(book.cancel(order.id));
    CHECK(!book.cancel(order.id));
    
    auto filled = book.add(side::sell, 100, 1);
    book.add(side::buy, 100, 1);
    CHECK(!book.cancel(filled.id));
}

int main(int argc, char** argv)
{
    return drogon::test::run(argc, argv);
}
