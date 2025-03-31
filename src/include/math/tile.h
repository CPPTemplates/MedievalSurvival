#pragma once
#include "fastList.h"
template<typename T, typename vectn>
struct tile:IDestructable
{
	vectn position = vec2();
	//the elements in this tile
	fastList<T>* elements = nullptr;
	tile(vectn position, fastList<T>* elements) 
	{
		this->position = position;
		this->elements = elements;
	}
	virtual ~tile() override
	{
		if (elements) 
		{

		}
		delete elements;
	}
};