#pragma once
#include <map>
#include "include/filesystem/filemanager.h"
#include "include/math/graphics/resolutiontexture.h"
struct resourceLoader
{
    std::map<stdPath, resolutionTexture *> loadedTextures = {};
    std::vector<stdPath> resourcePackPaths = {};
    resourceLoader(std::vector<stdPath> resourcePackPaths) : resourcePackPaths(resourcePackPaths) {}
    const resolutionTexture& getResource(const stdPath& relativePath);
    resolutionTexture *loadTexture(const stdPath &path, cvec2 &defaultSize);
    std::vector<stdPath> getResourceLocations(const stdPath &relativePath) const;
    bool getLastResourceLocation(const stdPath &relativePath, stdPath &result) const;
};

extern resourceLoader* currentLoader;