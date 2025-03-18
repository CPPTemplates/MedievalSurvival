#pragma once
#include "globalFunctions.h"
#include "array/fastlist.h"
#include "interface/idestructable.h"
struct jsonContainer
{
	std::wstring value = std::wstring();
	std::vector<jsonContainer> children = std::vector<jsonContainer>();
	size_t getChildIndex(const std::wstring& value) const;
	template <typename containerType>
	inline containerType* getChild(this containerType& container, const std::wstring& value) {
		csize_t& index = container.getChildIndex(value);
		if (index == std::wstring::npos)
		{
			return nullptr;
		}
		return &container.children[index];
	}
	template <typename containerType>
	inline containerType& operator[](this containerType& container, const std::wstring& name)
	{
		return *container.getChild<containerType>(name);
	}
};

jsonContainer readJson(std::wstring content);
