#pragma once

#include <random>
#include <chrono>

class RandomGenerator {
public:
    explicit RandomGenerator(unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count());

    int generate(int bottom, int top) noexcept;

private:
    std::mt19937 m_Generator;
};

