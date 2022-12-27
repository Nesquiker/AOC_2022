#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

std::vector<double> convertToDecimal (const std::string& path) {
    std::ifstream data(path);
    std::vector<double> output;
    for (std::string line; std::getline(data, line);) {
        char d;
        int power = 0;
        const int base = 5;
        double c_base = 1;
        double delta = 0;
        double val = 0;
        while (!line.empty()) {
            d = line.back();
            line.pop_back();
            c_base = std::pow(base, power);
            if (d == '=') {
                delta = c_base * -2;
            } else if (d == '-') {
                delta = c_base * -1;
            } else if (d == '1') {
                delta = c_base;
            } else if (d == '2') {
                delta = c_base * 2;
            } else if (d == '0') {
                delta = 0;
            }
            val += delta;
            power++;
        }
        output.emplace_back(val);
    }
    return output;
}

std::string snafuHelper (const int base_5, const double val) {
    int last = 2;
    double base = std::pow(5, base_5);
    double comp_val = base * last;
    while (val < comp_val) {
        comp_val -= base;
        last--;
    }
    std::string snafu;
    if (last == 2) {
        snafu = "2";
    } else if (last == -3) {
        snafu =  "=";
        comp_val += base;
    } else {
        if (std::abs(comp_val + base - val) < std::abs(comp_val - val)) {
            comp_val += base;
            last++;
        }
        if (last == 2) {
            snafu = "2";
        } else if (last == 1) {
            snafu = "1";
        } else if (last == 0) {
            snafu = "0";
        } else if (last == -1) {
            snafu = "-";
        } else if (last == -2) {
            snafu = "=";
        }
    }
    if (base_5 == 0) {
        return snafu;
    } else {
        return snafu + snafuHelper(base_5 - 1, val - comp_val);
    }
}

std::string convertToSnafu (const double dec_val) {
    int max_base_5 = 0;
    double comp_val;
    for (int i = 0; i < 30; i++) {
        comp_val = std::pow(5, i) * 2;
        if (comp_val >= dec_val) {
            max_base_5 = i;
            break;
        }
    }
    return snafuHelper(max_base_5, dec_val);

}


std::string part_1 (const std::vector<double>& dec_values) {
    double total = 0;
    for (double val : dec_values) {
        total += val;
    }
    return convertToSnafu(total);
}

int main () {
    const std::string test_path = "../test.txt";
    const std::string real_path = "../input.txt";
    auto converted_test = convertToDecimal(test_path);
    auto converted_real = convertToDecimal(real_path);
    std::cout << part_1(converted_test) << "\n";
    std::cout << part_1(converted_real) << "\n";

    return 0;

}