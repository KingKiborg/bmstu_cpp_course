#include "base_algo_let.h"
#include <algorithm>

std::vector<int> positive_numbers(const std::vector<int>& v)
{
    std::vector<int> result;
    for (int num : v) {
        if (num > 0) {
            result.push_back(num);
        }
    }
    return result;
}

void sort_positive_numbers(std::vector<int>& v)
{
    std::vector<int> positives;
    for (int num : v) {
        if (num > 0) {
            positives.push_back(num);
        }
    }
    std::sort(positives.begin(), positives.end());
    size_t pos_index = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i] > 0) {
            v[i] = positives[pos_index];
            ++pos_index;
        }
    }
}

int sum_positive_numbers(const std::vector<int>& v)
{
    int sum = 0;
    for (int num : v) {
        if (num > 0) {
            sum += num;
        }
    }
    return sum;
}

bool is_divisible_by_10(const std::vector<int>& v)
{
    for (int num : v) {
        if (num % 10 != 0) {
            return false;
        }
    }
    return true;
}

void replace_negative_numbers(std::vector<int>& v)
{
    for (int& num : v) {
        if (num < 0) {
            num = 0;
        }
    }
}

void double_values(std::vector<int>& v)
{
    for (int& num : v) {
        num *= 2;
    }
}

void sort_students_by_age(std::vector<Student>& v)
{
    std::sort(v.begin(), v.end(), [](const Student& a, const Student& b) {
        return a.age < b.age;
    });
}

void sort_students_by_name(std::vector<Student>& v)
{
    std::sort(v.begin(), v.end(), [](const Student& a, const Student& b) {
        return a.name < b.name;
    });
}