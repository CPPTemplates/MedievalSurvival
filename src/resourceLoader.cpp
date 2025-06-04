#include "resourceLoader.h"
#include "include/optimization/handleError.h"
#include "folderList.h"
// std::vector<stdPath> dataPackPaths = {
//	minecraftVersionFolder,
//	dataPackFolder / L"default",
//	dataPackFolder / L"randomized loot",
// };
//  from lowest to highest priority: resource packs
//  we don'T need a separate folder for data packs. data and resource packs are the same, as they both modify server behavior.
std::vector<stdPath> resourcePackPaths =
{
	minecraftVersionFolder,
	resourcePackFolder / L"default",
	resourcePackFolder / L"sounds 1.16",
	resourcePackFolder / L"qol",
	resourcePackFolder / L"BDcraft Sound Pack",
	// resourcePackFolder / L"happy-v1-3",
	// resourcePackFolder / L"light levels 1.16",
	resourcePackFolder / L"creator pack",
	//resourcePackFolder / L"randomized loot",
	//resourcePackFolder / L"Light Mob Spawning 1.19"
	// resourcePackFolder / L"4thful",};
};


std::vector<stdPath> getResourceLocations(const stdPath& relativePath)
{
	std::vector<stdPath> foundLocations = std::vector<stdPath>();
	for (const stdPath& resourcePackPath : resourcePackPaths)
	{
		const stdPath currentPath = resourcePackPath / relativePath;
		if (std::filesystem::exists(currentPath))
		{
			foundLocations.push_back(currentPath);
		}
	}
	return foundLocations;
}
bool getLastResourceLocation(const stdPath& relativePath, stdPath& result)
{
	const auto& locations = getResourceLocations(relativePath);

	if (locations.size())
	{
		result = locations[locations.size() - 1];
	}
	// handleCrash(relativePath.wstring() + std::wstring(L" not found in any of the resource packs. working directory: ") + workingDirectory.wstring());
	return locations.size();
}
