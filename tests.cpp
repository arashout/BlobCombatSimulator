#include "tests.hpp"
#include <assert.h>
#include "agent.hpp"

Tests::Tests()
{
    Agent a1(0);
    Agent a2(1);

    a1.canSeeEntity(a2);
}
