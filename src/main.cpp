#include "alice-bot.hpp"
#include "parser.hpp"
#include "simulator.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <variant>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }
    std::ifstream fin(argv[1]);
    if (!fin) {
        std::cout << "Error: cannot open " << argv[1] << '\n';
        return 1;
    }
    std::ofstream fout("result.txt");
    if (!fout) {
        std::cout << "Error: cannot open result.txt for writing\n";
        return 1;
    }
    
    auto result = alice_sandbox_calc::parse_input(fin);
    if (auto* bad = std::get_if<std::string>(&result)) {
        fout << *bad << '\n';
        return 0;
    }
    auto& input = std::get<alice_sandbox_calc::ParsedInput>(result);

    alice_sandbox_calc::AliceBot bot;
    auto actions = bot.run(input.m_dungeon, input.m_food, input.m_demand);
    alice_sandbox_calc::simulate(actions, input.m_dungeon, input.m_demand, fout);
}
