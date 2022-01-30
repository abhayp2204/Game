#include "variables.hpp"
#include "world.hpp"
#include "entity.hpp"

#ifndef UTILITY_H
#define UTILITY_H

using namespace std;

void clear()
{
    cout << "\033[2J\033[1;1H";
}

float mod(float x)
{
    return x * ((x > 0)-0.5)*2;
}

bool pressKey(bool key, bool prevKey)
{
    return (!prevKey && key);
}

bool leaveKey(bool key, bool prevKey)
{
    return (prevKey && !key);
}

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