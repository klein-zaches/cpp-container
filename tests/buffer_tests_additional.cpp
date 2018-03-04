#include "catch.hpp"
#include "../src/buffer.h"
#include "../src/level.h"

namespace incremental_book
{
SCENARIO("adding to the buffer with more complicated scenarios", "[buffer]")
{
    GIVEN("A full buffer")
    {
        auto powerof2 = 3;
        auto buffer = Buffer<Level<Order>, Order>(powerof2);

        buffer.add(Order(10, 1));
        buffer.add(Order(9, 2));
        buffer.add(Order(8, 3));
        buffer.add(Order(7, 4));
        buffer.add(Order(6, 5));
        buffer.add(Order(5, 6));
        buffer.add(Order(4, 7));
        buffer.add(Order(3, 8));
        REQUIRE(buffer.size() == 8);

        WHEN("the two items with different price are added")
        {
            buffer.add(Order(2, 9));
            THEN("the size remains equal to 8 and new head is 2")
            {
                REQUIRE(buffer.size() == 8);
            }
        }
        WHEN("the two items with the same price are added")
        {
            buffer.add(Order(15, 1));
            THEN("the size remains equal to 8 and new head is 3")
            {
                REQUIRE(buffer.size() == 8);
                
            }
        }
    }
}
}