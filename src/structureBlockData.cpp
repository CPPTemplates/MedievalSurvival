#include "structureBlockData.h"
#include "nbt/nbtSerializer.h"
#include "nbt/serializeVector.h"
void structureBlockData::serializeMembers(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"structure name"), structureName);
	serializeNBTValue(s, std::wstring(L"relative structure position"), relativeStructurePosition);
	serializeNBTValue(s, std::wstring(L"structure size"), structureSize);
	serializeNBTValue(s, std::wstring(L"include entities"), includeEntities);
	serializeNBTValue(s, std::wstring(L"mirror x"), mirrorX);
	serializeNBTValue(s, std::wstring(L"structure rotation"), structureRotation);
	serializeNBTValue(s, std::wstring(L"structure integrity"), structureIntegrity);
	serializeNBTValue(s, std::wstring(L"structure seed"), structureSeed);
	serializeNBTValue(s, std::wstring(L"show bounding box"), showBoundingBox);
	serializeNBTValue(s, std::wstring(L"structure block mode"), mode);
}