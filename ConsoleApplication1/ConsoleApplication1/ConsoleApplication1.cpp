// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>

std::string fun()
{
	std::string data = "233";

	return std::move(data);
}

void fun2(std::string && str)
{
	int i = 5;
}

int main()
{
	std::string& re = fun();
	fun2(std::move(re));

    return 0;
}

