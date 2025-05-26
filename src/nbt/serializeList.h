#pragma once
#include "nbt/nbtSerializer.h"

// for basic lists, use the serializeVariableArray function
template <typename T>
inline bool serializeNBTValue(nbtSerializer &s, const std::wstring &memberName, std::vector<T> &value)
{
    // CAUTION! this SFINAE construct uses double ::type! basically, it doesn'T get the ::type until the end of the evaluation, because not all ::type s are actually defined
    // std::common_type is just a trick to get a struct with ::type being T
    using signedT = std::conditional_t<std::is_integral_v<T>, std::make_signed<T>,
                                       std::conditional_t<std::is_enum_v<T>, std::underlying_type<T>,
                                                          std::conditional_t<std::is_same_v<T, bool>, std::common_type<bool>,
                                                                             std::common_type<T>>>>::type;
    // using signedT = std::conditional_t<std::is_integral_v<T>, std::make_signed_t<T>, T>;
    //  using signedT = std::is_integral_v<T> ? std::make_signed_t<T> : T;
    //constexpr auto dataTag = getListDataTag<signedT *>();
    if constexpr (getListDataTag<signedT *>() != nbtDataTag::tagEnd)
    {
        if (nbtData *currentChild = s.getOrCreateNBTData<signedT *>(memberName))
        {
            if (!s.write)
            {
                value.resize(((nbtDataArray<signedT> *)currentChild)->arraySize);
            }
            size_t count = value.size();
            signedT *ptr = value.size() ? (signedT *)&*value.begin() : nullptr; // ptr will not get modified because it's not null
            return s.serializeVariableArray(*currentChild, ptr, count);
        }
    }
    else
    {

        if (s.push<nbtDataTag::tagList>(memberName))
        {
            if (!s.write)
            {
                value = std::vector<T>(s.getChildren().size());
            }
            for (T &it : value)
            {
                serializeNBTValue(s, std::wstring(), it);
            }
            s.pop();
            return true;
        }
    }
    return false;
}

template <typename T>
inline bool serializeListOfSerializables(nbtSerializer &s, const std::wstring &memberName, std::vector<T> &value)
{
    if (s.push<nbtDataTag::tagList>(memberName))
    {
        if (!s.write)
        {
            value = std::vector<T>(s.getChildren().size());
        }
        //size_t i = 0;
        for (T &it : value)
        {
            if (s.push())
            {
                it.serializeValue(s);
                s.pop();
            }
        }
        s.pop();
        return true;
    }
    return false;
}