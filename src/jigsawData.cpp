#include "jigsawData.h"
#include "nbt/nbtSerializer.h"
#include "idConverter.h"
void jigsawData::serializeMembers(nbtSerializer& s)
{
	blockData::serializeMembers(s);
	facingData::serializeMembers(s);
	s.serializeMembers(std::wstring(L"jigsaw name"), jigsawName);
	s.serializeMembers(std::wstring(L"target name"), targetName);
	s.serializeMembers(std::wstring(L"target pool name"), targetPoolName);
	s.serializeMembers(std::wstring(L"rollable"), rollable);
	s.serializeMembers(std::wstring(L"keep jigsaws"), keepJigsaws);
	s.serializeMembers(std::wstring(L"levels"), (int&)levels);
	idConverter::serializeID(s, std::wstring(L"turns into"), (itemID&)turnsInto, s.converter ? &s.converter->itemIDConverter : nullptr);
}
