#include <string>
#include <chrono>
#include <algorithm>
#include <functional>
#include <sstream>
#include <vector>

#include "catch2/catch.hpp"

/*
    Question 2:
        char[] array = "RotateThisString";

        Implement the following function

        char[] RotateArray(char[] array, int index);

        output for RotateArray(array, 5) will be "eThisStringRotat";

        Allocate no new memory in the function.
        What is the function's efficiency?
        What if the char[] was a billion characters long? Does your solution complete efficiently?

    Assumptions:
        Assuming the goal with the function is to deal with "strings" and not so much "arrays", then we
        can leverage that assumption by pursuing in terms of std::string, with great performing results.

    Solutions:
        We do not work with malloc, new, or smart pointers in this approach, at least not directly.
        Also, that I know of, string is dealt with in terms of contiguous memory anyway.

        In terms of efficiency, of the operation itself? O(1), there is no iteration going on, but
        rather a couple of mathematical subtraction calculations. In terms of the string memory itself,
        I expect that this is probably pretty close to O(1) as well, although I have not reviewed the
        source libraries that closely, either.

        By far the longest running aspect is the test data use case creation, which we could even
        isolate and contain that in a pre-generated file, then simply read that, and either work with
        it in a more creative file view, or into memory, as we do in this approach.

        But again, as a prototype string focused C++ algorithm, the approach is sound. If we learn of
        more or other applications, requirements, etc, then we can modify our assumptions, opinions,
        approaches, etc, accordingly.

        Verified on a workstation with 64 GB; note, 1B characters is ~1% of total system memory. Of course,
        YMMV depending on the resources available to your system. In fact, I "tried" pushing that envelope
        by exercising 100B characters, before I did the math and realized this was actually 100 GB worth of
        test data; OOPS! It was trying, but I think it was also trying to page memory in and out. Not the
        most effective attempt.

        Additionally, started generating data one character at a time. This is SLOW! Instead, I append the
        full character set at a time until the length is attained. This is MUCH better, much more efficient
        at generating test data. See also comments above concerning test data generation to file, for instance.
 */

namespace interview {

    struct test_data {
        typedef std::tuple<std::string, ::size_t, std::string> use_case_type;

        std::vector<use_case_type> _use_cases;

        static test_data _data;

    private:
        /// <summary>
        /// 
        /// </summary>
        test_data() : _use_cases({
                std::make_tuple("RotateThisString", 5, "eThisStringRotat")
                // WARNING: THIS CAN BE DANGEROUS, i.e. THIS IS NEARLY 1% OF THE MEMORY ON A 64 GB SYSTEM
                , make_use_case(1000000000, 1000000)
            }) {
        }

        static const std::string cs;

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="_Period"></typeparam>
        /// <typeparam name="_Rep"></typeparam>
        /// <param name="units"></param>
        /// <returns></returns>
        static use_case_type make_use_case(std::size_t length, const std::size_t rotation) {
            std::string s;
            // Pre-allocate the size
            s.reserve(length);
            std::size_t _current;
            // Then replace each char in the sequence
            while ((_current = s.length()) < length) {
                s += cs.substr(0, std::min(length, length - s.length()));
            }
            return std::make_tuple(s, rotation, s.substr(rotation) + s.substr(0, rotation));
        }
    };

    // Could rinse and repeat for any character set
    const std::string test_data::cs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    test_data test_data::_data;

    /// <summary>
    /// An elapsed timer representation, allows elapsed time measurement beyond a well defined
    /// moment in time, when an instance was initialized.
    /// </summary>
    class elapsed_timer {
        std::chrono::high_resolution_clock::time_point _tp;

        /// <summary>
        /// Returns the HR clock time point "now".
        /// </summary>
        static const std::chrono::high_resolution_clock::time_point now() noexcept {
            return std::chrono::high_resolution_clock::now();
        }

    public:
        /// <summary>
        /// Constructor.
        /// </summary>
        elapsed_timer() : _tp(now()) {
        };

        /// <summary>
        /// Returns the elapsed time in the requested representation (default: double) and
        /// period (default: milli).
        /// </summary>
        /// <typeparam name="_Period">The duration period, default: milli</typeparam>
        /// <typeparam name="_Rep">The duration representation, default: double</typeparam>
        /// <returns></returns>
        template <class _Period = std::milli, class _Rep = double>
        std::chrono::duration<_Rep> elapsed() const {
            typedef std::chrono::duration<_Rep, _Period> duration_type;
            return std::chrono::duration_cast<duration_type>(now() - _tp);
        };

        /// <summary>
        /// Returns the string representation of the elapsed time assuming requested representation
        /// (default: double) and period (default: milli). Note that if either of these things changes,
        /// then the units may also need to change.
        /// </summary>
        /// <typeparam name="_Period">The duration period, default: milli</typeparam>
        /// <typeparam name="_Rep">The duration representation, default: double</typeparam>
        /// <param name="units">Chrono units assuming milli period.</param>
        /// <returns></returns>
        /// <see cref="!:https://stackoverflow.com/questions/42866524/convert-chronoduration-to-string-or-c-string"/>
        template <class _Period = std::milli, class _Rep = double>
        std::string elapsed_string(const std::string& units = "ms") const {
            const auto _elapsed = elapsed<_Period, _Rep>();
            std::stringstream ss;
            ss << _elapsed.count();
            if (units.length()) { ss << ' ' << units; };
            return ss.str();
        };
    };

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
    /// <remarks>
    /// Assuming by char[] we literally mean "string", then we operate under the assumption
    /// this is really std::string.
    /// </remarks>
    std::string rotate(const std::string& s, std::size_t count) {
        // https://www.cplusplus.com/reference/string/string
        // https://www.cplusplus.com/reference/string/string/substr
        count = std::min(count, s.length());
        return s.substr(count) + s.substr(0, count);
    }
}

std::string render_use_case_summary(const std::string& s, std::size_t count) {
    std::stringstream ss;
    ss << "s length: " << s.length() << ", rotate count: " << count;
    return ss.str();
}

std::string render_performance(const interview::elapsed_timer& elapsed_timer) {
    std::stringstream ss;
    ss << "rotate duration: " << elapsed_timer.elapsed_string();
    return ss.str();
}

/// <summary>
/// Rotates the given string s by the count and returns whether the actual result equals
/// the expected string. We also report the use case, as well as the elapsed time, in milliseconds,
/// of the string rotation itself. Catch also reports time components of the test case bits just
/// besides that via "--durations yes" command line arguments.
/// </summary>
/// <param name="s"></param>
/// <param name="count"></param>
/// <param name="expected"></param>
/// <returns></returns>
bool try_rotate(const std::string& s, std::size_t count, const std::string& expected) {
    using namespace interview;
    WARN(render_use_case_summary(s, count));
    elapsed_timer _elapsed_timer;
    auto actual = rotate(s, count);
    WARN(render_performance(_elapsed_timer));
    auto result = actual == expected;
    return result;
}

using namespace interview;

void RequireUseCase(const test_data::use_case_type& use_case) {
    REQUIRE(try_rotate(std::get<0>(use_case), std::get<1>(use_case), std::get<2>(use_case)));
};

TEST_CASE("interview::rotate simple use case is correct", "[rotate]") {
    RequireUseCase(test_data::_data._use_cases.at(0));
}

TEST_CASE("interview::rotate extreme use case is correct", "[rotate]") {
    RequireUseCase(test_data::_data._use_cases.at(1));
}

/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
rotate_tests.exe is a Catch v2.8.0 host application.
Run with -? for options

-------------------------------------------------------------------------------
interview::rotate simple use case is correct
-------------------------------------------------------------------------------
L:\Source\thedatagroup.io\interview\prototype\rotate_tests\rotate_tests.cpp(192)
...............................................................................

L:\Source\thedatagroup.io\interview\prototype\rotate_tests\rotate_tests.cpp(178):
warning:
  s length: 16, rotate count: 5

L:\Source\thedatagroup.io\interview\prototype\rotate_tests\rotate_tests.cpp(181):
warning:
  rotate duration: 3.23e-05 ms

0.004 s: interview::rotate simple use case is correct
-------------------------------------------------------------------------------
interview::rotate extreme use case is correct
-------------------------------------------------------------------------------
L:\Source\thedatagroup.io\interview\prototype\rotate_tests\rotate_tests.cpp(196)
...............................................................................

L:\Source\thedatagroup.io\interview\prototype\rotate_tests\rotate_tests.cpp(178): warning:
  s length: 1000000000, rotate count: 1000000

L:\Source\thedatagroup.io\interview\prototype\rotate_tests\rotate_tests.cpp(181): warning:
  rotate duration: 0.583606 ms

0.839 s: interview::rotate extreme use case is correct
===============================================================================
All tests passed (2 assertions in 2 test cases)

...waiting for enter/ return before exiting, with code: 0

 */
