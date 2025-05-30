#include "statusEffect.h"
#include "idConverter.h"
void statusEffect::serializeMembers(nbtSerializer& s)
{
	idConverter::serializeID(s, std::wstring(L"id"), identifier, s.converter ? &s.converter->statusEffectIDConverter : nullptr);
	s.serializeMembers(std::wstring(L"duration"), ticksDuration);
	s.serializeMembers(std::wstring(L"potency"), (int&)potency);
}
