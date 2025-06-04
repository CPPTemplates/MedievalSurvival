#include "jigsawData.h"
#include "nbt/nbtSerializer.h"
#include "idConverter.h"
void jigsawData::serializeMembers(nbtSerializer& s)
{
	blockData::serializeMembers(s);
	facingData::serializeMembers(s);
	serializeNBTValue(s, std::wstring(L"jigsaw name"), jigsawName);
	serializeNBTValue(s, std::wstring(L"target name"), targetName);
	serializeNBTValue(s, std::wstring(L"target pool name"), targetPoolName);
	serializeNBTValue(s, std::wstring(L"rollable"), rollable);
	serializeNBTValue(s, std::wstring(L"keep jigsaws"), keepJigsaws);
	serializeNBTValue(s, std::wstring(L"levels"), (int&)levels);
	idConverter::serializeID(s, std::wstring(L"turns into"), (itemID&)turnsInto, s.converter ? &s.converter->itemIDConverter : nullptr);
}
