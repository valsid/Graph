#ifndef RAND_H
#define RAND_H

#include <random>

class Rand
{
    Rand();

public:
    static int intNumber(int min, int max);

    static double doubleNumber(double min = 0, double max = 1);

    static double normalNumber(double mean, double stddev);

private:
    static int getSeed();

    static std::mt19937 randomNumberGenerator;
};

#endif // RAND_H
