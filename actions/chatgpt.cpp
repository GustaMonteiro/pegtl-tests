#include "tao/pegtl.hpp"
#include <iostream>

namespace pegtl = tao::pegtl;
namespace peg = tao::pegtl;

// Rules
struct arrow_separator : peg::seq<
                             peg::plus<peg::space>,
                             TAO_PEGTL_STRING("->"),
                             peg::plus<peg::space>>
{
};

struct equal_separator : peg::seq<
                             peg::plus<peg::space>,
                             TAO_PEGTL_STRING("="),
                             peg::plus<peg::space>>
{
};

struct semicolon_separator : peg::seq<
                                 peg::plus<peg::space>,
                                 TAO_PEGTL_STRING(":"),
                                 peg::plus<peg::space>>
{
};

struct Rule1 : peg::seq<
                                  peg::identifier,
                                  equal_separator,
                                  peg::print>
{
};

struct Rule2 : peg::seq<
                                  peg::identifier,
                                  arrow_separator,
                                  peg::print>
{
};

struct Rule3 : peg::seq<
                                  peg::identifier,
                                  semicolon_separator,
                                  peg::print>
{
};

// Action
template <typename Rule>
struct Action : pegtl::nothing<Rule>
{
};

template <>
struct Action<Rule1>
{
    template <typename Input>
    static void apply(const Input& in)
    {
        std::cout << "-> " << in.string() << std::endl;
    }
};

template <>
struct Action<Rule2>
{
    template <typename Input>
    static void apply(const Input& in)
    {
        std::cout << ":: " << in.string() << std::endl;
    }
};

template <>
struct Action<Rule3>
{
    template <typename Input>
    static void apply(const Input& in)
    {
        std::cout << "== " << in.string() << std::endl;
    }
};

int main()
{
    // Example usage
    std::string input1 = "example -> value";
    pegtl::memory_input in1(input1.c_str(), "");
    pegtl::parse<Rule1, Action>(in1);

    std::string input2 = "example : value";
    pegtl::memory_input in2(input2.c_str(), "");
    pegtl::parse<Rule2, Action>(in2);

    std::string input3 = "example = value";
    pegtl::memory_input in3(input3.c_str(), "");
    pegtl::parse<Rule3, Action>(in3);

    return 0;
}
