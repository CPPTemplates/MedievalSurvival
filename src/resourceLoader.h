#pragma once
#include <map>
#include "include/filesystem/filemanager.h"
#include "include/math/graphics/resolutiontexture.h"
#include "folderList.h"
#include "include/optimization/handleError.h"

std::vector<stdPath> getResourceLocations(const stdPath& relativePath);
bool getLastResourceLocation(const stdPath& relativePath, stdPath& result);

template<typename T>
struct resourceLoader
{
	std::unordered_map<stdPath, T*> loadedResources = {};
	resourceLoader() {}
	inline const T& getResource(const stdPath& relativePath) {

		if (!loadedResources.contains(relativePath))
		{
			const auto& locations = getResourceLocations(relativePath);
			csize_t& locationCount = locations.size();
			if (locationCount == 0)
			{
				handleError(relativePath.wstring() + std::wstring(L" not found in any of the resource packs. working directory: ") + workingDirectory.wstring());
			}
			else {
				loadedResources[relativePath] = loadResource(locations[locationCount - 1], locations);
			}
		}
		return *loadedResources[relativePath];
	}
	inline const resolutionTexture& operator[](const stdPath& relativePath) {
		return getResource(relativePath);
	}
	inline ~resourceLoader() {
		//delete all loaded textures
		for (const auto& item : loadedResources) {
			delete item.second;
		}
	}
private:
	virtual T* loadResource(const stdPath& resourcePath, const std::vector<stdPath>& locations) = 0;
};
