#pragma once

#include <algorithm>  // std::sort
#include <string>
#include <vector>

using std::sort;
using std::string;
using std::vector;

inline vector<int> find_numbers(const char* str) {
    vector<int> numbers;
    int result;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] >= '0' && str[i] <= '9') {  //a decimal number
            result = 0;
            while (str[i] >= '0' && str[i] <= '9') {
                result = result * 10 + str[i] - '0';
                i++;
            }
            numbers.push_back(result);
        }
    }
    return numbers;
}

inline string make_correct_response(string str) {
    char* str_ptr = &str[0];
    vector<int> numbers = find_numbers(str_ptr);
    if (numbers.empty()) {
        return str;
    }
    sort(numbers.begin(), numbers.end());  // N*logN
    string tmp = "";
    uint32_t sum = 0;
    for (const auto& i : numbers) {
        tmp += std::to_string(i) + " ";
        sum += i;
    }
    tmp.back() = '\n';
    tmp += std::to_string(sum);
    return tmp;
}