#pragma once

#include <random>

class RandomGenerator {
public:
    explicit RandomGenerator(std::random_device randomDevice = std::random_device());

    int generate(int bottom, int top) noexcept;

private:
    std::mt19937 m_Generator;
};

