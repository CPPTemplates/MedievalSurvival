#include "TextureLoader.h"
TextureLoader globalLoader = TextureLoader();

resolutionTexture* TextureLoader::loadResource(const stdPath& resourcePath, const std::vector<stdPath>& locations)
{
	cveci2& defaultSize = getImageSize(locations[0]); // the base size of the image will be the one of the lowest resource pack: the minecraft texture folder or the "default" resource pack.

	return new resolutionTexture(texture(resourcePath, true), defaultSize);
}
