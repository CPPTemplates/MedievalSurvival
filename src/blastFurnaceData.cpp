#include "stdafx.h"
#include "blastFurnaceData.h"

void blastFurnaceData::serializeMembers(nbtSerializer& s)
{
    furnaceData::serializeMembers(s);
    LinkableBlockData::serializeMembers(s);
}
