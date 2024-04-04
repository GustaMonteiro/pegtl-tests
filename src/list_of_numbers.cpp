// define a grammar
// define callback functions
// take an input
// parsing

#include <iostream>
#include "tao/pegtl.hpp"
#include "tao/pegtl/contrib/trace.hpp"

#include <sstream>

#include <vector>

namespace pegtl = tao::pegtl;

// 13123123,2123123123,-3123123,4,5,6

// basic definition of a rule
struct rule_name : pegtl::opt<>{};

struct integer : pegtl::seq<
    pegtl::opt<pegtl::one<'+', '-'>>,
    pegtl::plus<pegtl::digit>
>{};

struct list_of_numbers : pegtl::seq<
    TAO_PEGTL_STRING("["),
    pegtl::list<integer, pegtl::one<','>, pegtl::space>,
    TAO_PEGTL_STRING("]")
>{};

struct grammar : pegtl::must<list_of_numbers, pegtl::eof>{};

// basic definition of a group of callback functions
template<typename Rule>
struct action : pegtl::nothing<Rule>{};

template<>
struct action<integer> {
    template<typename ActionInput>
    static void apply(ActionInput& in, std::vector<int>& numbers) {
        std::stringstream ss (in.string());
        int num;
        ss >> num;
        numbers.push_back(num);
    }
};

int main (int argc, char** argv) {
    pegtl::argv_input in (argv, 1);

    std::vector<int> numbers;

    size_t res = pegtl::parse<grammar, action>(in, numbers);
    // size_t res = pegtl::standard_trace<grammar> (in);
    std::cout << res << std::endl;;

    if(res) {
        for(auto val : numbers)
            std::cout << val << " ";
        std::cout << std::endl;
    }


    return 0;
}
