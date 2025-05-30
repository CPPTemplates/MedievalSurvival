#include "attachmentDirectionData.h"
#include "nbt/nbtSerializer.h"
directionID attachmentDirectionData::getAttachmentDirection(tickableBlockContainer * /*containerIn*/, cveci2 & /*position*/) const
{
	return attachmentDirection;
}

void attachmentDirectionData::serializeMembers(nbtSerializer &s)
{
	s.serializeMembers(std::wstring(L"attachment direction"), attachmentDirection);
}