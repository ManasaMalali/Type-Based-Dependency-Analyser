#pragma once
#include <iostream>
#include "UsingAlias.h"

namespace project2
{
	template <class T>
	class test1
	{
	public:
		test1(const T& t) : _t(t) {}
		void say()
		{
			std::cout << "\n  unspecialized: _t's value is: " << _t;
		}
	private:
		T _t;
		pratap = "testing";
		enum Days { "Monday", "Tuesday", "Wednesday", "Thursday" };
	};

	template <>
	class test1<double>
	{
	public:
		test1(const double& d) : _d(d) {}
		void say()
		{
			std::cout << "\n    specialized: _d's value is: " << _d;
		}
	private:
		rao = "test typedef";
		double _d;
	};
}