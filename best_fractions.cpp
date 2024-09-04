#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <queue>
#include <utility>
#include <numeric>
#include <iomanip>
#include <sstream>
#include <algorithm>

// Function to calculate gcd of two numbers
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return std::abs(a);
}

// Comparator for the priority queue in mode 1 (smallest delta first)
struct CompareDelta {
    bool operator()(const std::pair<double, std::pair<int, int>>& p1, const std::pair<double, std::pair<int, int>>& p2) {
        return p1.first > p2.first;
    }
};

// Comparator for the priority queue in mode 2 (minimize a + b while satisfying delta constraint)
struct CompareSum {
    bool operator()(const std::pair<std::pair<double, std::pair<int, int>>, int>& p1, const std::pair<std::pair<double, std::pair<int, int>>, int>& p2) {
        return p1.second > p2.second; // Compare based on a + b
    }
};

// Helper function to count decimal places in delta
int count_decimal_places(double num) {
    std::string num_str = std::to_string(num);
    size_t dot_pos = num_str.find('.');
    if (dot_pos == std::string::npos) {
        return 0;
    }
    return num_str.size() - dot_pos - 1;
}

// Helper function to determine required precision for delta
std::string format_delta(double delta, int original_precision) {
    if (delta == 0.0) {
        return "0";  // If delta is exactly 0, return "0"
    }
    
    std::stringstream ss;
    ss << std::fixed << std::setprecision(original_precision) << delta;
    std::string delta_str = ss.str();

    // Find the first non-zero digit after the decimal point
    size_t dot_pos = delta_str.find('.');
    if (dot_pos != std::string::npos) {
        size_t first_non_zero_pos = delta_str.find_first_not_of('0', dot_pos + 1);
        
        // Check if we need to remove trailing zeros
        if (first_non_zero_pos != std::string::npos && first_non_zero_pos + 1 < delta_str.size()) {
            // Check if the digit after the first significant non-zero digit is zero
            if (delta_str[first_non_zero_pos + 1] == '0') {
                // Trim the number of trailing zeros after the significant digits
                delta_str.erase(delta_str.find_last_not_of('0') + 1);
                
                // Remove the decimal point if there are no digits after it
                if (delta_str.back() == '.') {
                    delta_str.pop_back();
                }
            }
        }
    }
    
    return delta_str;
}




// Function to print the results
void print_results(const std::vector<std::pair<double, std::pair<int, int>>>& results, double provided_delta) {
    int delta_precision = count_decimal_places(provided_delta);
    
    std::cout << std::setw(8) << "a" << std::setw(8) << "b" << std::setw(15) << "a/b" << std::setw(15) << "delta" << std::endl;
    std::cout << "===============================================" << std::endl;
    
    for (const auto& result : results) {
        int a = result.second.first;
        int b = result.second.second;
        double ratio = static_cast<double>(a) / b;
        double delta_val = result.first;

        // Determine precision of a/b (at least delta_precision + 1)
        std::cout << std::setw(8) << a << std::setw(8) << b << std::setw(15)
                  << std::fixed << std::setprecision(delta_precision + 1) << ratio
                  << std::setw(15) << format_delta(delta_val, delta_precision) << std::endl;
    }
}

// Function to find the best fractions (two modes)
std::vector<std::pair<double, std::pair<int, int>>> search_best_fractions(double c, double delta, double time_limit, bool positive_delta_only, int mode) {
    if (mode == 1) {
        // Mode 1: Stop after finding first 10 valid results based on delta
        std::priority_queue<std::pair<double, std::pair<int, int>>, std::vector<std::pair<double, std::pair<int, int>>>, CompareDelta> best_results;
        
        clock_t start_time = clock();
        int b = 1;

        while (true) {
            double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
            if (elapsed_time > time_limit)
                break;

            int a_approx = std::round(c * b);

            for (int a = a_approx - 1; a <= a_approx + 1; ++a) {
                if (a <= 0 || b <= 0)
                    continue;

                if (gcd(a, b) == 1) {
                    double ratio = static_cast<double>(a) / b;
                    double diff = ratio - c;

                    if (positive_delta_only && diff < 0) {
                        continue;
                    }

                    diff = std::abs(diff);

                    if (diff <= delta) {
                        best_results.push({ diff, { a, b } });
                        if (best_results.size() > 10) {
                            best_results.pop();
                        }
                        if (best_results.size() == 10) {
                            std::vector<std::pair<double, std::pair<int, int>>> results;
                            while (!best_results.empty()) {
                                results.push_back(best_results.top());
                                best_results.pop();
                            }
                            std::reverse(results.begin(), results.end());
                            return results;
                        }
                    }
                }
            }
            ++b;
        }

        std::vector<std::pair<double, std::pair<int, int>>> results;
        while (!best_results.empty()) {
            results.push_back(best_results.top());
            best_results.pop();
        }
        std::reverse(results.begin(), results.end());
        return results;
    } else {
        // Mode 2: Minimize a + b for the 10 best results
        std::priority_queue<std::pair<std::pair<double, std::pair<int, int>>, int>, std::vector<std::pair<std::pair<double, std::pair<int, int>>, int>>, CompareSum> best_results;

        clock_t start_time = clock();
        int b = 1;

        while (true) {
            double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
            if (elapsed_time > time_limit)
                break;

            int a_approx = std::round(c * b);

            for (int a = a_approx - 1; a <= a_approx + 1; ++a) {
                if (a <= 0 || b <= 0)
                    continue;

                if (gcd(a, b) == 1) {
                    double ratio = static_cast<double>(a) / b;
                    double diff = ratio - c;

                    if (positive_delta_only && diff < 0) {
                        continue;
                    }

                    diff = std::abs(diff);

                    if (diff <= delta) {
                        int sum = a + b;
                        best_results.push({ { diff, { a, b } }, sum });
                        if (best_results.size() > 10) {
                            best_results.pop();
                        }
                    }
                }
            }
            ++b;
        }

        std::vector<std::pair<double, std::pair<int, int>>> results;
        while (!best_results.empty()) {
            results.push_back(best_results.top().first);
            best_results.pop();
        }
        std::reverse(results.begin(), results.end());
        return results;
    }
}

// Main function
int main() {
    bool console_input = true;
    double c, delta, time_limit;
    bool positive_delta_only = false;
    int mode = 1; // Mode 1: stop after first 10, Mode 2: minimize a+b
    
    if (console_input) {
        std::cout << "Enter the value for c (target ratio): ";
        std::cin >> c;
        std::cout << "Enter the delta (tolerance): ";
        std::cin >> delta;
        std::cout << "Enter the time limit (in seconds): ";
        std::cin >> time_limit;
        std::cout << "Should delta be positive only? (1 for yes, 0 for no): ";
        int delta_switch;
        std::cin >> delta_switch;
        positive_delta_only = delta_switch == 1;
        std::cout << "Enter mode (1: stop after first 10, 2: minimize a+b within time): ";
        std::cin >> mode;
    } else {
        c = 3.14159265;        // Example target ratio (approximation of pi)
        delta = 0.00001;   // Example tolerance
        time_limit = 5.0; // Example time limit
    }

    std::cout << "Searching for fractions close to " << c << " within a delta of " << delta
              << " and time limit of " << time_limit << " seconds." << std::endl;

    std::vector<std::pair<double, std::pair<int, int>>> results = search_best_fractions(c, delta, time_limit, positive_delta_only, mode);

    print_results(results, delta);

    return 0;
}