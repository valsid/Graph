#include "rand.h"

#include <chrono>

std::mt19937 Rand::randomNumberGenerator(Rand::getSeed());

int Rand::intNumber(int min, int max)
{
    std::uniform_int_distribution<int> uid(min, max);
    return uid(randomNumberGenerator);
}

double Rand::doubleNumber(double min, double max)
{
    std::uniform_real_distribution<double> uid(min, max);
    return uid(randomNumberGenerator);
}

double Rand::normalNumber(double mean, double stddev)
{
    std::normal_distribution<double> uid(mean, stddev);
    return uid(randomNumberGenerator);
}

int Rand::getSeed()
{
    return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}
