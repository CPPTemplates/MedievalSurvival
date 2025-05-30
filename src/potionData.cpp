#include "potionData.h"
#include "nbt/nbtSerializer.h"
void potionData::serializeMembers(nbtSerializer& s)
{
	if (s.push<nbtDataTag::tagList>(std::wstring(L"active effects")))
	{
		if (s.write)
		{
			for (size_t i = 0; i < effectsToAdd.size(); i++)
			{
				if (s.push<nbtDataTag::tagCompound>())
				{
					effectsToAdd[i].serializeMembers(s);
					s.pop();
				}
			}
		}
		else
		{
			std::vector<nbtData*> serializedEffectList = s.getChildren();
			effectsToAdd.clear();
			int i = 0;
			for (nbtData* serializedEffect : serializedEffectList)
			{
				if (s.push(serializedEffect))
				{
					statusEffect effect = statusEffect();
					effect.serializeMembers(s);
					effectsToAdd.push_back(effect);
					s.pop();
				}
				i++;
			}
		}
		s.pop();
	}
}