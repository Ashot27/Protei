#include "../String_parser.h"
#include <gtest/gtest.h>

using namespace std;

class Parser_test : public ::testing::Test {
   protected:
    Parser_test() {}

    ~Parser_test() override {}

    void SetUp() override {}

    void TearDown() override {}
};

TEST(Parser_test, keep_only_numbers_empty) {
    char const_str[] = "";
    char* str = const_str;
    auto length = keep_only_numbers(str);

    EXPECT_TRUE(length == 0);
    EXPECT_TRUE(strcmp(str, "") == 0);
}

TEST(Parser_test, keep_only_numbers_from_only_numbers) {
    char const_str[] = "1";
    char* str = const_str;
    auto length = keep_only_numbers(str);

    EXPECT_TRUE(length == 1);
    EXPECT_TRUE(strcmp(str, "1") == 0);
}

TEST(Parser_test, keep_only_numbers_from_only_numbers_2) {
    char const_str[] = "1 2 3 4 5";
    char* str = const_str;
    auto length = keep_only_numbers(str);

    EXPECT_TRUE(length == 9);
    EXPECT_TRUE(strcmp(str, "1 2 3 4 5") == 0);
}

TEST(Parser_test, keep_only_numbers_1) {
    char const_str[] = "1 2 3 4 a";
    char* str = const_str;
    auto length = keep_only_numbers(str);

    EXPECT_TRUE(length == 8);
    EXPECT_TRUE(strcmp(str, "1 2 3 4 ") == 0);
}

TEST(Parser_test, keep_only_numbers_2) {
    char const_str[] = "1 2 a 4 a";
    char* str = const_str;
    auto length = keep_only_numbers(str);

    EXPECT_TRUE(length == 6);
    EXPECT_TRUE(strcmp(str, "1 2 4 ") == 0);
}

TEST(Parser_test, keep_only_numbers_spaces) {
    char const_str[] = " ";
    char* str = const_str;
    auto length = keep_only_numbers(str);

    EXPECT_TRUE(length == 1);
    EXPECT_TRUE(strcmp(str, " ") == 0);
}

TEST(Parser_test, keep_only_numbers_spaces_2) {
    char const_str[] = "         ";
    char* str = const_str;
    auto length = keep_only_numbers(str);

    EXPECT_TRUE(length == 1);
    EXPECT_TRUE(strcmp(str, " ") == 0);
}

TEST(Parser_test, keep_only_numbers_from_string) {
    char const_str[] = "20 apples, 30 bananas, 15 peaches and 1 watermelon";
    char* str = const_str;
    auto length = keep_only_numbers(str);

    EXPECT_TRUE(length == 11);
    EXPECT_TRUE(strcmp(str, "20 30 15 1 ") == 0);
}

TEST(Parser_test, custom_atoi) {
    char const_str[] = "1 2 3 4 ";
    char* str = const_str;
    vector<int> numbers = {};
    custom_atoi(str, numbers);

    vector<int> ref_vector = {1, 2, 3, 4};
    EXPECT_TRUE(numbers == ref_vector);
}

TEST(Parser_test, custom_atoi_2) {
    char const_str[] = "111 111 ";
    char* str = const_str;
    vector<int> numbers = {};
    custom_atoi(str, numbers);

    vector<int> ref_vector = {111, 111};
    EXPECT_TRUE(numbers == ref_vector);
}

TEST(Parser_test, custom_atoi_3) {
    char const_str[] = "111 111 1 102";
    char* str = const_str;
    vector<int> numbers = {};
    custom_atoi(str, numbers);

    vector<int> ref_vector = {111, 111, 1, 102};
    EXPECT_TRUE(numbers == ref_vector);
}

TEST(Parser_test, parse_string_empty) {
    char const_str[] = "";
    char* str = const_str;
    vector<int> numbers = {};

    parse_string(str, numbers);

    vector<int> ref_vector = {};
    EXPECT_TRUE(numbers == ref_vector);
}

TEST(Parser_test, parse_string) {
    char const_str[] = "111 111 1 102";
    char* str = const_str;
    vector<int> numbers = {};

    parse_string(str, numbers);

    vector<int> ref_vector = {1, 102, 111, 111};
    EXPECT_TRUE(numbers == ref_vector);
}

TEST(Parser_test, parse_string_2) {
    char const_str[] = "20 apples, 30 bananas, 15 peaches and 1 watermelon";
    char* str = const_str;
    vector<int> numbers = {};

    parse_string(str, numbers);

    vector<int> ref_vector = {1, 15, 20, 30};
    EXPECT_TRUE(numbers == ref_vector);
}