#include "resourceLoader.h"
#include "include/optimization/handleError.h"
#include "folderList.h"
// std::vector<stdPath> dataPackPaths = {
//	minecraftVersionFolder,
//	dataPackFolder / L"default",
//	dataPackFolder / L"randomized loot",
// };
//  from lowest to highest priority: resource packs
//  we don't need a separate folder for data packs. data and resource packs are the same, as they both modify server behavior.
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
	resourcePackFolder / L"randomized loot",
	// resourcePackFolder / L"4thful",};
};

resourceLoader globalLoader = resourceLoader(resourcePackPaths);

const resolutionTexture& resourceLoader::getResource(const stdPath& relativePath)
{
	if (loadedTextures.contains(relativePath))
		return *loadedTextures[relativePath];
	const auto &locations = getResourceLocations(relativePath);
	csize_t& locationCount = locations.size();
	if (locationCount == 0)
	{
		handleError(relativePath.wstring() + std::wstring(L" not found in any of the resource packs. working directory: ") + workingDirectory.wstring());
	}
	csize_t &lastLocation = locationCount - 1;
	cveci2 &size = getImageSize(locations[0]); // the base size of the image will be the one of the lowest resource pack: the minecraft texture folder or the "default" resource pack.

	resolutionTexture *const &tex = loadTexture(locations[lastLocation], size);
	loadedTextures[relativePath] = tex;
	return *tex;
}

resolutionTexture *resourceLoader::loadTexture(const stdPath &path, cvec2 &defaultSize)
{
	return new resolutionTexture(texture(path, true), defaultSize);
}

std::vector<stdPath> resourceLoader::getResourceLocations(const stdPath &relativePath) const
{
	std::vector<stdPath> foundLocations = std::vector<stdPath>();
	for (const stdPath &resourcePackPath : resourcePackPaths)
	{
		const stdPath currentPath = resourcePackPath / relativePath;
		if (std::filesystem::exists(currentPath))
		{
			foundLocations.push_back(currentPath);
		}
	}
	return foundLocations;
}

bool resourceLoader::getLastResourceLocation(const stdPath &relativePath, stdPath &result) const
{
	const auto &locations = getResourceLocations(relativePath);

	if (locations.size())
	{
		result = locations[locations.size() - 1];
	}
	// handleCrash(relativePath.wstring() + std::wstring(L" not found in any of the resource packs. working directory: ") + workingDirectory.wstring());
	return locations.size();
}

resourceLoader::~resourceLoader()
{
	//delete all loaded textures
	for (const auto& item : loadedTextures) {
		delete item.second;
	}
}
