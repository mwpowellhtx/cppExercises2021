#include <algorithm>

#include "catch2/catch.hpp"

/*
    Question 1:
        int i = 10;
        int j = 50;

        Reverse the values of both integers and allocate no new memory.

        Does your solution work for all possible ints?

    Assumptions:
        By "allocate" I take to mean intentional use of malloc, new, or even smart pointer allocations.
        Additionally, "allocate" not including local, scoped or temporary variables.
 */

/// <summary>
/// Caveat, requires experimental/preview features, YMMV for other compilers, i.e. GCC.
/// </summary>
/// <typeparam name="T"></typeparam>
/// <see cref="!:https://stackoverflow.com/questions/59379735/visual-studio-2019-c-concept-identifier-is-undefined"/>
template <typename T> concept Swappable = requires(T x, T y) { std::swap(x, y); };
//                    ^^^^^^^

/// <summary>
/// So with this challenge, we could roll our own, of course, but in this case we
/// just lean on the std namespace, specifically std::swap.
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
/// <see cref="!:https://en.cppreference.com/w/cpp/algorithm/swap"/>
/// <see cref="!:https://en.cppreference.com/w/cpp/named_req/Swappable"/>
template <class T> requires Swappable<T>
    bool try_std_swap(T x, T y) {
        using namespace std;
        auto a = x;
        auto b = y;
        // Swap but only when we need to
        if (x != y) {
            swap<T>(a, b);
        };
        return a == y && b == x;
    }

    /* Two questions proposed:
     *  1. Reverse the values of both integers and allocate no new memory.
     *      Done.
     * 2. Does your solution work for all possible ints?
     *      What exactly is the question here, will elaborate best I can...
     *      First, not only does it work for "ints", but since it is template oriented solution, should work for any primitive or user defined type that is also Swappable.
     *      Second, if we are driving at "corner cases" such as (x == y), it is a virtually a non-issue.
     *      For example, if we are simply talking about bits at a memory address, we do not care what the values were, just swap them, right?
     *      However, one minor optimization we can verify inequality prior to swapping, i.e. no need to swap when they are both equal.
     */

    TEST_CASE("std::swap is correct", "[swap]") {
        REQUIRE(try_std_swap(1, 2));
        REQUIRE(try_std_swap(2, 3));
        REQUIRE(try_std_swap('a', 'b'));
        REQUIRE(try_std_swap('x', 'y'));
    }
