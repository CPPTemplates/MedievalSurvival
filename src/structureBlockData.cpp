#include "structureBlockData.h"
#include "nbt/nbtSerializer.h"
#include "nbt/serializeVector.h"
void structureBlockData::serializeMembers(nbtSerializer& s)
{
	s.serializeMembers(std::wstring(L"structure name"), structureName);
	serializeNBTValue(s, std::wstring(L"relative structure position"), relativeStructurePosition);
	serializeNBTValue(s, std::wstring(L"structure size"), structureSize);
	s.serializeMembers(std::wstring(L"include entities"), includeEntities);
	s.serializeMembers(std::wstring(L"mirror x"), mirrorX);
	s.serializeMembers(std::wstring(L"structure rotation"), structureRotation);
	s.serializeMembers(std::wstring(L"structure integrity"), structureIntegrity);
	s.serializeMembers(std::wstring(L"structure seed"), structureSeed);
	s.serializeMembers(std::wstring(L"show bounding box"), showBoundingBox);
	s.serializeMembers(std::wstring(L"structure block mode"), mode);
}