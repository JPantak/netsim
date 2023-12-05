#include <cstdlib>
#include <iostream>
#include <random>
#include <ctime>
 
std::random_device rd;
std::mt19937 rng(rd());
 
double probability_generator_1() {
    return std::generate_canonical<double, 10>(rng);
}
 
double probability_generator_2() {
    return (double) (rand() % RAND_MAX) / RAND_MAX;  // NIEZALECANE: mała losowość generowanego ciągu liczb...
}
 