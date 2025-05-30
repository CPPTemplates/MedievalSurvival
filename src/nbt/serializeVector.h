#pragma once
#include "nbt/nbtSerializer.h"
#include "math/graphics/color/color.h"

template <typename T, fsize_t axisCount>
inline bool serializeNBTValue(nbtSerializer &s, const std::wstring &memberName, vectn<T, axisCount> &vector)
{
    if (s.push<nbtDataTag::tagCompound>(memberName))
    {
        for (fsize_t i = 0; i < axisCount; i++)
        {
            s.serializeMembers(axisNames[i], vector[i]);
        }

        s.pop();
        return true;
    }
    else
    {
        return false;
    }
}

template <typename T, fsize_t axisCount>
inline bool serializeNBTValue(nbtSerializer &s, const std::wstring &memberName, colortn<T, axisCount> &vector)
{
    return serializeNBTValue(s, memberName, (vectn<T, axisCount> &)vector);
}