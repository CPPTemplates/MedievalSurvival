#pragma once
#include "filesystem/filemanager.h"
#include "globalFunctions.h"
struct nbtSerializer;
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