#include <iostream>
#include <string>
#include "tao/pegtl.hpp"

namespace pegtl = tao::pegtl;

struct KeyValue
{
    std::string key, value;
};

struct arrow_separator : pegtl::seq<
                             pegtl::plus<pegtl::space>,
                             TAO_PEGTL_STRING("->"),
                             pegtl::plus<pegtl::space>>
{
};

struct equal_separator : pegtl::seq<
                             pegtl::plus<pegtl::space>,
                             TAO_PEGTL_STRING("="),
                             pegtl::plus<pegtl::space>>
{
};

struct semicolon_separator : pegtl::seq<
                                 pegtl::plus<pegtl::space>,
                                 TAO_PEGTL_STRING(":"),
                                 pegtl::plus<pegtl::space>>
{
};

struct rule_equal_statement_identifier : pegtl::identifier
{
};
struct rule_equal_statement : pegtl::seq<
                                  rule_equal_statement_identifier,
                                  equal_separator,
                                  pegtl::plus<pegtl::print>>
{
};

struct rule_arrow_statement_identifier : pegtl::identifier
{
};
struct rule_arrow_statement : pegtl::seq<
                                  rule_arrow_statement_identifier,
                                  arrow_separator,
                                  pegtl::plus<pegtl::print>>
{
};

struct rule_semicolon_statement_identifier : pegtl::identifier
{
};
struct rule_semicolon_statement : pegtl::seq<
                                      rule_semicolon_statement_identifier,
                                      semicolon_separator,
                                      pegtl::plus<pegtl::print>>
{
};

template <typename Rule>
struct action : pegtl::nothing<Rule>
{
};

template <>
struct action<rule_equal_statement_identifier>
{
    template <typename ActionInput>
    static void apply(const ActionInput &in, KeyValue &kv)
    {
        kv.key = "== ";
        kv.key += in.string();
    }
};

template <>
struct action<rule_arrow_statement_identifier>
{
    template <typename ActionInput>
    static void apply(const ActionInput &in, KeyValue &kv)
    {
        kv.key = "-> ";
        kv.key += in.string();
    }
};

template <>
struct action<rule_semicolon_statement_identifier>
{
    template <typename ActionInput>
    static void apply(const ActionInput &in, KeyValue &kv)
    {
        kv.key = ":: ";
        kv.key += in.string();
    }
};

template <>
struct action<pegtl::print>
{
    template <typename ActionInput>
    static void apply(const ActionInput &in, KeyValue &kv)
    {
        kv.value += in.string();
    }
};

struct grammar : pegtl::sor<rule_semicolon_statement, rule_arrow_statement, rule_equal_statement>
{
};

int main(int argc, char *argv[])
{
    std::cout << "Initing..." << std::endl;

    tao::pegtl::argv_input in(argv, 1);

    KeyValue kv;

    pegtl::parse<grammar, action>(in, kv);

    std::cout << kv.key << std::endl;
    std::cout << kv.value << std::endl;

    return 0;
}
