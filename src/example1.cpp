#include <iostream>
#include <string>
#include "tao/pegtl.hpp"

// Define a simple grammar consisting of a single rule.
struct my_grammar
    : tao::pegtl::star<tao::pegtl::any>
{
};

// Primary action class template.
template <typename Rule>
struct my_action
    : tao::pegtl::nothing<Rule>
{
};

// Specialise the action class template.
template <>
struct my_action<tao::pegtl::any>
{
   // Implement an apply() function that will be called by
   // the PEGTL every time tao::pegtl::any matches during
   // the parsing run.
   template <typename ActionInput>
   static void apply(const ActionInput &in, std::string &out)
   {
      // Get the portion of the original input that the
      // rule matched this time as string and append it
      // to the result string.
      out += in.string();
   }
};

template <typename ParseInput>
std::string as_string(ParseInput &in)
{
   // Set up the states, here a single std::string as that is
   // what our action requires as additional function argument.
   std::string out;

   // Start the parsing run with our grammar, action and state.
   tao::pegtl::parse<my_grammar, my_action>(in, out);

   // Do something with the result.
   return out;
}

int main()
{
   const char *input = "Some weird string";

   tao::pegtl::memory_input<> in(input, "");

   std::string something = as_string(in);

   std::cout << something << std::endl;

   return 0;
}