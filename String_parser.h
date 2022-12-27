#pragma once

#include <algorithm>  // std::sort
#include <cstddef>    //size_t
#include <vector>

using std::sort;
using std::vector;

inline size_t keep_only_numbers(char* str) {
    size_t shift = 0;  // we are going to shift a decimal number
    size_t length = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] >= '0' && str[i] <= '9') {  //a decimal number
            str[i - shift] = str[i];
            length++;
        } else if (str[i] == ' ') {
            if ((i > shift) &&
                (str[i - shift - 1] == ' ')) {  //the separator must be one
                shift++;
            } else {
                str[i - shift] = str[i];
                length++;
            }
        } else {
            shift++;
        }
    }
    str[length] = '\0';
    return length;
}

inline void custom_atoi(char* str, vector<int>& numbers) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        int result = 0;

        for (; str[i] != ' ' && str[i] != '\0'; i++) {
            result = result * 10 + str[i] - '0';
        }
        numbers.push_back(result);
    }
}

inline void parse_string(char* str, vector<int>& numbers) {
    keep_only_numbers(str);
    custom_atoi(str, numbers);
    sort(numbers.begin(), numbers.end());  // N*logN
}