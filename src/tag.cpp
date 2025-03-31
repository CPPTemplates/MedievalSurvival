#include "filesystem/textfile.h"
#include "filesystem/jsonReader.h"
#include "tag.h"
#include "itemData.h"
fastList<tag *> tagList = fastList<tag *>();

tag *readTag(const std::wstring &tagName, const stdPath &tagDirectory)
{
	tag *T = new tag(tagName);

	const jsonContainer &content = readJson(stringToWString(readAllText(tagDirectory / (tagName + jsonFileExtension))));
	const size_t valuesIndex = content.getChildIndex(L"values");
	if (valuesIndex == std::wstring::npos)
		return T;

	const std::vector<jsonContainer> &taggedItemsContainer = content.children[valuesIndex].children;
	for (const jsonContainer &taggedItem : taggedItemsContainer)
	{
		std::wstring taggedItemName = taggedItem.value;
		if (taggedItemName[0] == L'#')
		{
			// this tag tagged a tag
			// remove the '#'
			std::wstring tagName = taggedItemName.substr(1);
			size_t currentTagID = getTagListIndexByName(tagName);
			if (currentTagID == std::wstring::npos)
			{
				// not loaded yet
				// pre-load it
				readTag(removeNameSpace(tagName), tagDirectory);
				// it will now certainly be found
				// we have no guarantee that it is at the end of the list, because the 'subtag' can have subtags too.
				currentTagID = getTagListIndexByName(tagName);
			}
			tag *taggedTag = tagList[currentTagID];
			if (taggedTag->taggedComparables->size) // useless if it does not have tagged items
			{
				T->taggedComparables->push_back(taggedTag);
			}
		}
		else
		{
			const itemID &identifier = itemList.getIDByName(taggedItemName);
			if ((int)identifier != -1)
			{
				itemData *taggedItem = itemList[identifier];
				T->taggedComparables->push_back(taggedItem);
			}
		}
	}
	T->taggedComparables->update();

	// add to the tag list either way to stop constantly loading from happening
	tagList.push_back(T);
	tagList.update();
	return T;
}
bool tag::hasTaggedItems()
{
	return false;
}

bool tag::compare(const itemID &itemToCompare) const
{
	for (IItemComparable *taggedComparable : *taggedComparables)
	{
		if (taggedComparable->compare(itemToCompare))
		{
			return true;
		}
	}
	return false;
}

tag::~tag()
{

	delete taggedComparables;
	taggedComparables = nullptr;
}
size_t getTagListIndexByName(std::wstring name)
{
	name = removeNameSpace(name);
	// std::function<bool(block*)> func = std::function<bool(block*)>([name](const auto& a) {return a->name == name; });
	// return itemtagList.find(func);
	return tagList.findFunction([&name](const auto &a)
								{ return a->name == name; });
}