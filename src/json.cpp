#include <iostream>
#include "tao/pegtl.hpp"
#include "tao/pegtl/contrib/trace.hpp"
#include <map>
#include <string>

namespace pegtl = TAO_PEGTL_NAMESPACE;

// {}
// "string"
// 5
// true
// null

// '{' + ALGUMACOISA + '}'
// TANTO CHAVE QUANTO VALOR SAO ENVOLTAS POR ""
// ALGUMACOISA ::= CHAVE:VALOR

typedef std::map<std::string, std::string> Content;

struct Json
{
    std::string key;
    Content content;
};

struct key_identifier : pegtl::identifier
{
};
struct key : pegtl::seq<
                 TAO_PEGTL_STRING("\""),
                 key_identifier,
                 TAO_PEGTL_STRING("\"")>
{
};

struct value_data : pegtl::plus<pegtl::sor<pegtl::alnum, TAO_PEGTL_STRING(" ")>>
{
};
struct value : pegtl::seq<
                   TAO_PEGTL_STRING("\""),
                   value_data,
                   TAO_PEGTL_STRING("\"")>
{
};

struct keyvalue : pegtl::seq<
                      key,
                      TAO_PEGTL_STRING(":"),
                      value>
{
};

struct object : pegtl::seq<
                    TAO_PEGTL_STRING("{"),
                    pegtl::star<pegtl::space>,
                    pegtl::list<keyvalue, TAO_PEGTL_STRING(","), pegtl::space>,
                    pegtl::star<pegtl::space>,
                    TAO_PEGTL_STRING("}")>
{
};

template <typename Rule>
struct action : pegtl::nothing<Rule>
{
};

template <>
struct action<key_identifier>
{
    template <typename ActionInput>
    static void apply(ActionInput &in, Json &json)
    {
        json.key = in.string();
    }
};

template <>
struct action<value_data>
{
    template <typename ActionInput>
    static void apply(ActionInput &in, Json &json)
    {
        json.content[json.key] = in.string();
    }
};

struct grammar : pegtl::seq<pegtl::star<pegtl::space>, object, pegtl::star<pegtl::space>, pegtl::eof>
{
};

int main()
{
    std::string input = R"(
        {
            "nome":"fulano",
            "idade":"15","endereco":"rua das frutas"
        }
    )";

    pegtl::string_input in(input, "");

    Json json;
    size_t res = pegtl::parse<grammar, action>(in, json);
    // size_t res = pegtl::standard_trace<grammar>(in);

    std::cout << res << std::endl;

    if (res)
        for (auto [key, value] : json.content)
            std::cout << "Chave: " << key << ", valor: " << value << std::endl;

    return 0;
}