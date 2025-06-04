#include "nbt/nbtSerializable.h"
#include "nbt/nbtSerializer.h"
void nbtSerializable::serializeMembers(nbtSerializer& s)
{
	//don'T throw an error, it could be that something just has no data to serialize
}

void nbtSerializable::serialize(nbtSerializer& s, const std::wstring& name)
{
	if (s.push<nbtDataTag::tagCompound>(name))
	{
		serializeMembers(s);
		s.pop();
	}
}

void nbtSerializable::clone(nbtSerializable& cloneTo)
{
	nbtCompound compound = nbtCompound(std::wstring(L"cloneCompound"), nbtDataTag::tagCompound);
	nbtSerializer s = nbtSerializer(compound, true, false);
	serializeMembers(s);
	s.write = false;
	cloneTo.serializeMembers(s);

}

bool nbtSerializable::compare(nbtSerializable& other)
{
	//check if the same nbt tags exist

	nbtCompound compareCompound = nbtCompound(std::wstring(L"compareCompound"), nbtDataTag::tagCompound);
	nbtSerializer compareSerializer = nbtSerializer(compareCompound, true, false);
	serializeMembers(compareSerializer);

	nbtCompound otherCompound = nbtCompound(std::wstring(L"otherCompound"), nbtDataTag::tagCompound);
	nbtSerializer otherSerializer = nbtSerializer(otherCompound, true, false);
	other.serializeMembers(otherSerializer);

	return compareCompound.compare(otherCompound);
}
bool nbtSerializable::serialize(const std::wstring& name, const stdPath& path, cbool& write)
{
	nbtCompound* compound = nullptr;
	if (write)
	{
		compound = new nbtCompound(name);
	}
	else
	{
		if (!nbtCompound::serialize(compound, write, path))
		{
			return false;
		}
	}
	nbtSerializer s = nbtSerializer(*compound, write);
	serializeMembers(s);

	if (write)
	{
		if (!nbtCompound::serialize(compound, write, path))
		{

			delete compound;
			return false;
		}
	}

	delete compound;
	return true;
}
std::wstring nbtSerializable::toString()
{
	nbtCompound compound = nbtCompound(std::wstring(L"getCompound"), nbtDataTag::tagCompound);
	nbtSerializer s = nbtSerializer(compound, true, false);
	serializeMembers(s);
	std::wstring resultingString;
	compound.serialize(true, resultingString);
	return resultingString;
}