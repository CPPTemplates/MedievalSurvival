#pragma once
#include "filesystem/iserializer.h"
#include "nbt/nbtCompound.h"
#include "nbtValue.h"
#include "nbtDataArray.h"
#include <concepts>
#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>
#include "globalFunctions.h"
#include "math/axis.h"
#include "optimization/debug.h"
#include "nbt/nbtData.h"
#include "nbt/nbtDataTag.h"
#include "type/types.h"
#include "array/arrayFunctions/arrayFunctions.h"
#include "optimization/handleError.h"

template <typename T>
constexpr nbtDataTag getListDataTag()
{
	if constexpr (std::is_same<T, int8_t*>())
	{
		return nbtDataTag::tagSignedInt8Array;
	}
	else if constexpr (std::is_same<T, int32_t*>())
	{
		return nbtDataTag::tagSignedInt32Array;
	}
	else if constexpr (std::is_same<T, int64_t*>())
	{
		return nbtDataTag::tagSignedInt64Array;
	}
	else
	{
		return nbtDataTag::tagEnd; // data tag not found
	}
}

template <typename T>
constexpr nbtDataTag getNBTDataTag()
{
	if constexpr (std::is_same<T, int8_t>())
	{
		return nbtDataTag::tagSignedInt8;
	}
	else if constexpr (std::is_same<T, int16_t>())
	{
		return nbtDataTag::tagSignedInt16;
	}
	else if constexpr (std::is_same<T, int32_t>())
	{
		return nbtDataTag::tagSignedInt32;
	}
	else if constexpr (std::is_same<T, int64_t>())
	{
		return nbtDataTag::tagSignedInt64;
	}
	else if constexpr (std::is_same<T, float>())
	{
		return nbtDataTag::tagFP32;
	}
	else if constexpr (std::is_same<T, double>())
	{
		return nbtDataTag::tagFP64;
	}
	else if constexpr (std::is_same<T, std::string>())
	{
		return nbtDataTag::tagUTF8String;
	}
	else
	{
		return getListDataTag<T>();
	}
}

template <typename T>
concept nbtType = getNBTDataTag<T>() != nbtDataTag::tagEnd;

// CAUTION!nbtListElemType does not use a pointer!
template <typename T>
concept nbtListElemType = getListDataTag<T*>() != nbtDataTag::tagEnd;

template <typename T>
concept enumType = std::is_enum_v<T>;

struct idConverter;
struct nbtSerializer : iSerializer
{
	nbtCompound& compound;
	const idConverter* converter = nullptr;
	std::vector<nbtCompound*> recursiveChildCompounds = std::vector<nbtCompound*>();
	std::vector<int> nextListIndexes = std::vector<int>();
	nbtCompound& getCurrentChildCompound() const;

	nbtSerializer(nbtCompound& compound, cbool& write, cbool& convert = true);

	// IMPORTANT: DO NOT READ AFTER A push<nbtDataTag::tagCompound>() WHICH RETURNED FALSE
	template <nbtDataTag tagType = nbtDataTag::tagCompound>
	bool push(const std::wstring& name = std::wstring())
	{
		nbtCompound* childCompound;
		childCompound = (nbtCompound*)getOrCreateNBTData<tagType>(name);
		if (childCompound == nullptr)
		{
			return false;
		}
		else
		{
			recursiveChildCompounds.push_back(childCompound);
			nextListIndexes.push_back(0);
			return true;
		}
		return childCompound != nullptr;
	}

	bool push(const nbtData* child);
	void pop();

	const std::vector<nbtData*>& getChildren() const;

	static bool isArrayTag(const nbtDataTag& dataTag);

	// string can only store short.maxValue amount of charachters!!!
	// can'T be static, because it's checking the write member
	template <nbtType T>
	inline bool serializeValue(nbtData& data, T& value) const
	{
		static_assert(std::is_same<T, std::string>::value || std::is_arithmetic<T>::value, "can'T convert to this type");

		if (write)
		{
			((nbtValue<T> &)data).data = value;
		}
		else
		{
			switch (data.dataTag)
			{
			case nbtDataTag::tagUTF8String:
				value = convertAnyType<std::string, T>(((nbtValue<std::string> &)data).data);
				break;
			case nbtDataTag::tagSignedInt8:
				value = convertAnyType<int8_t, T>(((nbtValue<int8_t> &)data).data);
				break;
			case nbtDataTag::tagSignedInt16:
				value = convertAnyType<int16_t, T>(((nbtValue<int16_t> &)data).data);
				break;
			case nbtDataTag::tagSignedInt32:
				value = convertAnyType<int32_t, T>(((nbtValue<int32_t> &)data).data);
				break;
			case nbtDataTag::tagSignedInt64:
				value = convertAnyType<int64_t, T>(((nbtValue<int64_t> &)data).data);
				break;
			case nbtDataTag::tagFP32:
				value = convertAnyType<float, T>(((nbtValue<float> &)data).data);
				break;
			case nbtDataTag::tagFP64:
				value = convertAnyType<double, T>(((nbtValue<double> &)data).data);
				break;
			default:
				return false;
			}
		}
		return true;
	}
	template <typename tFrom, nbtListElemType tTo>
	inline void copyArray(nbtDataArray<tFrom>& data, tTo*& dest, size_t& count) const
	{
		tFrom*& ptr = ((nbtDataArray<tFrom> &)data).data;
		if (dest)
		{
			//we cannot surpass the bounds of the array if it is created already
			count = math::minimum(count, ((nbtDataArray<tFrom> &)data).arraySize);
		}
		if (!dest)
		{
			count = ((nbtDataArray<tFrom> &)data).arraySize;
			dest = new tTo[count];
		}
		// value should not be cast to int8_t* because that could possibly corrupt memory
		copyAndCast(ptr, ptr + count, dest);
	}

	template <nbtListElemType T>
	inline bool serializeVariableArray(nbtData& data, T*& value, size_t& count) const
	{
		if (write)
		{
			((nbtDataArray<T> &)data).data = new T[count];
			((nbtDataArray<T> &)data).arraySize = count;
			std::copy(value, value + count, ((nbtDataArray<T> &)data).data);
		}
		else
		{
			switch (data.dataTag)
			{
			case nbtDataTag::tagSignedInt8Array:
			{
				copyArray((nbtDataArray<int8_t> &)data, value, count);
				// int8_t *&ptr = ((nbtDataArray<int8_t> &)data).data;
				// count = ((nbtDataArray<int8_t> &)data).arraySize;
				// if (!value)
				//{
				//	value = new T[count];
				// }
				//// value should not be cast to int8_t* because that could possibly corrupt memory
				// copyAndCast(ptr, ptr + count, value);
			}
			break;
			case nbtDataTag::tagSignedInt32Array:
			{
				copyArray((nbtDataArray<int32_t> &)data, value, count);
			}
			break;
			case nbtDataTag::tagSignedInt64Array:
			{
				copyArray((nbtDataArray<int64_t> &)data, value, count);
			}
			break;
			default:
				return false;
			}
		}
		return true;
	}

	template <nbtListElemType T>
	inline bool serializeArray(nbtData* data, T* value, size_t count = 1) const
	{
		return serializeVariableArray<T>(data, value, count);
	}
	// may return nullptr!
	template <nbtDataTag valueDataTag>
	inline nbtData* getOrCreateNBTData(const std::wstring& memberName)
	{
		// TODO: get asserts back
		// static_assert(valueDataTag != nbtDataTag::tagEnd, "this type is not allowed");

		nbtCompound& currentChildCompound = getCurrentChildCompound();
		nbtData* currentChild;

		if (currentChildCompound.dataTag == nbtDataTag::tagCompound)
		{
			if (memberName.length() == 0)
			{
				handleError(std::wstring(L"compound tags have names"));
			}
		}
		else
		{
			if (memberName.length())
			{
				handleError(std::wstring(L"list tags don'T have names"));
			}
		}

		if (write)
		{
			if (currentChildCompound.dataTag == nbtDataTag::tagCompound)
			{
				// check for multiple children with the same name
				auto it = std::find_if(currentChildCompound.children.begin(), currentChildCompound.children.end(), [&memberName](const auto& a)
					{ return a->name == memberName; });
				if (it != currentChildCompound.children.end())
				{
					handleError(std::wstring(L"multiple children with the same name"));
				}
			}
			if (currentChildCompound.dataTag == nbtDataTag::tagList)
			{
				if (currentChildCompound.children.size() && currentChildCompound.children[0]->dataTag != valueDataTag)
				{
					handleError(std::wstring(L"no different tags in a list; maybe you forgot to add a pop()"));
				}
			}
			currentChild = currentChildCompound.createNBTData(memberName, valueDataTag);
			currentChildCompound.children.push_back(currentChild);
			return currentChild;
		}
		else
		{
			if (currentChildCompound.dataTag == nbtDataTag::tagList)
			{
				csize_t& currentCompoundIndex = recursiveChildCompounds.size() - 1;

				// not a reference, make a copy, because the original will be incremented
				csize_t& listIndex = nextListIndexes[currentCompoundIndex];

				if (listIndex < currentChildCompound.children.size())
				{
					nextListIndexes[currentCompoundIndex]++;
					return currentChildCompound.children[listIndex];
				}
				else
				{
					return nullptr;
				}
			}
			else
			{
				if constexpr (isDebugging)
				{
					if (memberName == std::wstring())
					{
						handleError(std::wstring(L"compound tags have names"));
					}
				}
				auto it = std::find_if(currentChildCompound.children.begin(), currentChildCompound.children.end(), [&memberName](const auto& a)
					{ return a->name == memberName; });
				if (it == currentChildCompound.children.end()) // || (*it)->dataTag != valueDataTag)
				{
					return nullptr;
				}
				else
				{
					return *it;
				}
			}
		}
	}

	template <nbtType T>
	inline nbtData* getOrCreateNBTData(const std::wstring& memberName)
	{
		return getOrCreateNBTData<getNBTDataTag<T>()>(memberName);
	}

	// CAUTION! value should not be an undefined pointer! either initialize it with nullptr or an existing array!
	template <nbtType T>
	inline bool serializeVariableArray(const std::wstring& memberName, T*& value, size_t& count)
	{
		if (nbtData* currentChild = getOrCreateNBTData<T*>(memberName))
		{
			return serializeVariableArray(*currentChild, value, count);
		}
		else
		{
			return false;
		}
	}

	template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
	inline bool serializeVariableArray(const std::wstring& memberName, T*& value, size_t& count)
	{
		return serializeVariableArray(memberName, (std::make_signed_t<T> *&)value, count);
	}
	template <nbtType T>
	inline bool serializeValue(const std::wstring& memberName, T& value)
	{
		if (nbtData* currentChild = getOrCreateNBTData<T>(memberName))
		{
			return serializeValue(*currentChild, value);
		}
		else
		{
			return false;
		}
	}

	template <typename T>
	inline bool serializeArray(const std::wstring& memberName, T* value, size_t count)
	{
		return serializeVariableArray(memberName, value, count);
	}
	template <typename T, size_t count>
	inline bool serializeArray(const std::wstring& memberName, T(&value)[count])
	{
		return serializeVariableArray(memberName, value, count);
	}
	// inline bool serializeMembers(const std::wstring& memberName, byte& value)
	//{
	//	return serializeMembers(memberName, (sbyte&)value);
	// }


	
	// inline bool serializeMembers(const std::wstring& memberName, ushort& value)
	//{
	//	return serializeMembers(memberName, (short&)value);
	// }
	// inline bool serializeMembers(const std::wstring& memberName, uint& value)
	//{
	//	return serializeMembers(memberName, (uint&)value);
	// }
};
//fallback function
//make sure to have instantiated the templates by for example #include "nbt/serializeVector.h"!
template<nbtType T>
inline bool serializeNBTValue(nbtSerializer& s, const std::wstring& memberName, T& value) {
	return s.serializeValue(memberName, value);
}
template <enumType T>
inline bool serializeNBTValue(nbtSerializer& s, const std::wstring& memberName, T& value)
{
	return s.serializeValue(memberName, (std::underlying_type_t<T> &)value);
}
inline bool serializeNBTValue(nbtSerializer& s, const std::wstring& memberName, bool& value)
{
	return s.serializeValue(memberName, (sbyte&)value);
}
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
inline bool serializeNBTValue(nbtSerializer& s, const std::wstring& memberName, T& value)
{
	return s.serializeValue(memberName, (std::make_signed_t<T> &)value);
}
inline bool serializeNBTValue(nbtSerializer& s, const std::wstring& memberName, std::wstring& value)
{
	if (s.write)
	{
		std::string str = WStringToString(value);
		return serializeNBTValue(s, memberName, str);
	}
	else
	{
		std::string str;
		cbool& success = s.serializeValue(memberName, str);
		if (success)
		{
			value = stringToWString(str);
		}
		return success;
	}
}

template <typename T, size_t size>
inline bool serializeNBTValue(nbtSerializer& s, const std::wstring& memberName, T (& value)[size])
{
	bool success = s.push<nbtDataTag::tagList>(memberName);
	if (success) {
		//retrieve as many items as possible
		for (T& item : value) {
			if (!serializeNBTValue(s, L"", item)) {
				success = false;
				break;
			}
		}
		s.pop();
	}
	return success;
}