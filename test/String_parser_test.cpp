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

TEST(Parser_test, make_response_empty) {
    const string const_str = "";
    const string const_ref_str = "";
    string result = make_correct_response(const_str);
    EXPECT_TRUE(const_ref_str == result);
}

TEST(Parser_test, make_response_only_numbers) {
    const string const_str = "1";
    const string const_ref_str = "1\n1";
    string result = make_correct_response(const_str);
    EXPECT_TRUE(const_ref_str == result);
}

TEST(Parser_test, make_response_only_numbers_2) {
    const string const_str = "1 2";
    const string const_ref_str = "1 2\n3";
    string result = make_correct_response(const_str);
    EXPECT_TRUE(const_ref_str == result);
}

TEST(Parser_test, make_response_only_numbers_sorting) {
    const string const_str = "2 1";
    const string const_ref_str = "1 2\n3";
    string result = make_correct_response(const_str);
    EXPECT_TRUE(const_ref_str == result);
}

TEST(Parser_test, make_response_only_numbers_sorting_2) {
    const string const_str = "1 1";
    const string const_ref_str = "1 1\n2";
    string result = make_correct_response(const_str);
    EXPECT_TRUE(const_ref_str == result);
}

TEST(Parser_test, no_numbers_make_response) {
    const string const_str = "apples, bananas, peaches and watermelon";
    const string const_ref_str = "apples, bananas, peaches and watermelon";
    string result = make_correct_response(const_str);
    EXPECT_TRUE(const_ref_str == result);
}

TEST(Parser_test, has_numbers_make_response) {
    const string const_str =
        "20 apples, 30 bananas, 15 peaches and 1 watermelon";
    const string const_ref_str = "1 15 20 30\n66";
    string result = make_correct_response(const_str);
    EXPECT_TRUE(const_ref_str == result);
}
