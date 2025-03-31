#pragma once
#include "globalFunctions.h"


//if write: right = left
//else left = right
template<typename T>
inline void serialize(T& left, T& right, cbool& write)
{
	if (write)
	{
		right = left;
	}
	else
	{
		left = right;
	}
}

#define castout reinterpret_cast<const char*>//for writing to file streams
#define castin reinterpret_cast<char*>//for reading from file streams