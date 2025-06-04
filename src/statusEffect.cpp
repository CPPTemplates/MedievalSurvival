#include "statusEffect.h"
#include "idConverter.h"
void statusEffect::serializeMembers(nbtSerializer& s)
{
	idConverter::serializeID(s, std::wstring(L"id"), identifier, s.converter ? &s.converter->statusEffectIDConverter : nullptr);
	serializeNBTValue(s, std::wstring(L"duration"), ticksDuration);
	serializeNBTValue(s, std::wstring(L"potency"), (int&)potency);
}
