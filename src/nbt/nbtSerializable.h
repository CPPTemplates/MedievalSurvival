#pragma once
#include "filesystem/filemanager.h"
#include "globalFunctions.h"
#include "nbtSerializer.h"
struct nbtSerializable
{
	//ONLY CALL THIS FUNCTION IF YOU ARE SURE THERE ARE NOT TWO OF THOSE IN YOUR COMPOUND!
	virtual void serializeMembers(nbtSerializer& s);
	void serialize(nbtSerializer& s, const std::wstring& name = std::wstring());
	void clone(nbtSerializable& cloneTo);
	bool compare(nbtSerializable& other);
	bool serialize(const std::wstring& name, const stdPath& path, cbool& write);
	std::wstring toString();

	template<typename T>
	T clone();

};
template<typename T>
inline T nbtSerializable::clone()
{
	T clonedObject = T();
	clone(clonedObject);
	return clonedObject;
}
template<typename T>
concept nbtSerializableConcept = std::derived_from<T, nbtSerializable>;
template< nbtSerializableConcept T>
bool serializeNBTValue(nbtSerializer& s, const std::wstring& memberName, T& value) {

	if (s.push(memberName)) {
		value.serializeMembers(s);
		s.pop();
		return true;
	}
	return false;
}