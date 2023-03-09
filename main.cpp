#include "MooreNeighbor.h"

#include <iostream>
#include <vector>

int main() {
    auto width = 10;
    auto height = 10;
    auto x = 5;
    auto y = 5;
    
    std::vector<std::pair<int, int>> neighbors10x10 = mooreNeighborhood(x, y, width, height);

    std::cout << "Moore Neighborhood of (" << x << "," << y << ") in a " << width << "x" << height << " grid: ";
    for (auto neighbor : neighbors10x10) {
        std::cout << "(" << neighbor.first << "," << neighbor.second << ") ";
    }
    std::cout << std::endl;

    width = 3;
    height = 3;
    x = 1;
    y = 1;
    std::vector<std::pair<int, int>> neighbors3x3 = mooreNeighborhood(x, y, width, height);

    std::cout << "Moore Neighborhood of (" << x << "," << y << ") in a " << width << "x" << height << " grid: ";
    for (auto neighbor : neighbors3x3) {
        std::cout << "(" << neighbor.first << "," << neighbor.second << ") ";
    }
    std::cout << std::endl;

    return 0;
}
