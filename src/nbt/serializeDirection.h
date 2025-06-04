#include "math/direction.h"
#include "nbt/nbtSerializer.h"
inline bool serializeNBTValue(nbtSerializer& s, const std::wstring &memberName, directionID &direction)
{
    return serializeNBTValue(s, memberName, (sbyte &)direction);
}