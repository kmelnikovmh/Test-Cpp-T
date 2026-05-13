#include "parser.hpp"

#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

namespace alice_sandbox_calc {

namespace {

bool parse_num(const std::string& s, int& out)
{
    if (s.empty()
        || s[0] == '+'
        || s[0] == '-'
        || (1 < s.size() && s[0] == '0')) {
        return false;
    }
    char* end = nullptr;
    long val = std::strtol(s.c_str(), &end, 10);
    if (end != s.c_str() + s.size()) return false;
    out = val;
    return true;
}

std::vector<std::string> split(const std::string& s, char delim)
{
    std::vector<std::string> parts;
    std::istringstream iss(s);
    std::string token;
    while (std::getline(iss, token, delim)) {
        parts.push_back(token);
    }
    return parts;
}

std::vector<std::string> tokenize(const std::string& line)
{
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string tok;
    while (iss >> tok) {
        tokens.push_back(tok);
    }
    return tokens;
}

} // namespace

std::optional<Resource> parse_resource(std::string_view s)
{
    for (int i = 0; i < kResourceCount; ++i) {
        if (s == kResourceName[i]) {
            return static_cast<Resource>(i);
        }
    }
    return std::nullopt;
}

const char* resource_to_string(Resource r)
{
    return kResourceName[static_cast<int>(r)];
}

std::variant<ParsedInput, std::string> parse_input(std::istream& in)
{
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
        lines.push_back(line);
    }
    while (!lines.empty() && lines.back().empty()) {
        lines.pop_back();
    }
    if (lines.empty()) {
        return std::string{"error: empty input"};
    }

    int N = 0;
    auto tokens = tokenize(lines[0]);
    if (tokens.size() != 1 || !parse_num(tokens[0], N) || N < 1 || 255 < N) {
        return lines[0];
    }
    const int total = static_cast<int>(lines.size());
    if (total != N + 3) {
        return total < N + 3 ? lines.back() : lines[N + 2];
    }

    std::vector<Room> rooms(N + 1);
    for (int i = 0; i < N + 1; ++i) {
        const std::string& ln = lines[1 + i];
        auto toks = tokenize(ln);

        const bool size_ok = toks.size() == 6 || (i == 0 && toks.size() == 2);
        if (!size_ok) {
            return ln;
        }

        int id = -1;
        if (!parse_num(toks[0], id) || id != i) {
            return ln;
        }

        auto adj_parts = split(toks[1], ',');
        if (adj_parts.empty()) {
            return ln;
        }
        std::vector<int> nb_list;
        for (const auto& part : adj_parts) {
            int nb = -1;
            if (!parse_num(part, nb) || nb < 0 || N < nb || nb == id) {
                return ln;
            }
            nb_list.push_back(nb);
        }
        std::sort(nb_list.begin(), nb_list.end());

        int res[kResourceCount] = {};
        if (toks.size() == 6) {
            for (int r = 0; r < kResourceCount; ++r) {
                if (!parse_num(toks[2 + r], res[r]) || 255 < res[r] || (i == 0 && res[r] != 0)) {
                    return ln;
                }
            }
        }

        rooms[i].m_id = id;
        rooms[i].m_neighbors = std::move(nb_list);
        for (int r = 0; r < kResourceCount; ++r) {
            rooms[i].m_resources[r] = res[r];
        }
    }

    for (int i = 0; i < N + 1; ++i) {
        for (int nb : rooms[i].m_neighbors) {
            const auto& nb_adj = rooms[nb].m_neighbors;
            if (!std::binary_search(nb_adj.begin(), nb_adj.end(), i)) {
                return lines[1 + nb];
            }
        }
    }

    const std::string& last_line = lines[N + 2];
    auto last_toks = tokenize(last_line);
    int food = 0;
    auto demand_opt = last_toks.size() == 2
        ? parse_resource(last_toks[1]) : std::nullopt;
    if (last_toks.size() != 2
        || !parse_num(last_toks[0], food) || food < 2 || 255 < food
        || !demand_opt.has_value())
    {
        return last_line;
    }

    return ParsedInput{Dungeon(std::move(rooms)), food, *demand_opt};
}

} // namespace alice_sandbox_calc
