#include "resourceLoader.h"
#include "include/optimization/handleError.h"
#include "folderList.h"

resourceLoader* currentLoader = nullptr;

const resolutionTexture& resourceLoader::getResource(const stdPath& relativePath)
{
	const auto &locations = getResourceLocations(relativePath);
	if (locations.size() == 0)
	{
		handleError(relativePath.wstring() + std::wstring(L" not found in any of the resource packs. working directory: ") + workingDirectory.wstring());
	}
	csize_t &lastLocation = locations.size() - 1;
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