#pragma once
#include <iostream>

class Random
{
public:
	static float InRange(float min, float max)
	{
		srand(time(0));
		return min + (rand() / (RAND_MAX / (max - min)));
	}
	static int InRange(int min, int max)
	{
		//srand(time(0));
		return min + (rand() / (RAND_MAX / (max - min)));
	}
};