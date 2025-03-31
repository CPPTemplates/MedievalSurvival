#pragma once
#include "type/conversion.h"
template<typename T>
struct nbtValue :nbtData
{
	T data = T();
	virtual inline bool serialize(const streamSerializer& s) override
	{
		static_assert(!std::is_pointer<T>::value, "no arrays");
		if (dataTag == nbtDataTag::tagUTF8String)
		{
			byte* ptr = (byte*)&data;
			s.serializeString<int16_t>(*(std::string*)ptr);
			//s.serializeWStringAsString<int16_t>(*((std::wstring*)ptr));
		}
		else
		{
			if constexpr (is_endian_convertable_v<T>) {
				s.serialize(data);
			}
		}
		return true;
	}
	virtual inline bool serialize(cbool& write, std::wstring& s) override
	{
		if (write)
		{
			if constexpr (std::is_integral<T>::value)
			{
				s = std::to_wstring(data);
			}
			else if constexpr (std::is_floating_point<T>::value)
			{
				s = std::to_wstring(data);
			}
			else if constexpr (std::is_same<T, std::wstring>::value)
			{
				s = data;
			}
			else
			{
				return false;
				//handleError(std::wstring(L"cant convert"));
			}
			return true;
		}
		else
		{
			return convertTo<T>(s, data);
		}
	}
	nbtValue(const std::wstring& name, const nbtDataTag& dataTag) :nbtData(name, dataTag), data(T()) {}
	virtual bool compare(const nbtData& other) const override;
};
template<typename T>
inline bool nbtValue<T>::compare(const nbtData& other) const
{
	return ((const nbtValue<T>&)other).data == data;
}