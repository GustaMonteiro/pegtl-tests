#include <iostream>
#include <string>
#include <map>
#include "tao/pegtl.hpp"
#include "tao/pegtl/contrib/trace.hpp"

namespace pegtl = tao::pegtl;

using IniContents = std::map<std::string, std::map<std::string, std::string>>;

std::string trim_string(const std::string &str)
{
    size_t first = str.find_first_not_of(" \n\t\r");
    size_t last = str.find_last_not_of(" \n\t\r");

    if (first == std::string::npos)
        return "";

    return str.substr(first, (last - first + 1));
}

struct Ini
{
    IniContents contents;
    std::string current_section_header = "";
    std::string current_statement_identifier = "";
};

Ini ini_file_contents;

// ___ = ___
struct equal : pegtl::seq<pegtl::plus<pegtl::space>, TAO_PEGTL_STRING("="), pegtl::plus<pegtl::space>>
{
};

struct header_identifier : pegtl::identifier
{
};

// [something]
struct header : pegtl::seq<
                    TAO_PEGTL_STRING("["),
                    pegtl::star<pegtl::space>,
                    header_identifier,
                    pegtl::star<pegtl::space>,
                    TAO_PEGTL_STRING("]"),
                    pegtl::plus<pegtl::eol>>
{
};

struct key_value_identifier : pegtl::identifier
{
};
struct key_value_value : pegtl::plus<pegtl::print>
{
};

// KEY = VALUE
struct key_value_statement : pegtl::seq<
                                 pegtl::star<pegtl::space>,
                                 key_value_identifier,
                                 equal,
                                 key_value_value,
                                 pegtl::star<pegtl::eol>>
{
};

struct section : pegtl::seq<
                     header,
                     pegtl::plus<key_value_statement>>
{
};

struct grammar : pegtl::plus<section>
{
};

template <typename Rule>
struct action : pegtl::nothing<Rule>
{
};

template <>
struct action<header_identifier>
{
    template <typename ActionInput>
    static void apply(const ActionInput &in, Ini &ini)
    {
        ini.current_section_header = trim_string(in.string());
    }
};

template <>
struct action<key_value_identifier>
{
    template <typename ActionInput>
    static void apply(const ActionInput &in, Ini &ini)
    {
        ini.current_statement_identifier = trim_string(in.string());
    }
};

template <>
struct action<key_value_value>
{
    template <typename ActionInput>
    static void apply(const ActionInput &in, Ini &ini)
    {
        ini.contents[ini.current_section_header][ini.current_statement_identifier] = trim_string(in.string());
    }
};

int main(int argc, char **argv)
{
    if (argc != 2)
        return std::cout << "Usage: " << argv[0] << " <input_file_path>" << std::endl, -1;
    pegtl::file_input in(argv[1]);

    Ini ini;
    std::cout << "Parsing result: " << pegtl::parse<grammar, action>(in, ini) << std::endl;
    // pegtl::standard_trace< grammar >( in );

    for (const auto &[section_header, contents] : ini.contents)
    {
        std::cout << ">> Section \"" << section_header << "\"" << std::endl
                  << std::endl;

        for (const auto &[key, value] : contents)
        {
            std::cout << "\"" << key << "\" = \"" << value << "\"" << std::endl;
        }

        std::cout << std::endl;
    }

    return 0;
}