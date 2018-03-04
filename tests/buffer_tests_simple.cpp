#include "catch.hpp"
#include "../src/buffer.h"
#include "../src/level.h"

namespace incremental_book
{
SCENARIO("adding to the buffer", "[buffer]")
{
    GIVEN("An empty buffer")
    {
        auto powerof2 = 5;
        auto buffer = Buffer<Level<Order>, Order>(powerof2);

        REQUIRE(buffer.max_size() == 1ULL << powerof2);
        REQUIRE(buffer.size() == 0);

        buffer.add(Order(10, 1));
        REQUIRE(buffer.size() == 1);

        WHEN("the two items with different price are added")
        {
            buffer.add(Order(11, 2));
            THEN("the size changes to 2")
            {
                REQUIRE(buffer.size() == 2);
            }
        }
        WHEN("the two items with the same price are added")
        {
            buffer.add(Order(10, 2));
            THEN("the size stays 1")
            {
                REQUIRE(buffer.size() == 1);
            }
        }
    }
}
}