#pragma once
#include "type/conversion.h"
template<typename T>
struct nbtDataArray :nbtData
{
	size_t arraySize = 0;
	T* data = nullptr;
	virtual inline bool serialize(const streamSerializer& s) override
	{
		int intSize = (int)arraySize;
		s.serialize(intSize);

		if (!s.write)
		{
			arraySize = intSize;
			data = new T[arraySize];
		}
		s.serialize(data, arraySize);
		return true;
	}
	virtual inline bool serialize(cbool& write, std::wstring& s) override
	{
		if (write)
		{
			for (size_t i = 0; i < arraySize; i++)
			{
				s += std::to_wstring(data[i]);
				if (i < (arraySize - 1))
				{
					s += std::wstring(L",");
				}
			}
		}
		else
		{
			const wstringContainer& container = split_string(s, std::wstring(L","));
			arraySize = container.size();
			//size of container will always be 1 or more (when nothing is in the string, it will return 1 string containing "")
			assumeInRelease1(arraySize);
			data = new T[arraySize];
			for (size_t i = 0; i < arraySize; i++)
			{
				convertTo<T>(s, data[i]);
			}
		}
		return true;
	}
	nbtDataArray(const std::wstring& name, const nbtDataTag& dataTag) :nbtData(name, dataTag), arraySize(0), data(nullptr) {}
	inline virtual ~nbtDataArray() override
	{
		delete data;
	}
	inline bool serializeMembers(cbool& write, void* value, int& size)
	{
		if (write)
		{
			//copy array
			arraySize = size;
			data = new T[size];
			std::copy(value, (T*)value + size, data);
		}
		else
		{
			size = arraySize;
			value = new T[size];
			//copy array
			std::copy(data, data + arraySize, value);
		}
		return true;
	}
	virtual bool compare(const nbtData& other) const override;
};
template<typename T>
inline bool nbtDataArray<T>::compare(const nbtData& other) const
{
	return (((const nbtDataArray<T>&)other).arraySize == arraySize) &&
		(std::equal(data, data + arraySize, ((const nbtDataArray<T>&)other).data));
}