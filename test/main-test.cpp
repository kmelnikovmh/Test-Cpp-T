#include <gtest/gtest.h>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

namespace {

#ifdef _WIN32
const std::string exe_path =  "alice-sandbox-calc.exe";
#else
const std::string exe_path =  "./alice-sandbox-calc";
#endif

void write_file(const std::string& path, const std::string& content)
{
    std::ofstream f(path);
    f << content;
}

std::string read_file(const std::string& path)
{
    std::ifstream f(path);
    if (!f) return {};
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

int run_app(const std::string& input_file)
{
    std::string cmd = exe_path + " " + input_file;
    int rc = std::system(cmd.c_str());
#ifndef _WIN32
    rc = WEXITSTATUS(rc);
#endif
    return rc;
}

} // namespace

TEST(MainTest, NoArgs)
{
    int rc = std::system(exe_path.c_str());
#ifndef _WIN32
    rc = WEXITSTATUS(rc);
#endif
    EXPECT_EQ(rc, 1);
}

TEST(MainTest, NonexistentFile)
{
    std::string cmd = exe_path + " no_exist_file.txt";
    int rc = std::system(cmd.c_str());
#ifndef _WIN32
    rc = WEXITSTATUS(rc);
#endif
    EXPECT_EQ(rc, 1);
}

TEST(MainTest, TZExampleValidFull)
{
    std::string input =
        "5\n"
        "0 1,2 0 0 0 0\n"
        "1 0,3 5 2 1 15\n"
        "2 0,4 3 2 1 10\n"
        "3 1,4 1 0 2 40\n"
        "4 2,3,5 2 4 0 15\n"
        "5 4 0 5 4 10\n"
        "6 gems\n";
    write_file("test_input_tz_valid.txt", input);

    std::remove("result.txt");
    EXPECT_EQ(run_app("test_input_tz_valid.txt"), 0);

    std::string expected =
        "go 1\n"
        "state 1 5 2 1 15\n"
        "collect gems\n"
        "state 1 5 2 _ 15\n"
        "go 3\n"
        "state 3 1 0 2 40\n"
        "collect gems\n"
        "state 3 1 0 _ 40\n"
        "go 4\n"
        "state 4 2 4 0 15\n"
        "collect gold\n"
        "state 4 2 _ 0 15\n"
        "go 3\n"
        "state 3 1 0 _ 40\n"
        "go 1\n"
        "state 1 5 2 _ 15\n"
        "go 0\n"
        "result 0 4 3 0 182\n";

    EXPECT_EQ(read_file("result.txt"), expected);

    std::remove("test_input_tz_valid.txt");
    std::remove("result.txt");
}

TEST(MainTest, TZExampleInvalidFull)
{
    std::string input =
        "5\n"
        "0 1,2\n"
        "1 0,3 5 2 1 15\n"
        "2 0,4 3 2 1 10\n"
        "3 1,4 1 0 2 40\n"
        "4 2|5 2 4 0 15\n"
        "5 4 0 5 4 10\n"
        "6 gems\n";
    write_file("test_input_tz_invalid.txt", input);

    std::remove("result.txt");
    EXPECT_EQ(run_app("test_input_tz_invalid.txt"), 0);

    EXPECT_EQ(read_file("result.txt"), "4 2|5 2 4 0 15\n");

    std::remove("test_input_tz_invalid.txt");
    std::remove("result.txt");
}
