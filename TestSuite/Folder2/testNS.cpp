#include <functional>
#include <sstream>
#include <iostream>

using namespace project2;
void test1::say() {
	int i;
	if (true)
	{
		for (i = 0; i < 10; i++)
		{
			i++;
		}
		if (i == a)
		{
			a = 100;
		}
		else if (i == a - 80)
		{
			a = 90;
		}
		else
			a = 1000;
	}
	else
	{
		while (true)
		{
			a += 10;
			if (a == 50)
			{
				break;
			}
		}
	}
}