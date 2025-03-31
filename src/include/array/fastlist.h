#pragma once
#include <vector>
#include "optimization/debug.h"
#include <stdexcept>

#include "interface/idestructable.h"
#include <filesystem>
#include <initializer_list>
#include <utility>
#include "globalFunctions.h"
#include "optimization/optimization.h"

// super fast solution for iterating through a list while deleting certain items and adding items
constexpr bool keepindicator = true;
constexpr bool eraseindicator = !keepindicator;
template <typename T>
struct fastList : IDestructable
{
	T *baseArray = nullptr;
	size_t size = 0;
	bool *mask = nullptr;
	size_t newSize = 0;
	std::vector<T> addList = std::vector<T>();

	inline explicit fastList(const size_t &size = 0, cbool &initializeToDefault = true)
		: baseArray(initializeToDefault ? new T[size]() : new T[size]),
		  size(size),
		  mask(nullptr),
		  newSize(size),
		  addList(std::vector<T>())
	{
	}

	inline fastList(const std::initializer_list<T> &values) : baseArray(new T[values.size()]),
															  size(values.size()),
															  mask(nullptr),
															  newSize(values.size()),
															  addList(std::vector<T>())
	{
		std::copy(values.begin(), values.end(), baseArray);
	}

	inline fastList(const fastList &other) : baseArray(new T[other.size]), size(other.size),
											 mask(other.mask ? new bool[other.size] : nullptr),
											 newSize(other.newSize),
											 addList(other.addList)
	{
		if (other.mask)
		{
			std::copy(other.mask, other.mask + other.size, mask);
		}

		std::copy(other.baseArray, other.baseArray + other.size, baseArray);
	}

	void swap(fastList<T> &with) noexcept
	{
		std::swap(addList, with.addList);
		std::swap(baseArray, with.baseArray);
		std::swap(mask, with.mask);
		std::swap(newSize, with.newSize);
		std::swap(size, with.size);
	}

	// inline fastList& operator=(fastList copy) = default;
	inline fastList &operator=(fastList copy)
	{
		this->swap(copy);
		return *this;
	}

	// for iterative functions
	inline T *begin() const
	{
		return baseArray;
	}
	inline T *end() const
	{
		return baseArray + size;
	}

	inline void removeDoubles()
	{
		update();
		std::vector<T> newArray = std::vector<T>();
		// remove all double elements from the list
		for (size_t i = 0; i < size; i++)
		{
			const T current = baseArray[i];
			if (std::find(newArray.begin(), newArray.end(), current) == newArray.end())
			{
				newArray.push_back(current);
			}
		}
		size = newArray.size();
		delete[] baseArray;
		baseArray = new T[size];
		std::copy(newArray.begin(), newArray.end(), baseArray);
	}

	// refresh the list (add and remove selected items)
	inline void update()
	{
		if (mask || size != newSize)
		{
			// needs to update
			// delete according to the mask: false = do not delete, true = delete
			T *newArray = new T[newSize];
			size_t newIndex = 0;
			// iterate through old array
			for (size_t oldindex = 0; oldindex < size; oldindex++)
			{
				if ((!mask) || mask[oldindex])
				{
					assumeInRelease(newIndex < newSize);
					newArray[newIndex] = baseArray[oldindex];
					++newIndex;
				} // else skip
			}
			if (addList.size())
			{
				// add the 'addlist' array
				std::copy(addList.begin(), addList.end(), &newArray[newIndex]);
			}
			// update variables
			size = newSize;
			if (mask)
			{
				delete[] mask;
				mask = nullptr;
			}
			addList.clear();
			delete[] baseArray;
			baseArray = newArray;
		}
	}
	inline void generateMask()
	{
		mask = new bool[size];
		std::fill(mask, mask + size, keepindicator);
	}
	inline void erase(const T &element)
	{
		csize_t &index = find(element);
		if (index != std::wstring::npos)
		{
			erase(index);
		}
	}
	inline void erase(csize_t &index)
	{
		if (!mask)
		{
			generateMask();
		}
		if (mask[index] != eraseindicator)
		{
			newSize--;
			mask[index] = eraseindicator;
		}
	}

	inline void erase(const T *const &pointer)
	{
		erase(pointer - baseArray);
	}

	// find the index of an element with a comparison function
	template <class compareFunction>
	inline size_t findFunction(const compareFunction &&function) const
	{
		for (size_t index = 0; index < size; index++)
		{
			if (function(baseArray[index]))
				return index;
		}
		return -1;
	}

	// find the index of an element
	inline size_t find(const T &element) const
	{
		for (size_t index = 0; index < size; index++)
		{
			if (baseArray[index] == element)
				return index;
		}
		return (size_t)-1;
	}

	// this is slow
	inline void insertAndUpdate(const T &element, size_t index)
	{
		// mask is null, that will continue to be so
		update();

		newSize = size + 1;
		T *newarray = new T[newSize];
		// 0 to index: old 0 to index
		std::copy(baseArray, baseArray + index, newarray);
		// index: new value
		newarray[index] = element;
		// index + 1 to size + 1: old index to size
		std::copy(baseArray + index, baseArray + size, newarray + index + 1);
		delete[] baseArray;
		baseArray = newarray;
		size = newSize;
	}
	inline void push_back(const T &element)
	{
		addList.push_back(element);
		newSize++;
	}
	inline void push_back(T *const begin, T *const end)
	{
		addList->insert(addList.end(), begin, end);
		newSize += end - begin;
	}
	inline void push_back(std::initializer_list<T> elements)
	{
		addList.insert(addList.end(), elements.begin(), elements.end());
		newSize += elements.size();
	}
	template <typename indexType>
	inline T &operator[](const indexType &index) const
	{
		if constexpr (isDebugging)
		{
			// index can'T be less than 0
			if ((size_t)index >= size)
			{
				throw std::out_of_range("index out of range");
			}
		}

		return baseArray[index];
	}

	inline void clear()
	{
		if (mask)
			delete[] mask;
		mask = nullptr;
		delete[] baseArray;
		baseArray = new T[0];
		size = 0;
		newSize = 0;
		addList.clear();
	}
	/*
	//update the list before doing this
	inline void deleteMembers() override
	{
		for (int i = 0; i < size; i++)
		{
			delete basearray[i];
		}
	}
	*/
	inline ~fastList() override
	{
		// dont delete the elements!
		delete[] baseArray;
		baseArray = nullptr;
		if (mask)
		{
			delete[] mask;
			mask = nullptr;
		}
	}
};

// WILL NOT SERIALIZE LIST MEMBERS
template <typename streamType, typename T>
inline void serializeList(streamType &stream, cbool &write, fastList<T> *&list)
{
	if (write)
	{
		serialize(stream, write, list->size);
	}
	else
	{
		size_t size = 0;
		serialize(stream, write, size);
		list = new fastList<T>(size, false);
	}
};
