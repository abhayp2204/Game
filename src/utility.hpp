#include "apoc/variables.hpp"
#include "classes/world.hpp"
#include "classes/entity.hpp"

#ifndef UTILITY_H
#define UTILITY_H

using namespace std;

// Clear terminal
void clear()
{
    cout << "\033[2J\033[1;1H";
}

// mod(-7) = 7
float mod(float x)
{
    return x * ((x > 0)-0.5)*2;
}

// Toggle functions
bool pressKey(bool key, bool prevKey)
{
    return (!prevKey && key);
}

bool leaveKey(bool key, bool prevKey)
{
    return (prevKey && !key);
}

// Debug
void a()
{
    cout << "apple\n";
}
void b()
{
    cout << "bear\n";
}
void c()
{
    cout << "crab\n";
}
void d()
{
    cout << "dog\n";
}

#endif