#pragma once
#include "constants.h"
#include "math/graphics/resolutiontexture.h"
#include <memory>
#include "itemID.h"
#include "filesystem/filemanager.h"
#include <audioCollection.h>

resolutionTexture* loadTextureFromResourcePack(const stdPath& relativePath, cbool& addToTextureList = true);
resolutionTexture* loadTexture(stdPath path, cvec2& defaultSize = cvec2(blockTextureSize), cbool& addToTextureList = true);
resolutionTexture* loadChestTexture(const stdPath& path);
void loadResourcePacks();

std::vector<resolutionTexture*> loadGrowthStageTextures(const std::wstring& blockName, std::vector<size_t> growthStageIndexes);
resolutionTexture* loadRotatedTexture(const stdPath& path, cvec2& defaultSize, cint& angle);
resolutionTexture* loadRailsTexture(const stdPath& path);

std::shared_ptr<audioCollection> getEquipSound(const itemID& identifier);
void unloadResourcePacks();

extern std::vector<stdPath> resourcePackPaths;
extern std::wstring creditsText;
extern std::vector<resolutionTexture*> loadedTextures;


extern fp guiTextureMultiplier;