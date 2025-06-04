#pragma once
#include "resourceLoader.h"
struct TextureLoader : resourceLoader<resolutionTexture>{
private:

	virtual resolutionTexture* loadResource(const stdPath& resourcePath, const std::vector<stdPath>& locations) override;
};
extern TextureLoader globalLoader;