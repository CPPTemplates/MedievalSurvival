#include "fireworkStarData.h"
#include "idConverter.h"
#include "nbt/nbtSerializer.h"
#include "nbt/serializeList.h"
#include "nbt/serializeVector.h"
void fireWorkStarData::serializeMembers(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"flicker"), flicker);
	serializeNBTValue(s, std::wstring(L"trail"), trail);
	idConverter::serializeID(s, std::wstring(L"shape"), shape, s.converter ? &s.converter->fireworkShapeIDConverter : nullptr);
	serializeNBTValue(s, std::wstring(L"colors"), colors);
	serializeNBTValue(s, std::wstring(L"fade colors"), fadeColors);
}