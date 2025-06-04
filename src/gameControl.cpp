#include "math/collisions.h"
#include "overWorld.h"
#include "settings.h"
#include "cpuUsageID.h"
#include "gameControl.h"
#include "world.h"
#include "human.h"
#include "idAnalysis.h"
#include "dimension.h"
#include "musicManager.h"
#include "optimization/benchmark.h"
#include "block.h"
#include "dimensionData.h"
#include "benchMark.h"
#include "textureList.h"
#include "experienceOrb.h"
#include "gameModeData.h"
#include "array/arrayFunctions/arrayFunctions.h"
#include "entityData.h"
#include "keyID.h"
#include "musicList.h"
#include "application/mouseButton.h"
#include <cmath>
#include <string>
#include <vector>
#include "arrayDataType.h"
#include "blockData.h"
#include "itemID.h"
#include "chunkLoadLevel.h"
#include "collisionDataCollection.h"
#include "constants.h"
#include "dimensionID.h"
#include "entity.h"
#include "entityID.h"
#include "gameModeID.h"
#include "graphicsMode.h"
#include "humanoid.h"
#include "application/control/control.h"
#include "application/control/form/form.h"
#include "array/arraynd/arraynd.h"
#include "array/fastarray.h"
#include "globalFunctions.h"
#include "math/axis.h"
#include "math/graphics/brush/brushes.h"
#include "math/graphics/brush/transformbrush.h"
#include "math/graphics/color/color.h"
#include "math/graphics/resolutiontexture.h"
#include "math/graphics/texture.h"
#include "math/LayerNoiseSimplex.h"
#include "math/mathFunctions.h"
#include "math/mattnxn.h"
#include "math/rectangle/rectangletn.h"
#include "math/timemath.h"
#include "math/vector/vectn.h"
#include "levelID.h"
#include "lightLevel.h"
#include "lightLevelID.h"
#include "mob.h"
#include "gameRenderData.h"
#include "keyHistoryEvent.h"
#include "minecraftFont.h"
#include <SFML/Window.hpp>
#include "type/types.h"
#include "resourcePack.h"
#include "folderList.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include "math/graphics/brush/brushes/squareInterpolator.h"
#include "math/graphics/brush/brushes/colorMultiplier.h"
#include "math/graphics/brush/brushes/alphaMask.h"
#include "include/math/graphics/brush/brushes/repeatingBrush.h"
#include "include/math/graphics/brush/brushes/vignetteBrush.h"
#include "rectangularSlotContainer.h"
#include "humanSlotContainerUI.h"
#include "end.h"
#include "application/control/eventTranslator.h"
#include "resourceLoader.h"
#include "StartSoundPacket.h"
#include <soundList.h>
#include <TextureLoader.h>

constexpr rectangle2 crosshairTextureRect = crectangle2(0, 0, 15, 15);
constexpr rectangle2 iconTextureRect = rectangle2(0, 0, 9, 9);

constexpr int hotbarPadding = 1;
constexpr int hotbarMargin = 2;
constexpr int selectorMargin = 3;
constexpr rectangle2 hotbarTextureRect = crectangle2(0, 0, hotbarSpacing * StandardInventoryColumnCount + hotbarPadding * 2, hotbarSpacing + hotbarPadding * 2);
constexpr rectangle2 selectorTextureRect = crectangle2(0, 0, 24, 23);

microseconds lastTickTimeMicroseconds;
seconds lastTickTime;

void gameControl::render(cveci2& position, const texture& renderTarget)
{

	// update transform and stuff

	if (startCredits)
	{
		startCredits = false;
		currentCredits->visible = true;
		currentCredits->timeStartedReading = getmicroseconds();
		replaceMusic(creditsMusic.get());
		focusChild(currentCredits);
	}
	else
	{
		// measure the time point at which the game started drawing this frame; use for smooth 60fps animations
		microseconds newFrameTime = getmicroseconds();
		const microseconds& currentFrameStartMicroSeconds = newFrameTime;
		const seconds& newFrameStartSeconds = microsectosec(currentFrameStartMicroSeconds);
		const seconds& lastRenderTime = newFrameStartSeconds - currentFrameStartSeconds;
		currentFrameStartSeconds = newFrameStartSeconds;

		cfp& maximum = math::maximum(player->visibleRangeXWalk, requiredVisibleRangeXSprint);

		visibleRange.x = math::lerp(visibleRange.x,
			player->wantsToSprint ? maximum : player->visibleRangeXWalk,
			1 -
			pow(1 - visibleRangeTransitionSpeedPerSecond,
				(fp)lastRenderTime));
		renderGame(crectanglei2(position, rect.size), renderTarget, settings::renderHUD);
	}

	if (settings::renderDebugData)
	{
		text = std::wstring(L"medieval survival 1.") + std::to_wstring(currentFileVersionID) +
			std::wstring(L"\n");
		text += std::wstring(L"position:\tx:") + std::to_wstring(player->position.x) +
			std::wstring(L"\ty:") + std::to_wstring(player->position.y) + std::wstring(L"\n");
		text += std::wstring(L"speed:\tx:") + std::to_wstring(player->speed.x) +
			std::wstring(L"\ty:") + std::to_wstring(player->speed.y) + std::wstring(L"\n");

		cint& selectedBlockID = (int)player->selectedBlockContainer->getBlockID(
			player->selectedBlockPosition);
		text += std::wstring(L"selected block:\t") + blockList[selectedBlockID]->name + L"(" +
			std::to_wstring(selectedBlockID) + std::wstring(L")\n");
		blockData* selectedBlockData = player->selectedBlockContainer->getBlockData(
			player->selectedBlockPosition);
		if (selectedBlockData)
		{
			text += std::wstring(L"selected block data:\t") + selectedBlockData->toString() +
				L"\n";
		}
		text += std::wstring(L"selected light level:\tinternal sunlight:") + std::to_wstring(player->selectedBlockContainer->getInternalSunLightLevel(player->selectedBlockPosition)) +
			std::wstring(L"\tblock light:") + std::to_wstring(player->selectedBlockContainer->getBlockLightLevel(player->selectedBlockPosition)) +
			std::wstring(L"\tpower level:") + std::to_wstring(player->selectedBlockContainer->getPowerLevel(player->selectedBlockPosition)) + std::wstring(L"\n");

		if (player->dimensionIn->identifier == dimensionID::overworld)
		{
			overWorld* currentOverWorld = (overWorld*)player->dimensionIn;
			cvec2& coords = currentOverWorld->getBiomeTextureCoords(player->position.x);
			text += std::wstring(L"noise values:\tbiome texture coords:\tx:") +
				std::to_wstring(coords.x) +
				std::wstring(L"\ty:") + std::to_wstring(coords.y) +
				std::wstring(L"\ttemp:") +
				std::to_wstring(currentOverWorld->getTemperature(player->position)) +
				std::wstring(L"\tcloudiness:\t") + std::to_wstring(currentOverWorld->cloudThicknessNoise->evaluate(vec1(currentWorld->currentTime))) + std::wstring(L"\n");
		}

		cvec2& windSpeed = player->dimensionIn->getWindSpeed(player->position);

		text += std::wstring(L"wind speed:\tx") + std::to_wstring(windSpeed.x) +
			std::wstring(L"\ty") + std::to_wstring(windSpeed.y) + std::wstring(L"\n");

		text += std::wstring(L"biome:\t") + getClassName(*currentWorld->biomeList[(int)player->dimensionIn->getBiome(player->position)]) +
			std::wstring(L"\n");
		text += std::wstring(L"chunks loaded:");
		for (int i = 0; i < (int)dimensionID::count; i++)
		{
			if (currentWorld->dimensions[i]->loadedChunksMap.size())
			{
				text += std::wstring(L"\n") + dimensionDataList[i]->name + std::wstring(L":\t");
				text += std::to_wstring(currentWorld->dimensions[i]->loadedChunksMap.size()) +
					std::wstring(L":\t");
			}
		}
		text += std::wstring(L"\n");
		// text += std::wstring(L"sound count:\t") + std::to_wstring(playingSoundCount) +
		//         std::wstring(L"\n");
		text += std::wstring(L"fps:\t") + std::to_wstring(1.0 / microsectosec(currentBenchmark->measureTotalBenchmarkTime())) +
			std::wstring(L"\nmicroseconds used:\ntotal: ") +
			std::to_wstring(currentBenchmark->measureTotalBenchmarkTime()) +
			std::wstring(L"\nrendering: ") +
			std::to_wstring(currentBenchmark->measureBenchmarkTime(cpuUsageID::drawing)) +
			std::wstring(L"\nblock updates: ") +
			std::to_wstring(currentBenchmark->measureBenchmarkTime(cpuUsageID::blockUpdates)) +
			std::wstring(L"\nlight updates: ") +
			std::to_wstring(currentBenchmark->measureBenchmarkTime(cpuUsageID::lightUpdates)) +
			std::wstring(L"\nentities: ") +
			std::to_wstring(currentBenchmark->measureBenchmarkTime(cpuUsageID::entities)) +
			std::wstring(L"\nphysics: ") +
			std::to_wstring(currentBenchmark->measureBenchmarkTime(cpuUsageID::physics)) +
			std::wstring(L"\nchunk loading: ") +
			std::to_wstring(currentBenchmark->measureBenchmarkTime(cpuUsageID::chunkLoading)) +
			std::wstring(L"\nnetworking: ") +
			std::to_wstring(currentBenchmark->measureBenchmarkTime(cpuUsageID::networking)) +
			std::wstring(L"\nmiscellaneous: ") +
			std::to_wstring(currentBenchmark->measureBenchmarkTime(cpuUsageID::miscellaneous)) +
			std::wstring(L"\n");
		text += std::wstring(L"\n");

		renderText(position, renderTarget);
	}
	/*vec2 screenPos = targetData.worldToRenderTargetTransform.multPointMatrix(
		//player->exactMobIntersection
		//player->getHeadPosition()
	);
	fillCircleCentered(renderTarget, screenPos, vec2(10), brushes::red);
	*/

	currentWorld->currentChat.render(renderTarget, *this);
	renderChildren(position, renderTarget);
	if (focusedChild)
	{
		fillTransparentRectangle(crosshairTextureRect,
			crectangle2(cvec2(mousePositionPixels), cvec2()).expanded((fp)crosshairTextureRect.size.x * settings::videoSettings::guiScale), globalLoader[hudTextureFolder / "crosshair.png"],
			renderTarget);
	}
}

void gameControl::addSounds()
{
	if (windSoundID) {
		//update wind sound
	//attachedSounds.push_back(new AttachedSound(randomUUID(), ));
	//playSoundAtHead(windSound);
	//if (currentWindSound)
	//{
	//	if (currentWindSound->getStatus() == sf::SoundSource::Stopped) {
	//		currentWindSound->play();
	//	}
	//}
	//else {
	//	currentWindSound = windSound->playRandomSound();
	//}
	//
		cfp& windSpeed = player->dimensionIn->getWindSpeed(player->getHeadPosition()).length();
		static const transition<fp> speedToVolume = transition<fp>({
		   keyFrame<fp>(2,0),
		   keyFrame<fp>(20,1),
			});
		static const transition<fp> speedToPitch = transition<fp>({
		   keyFrame<fp>(6,1),
		   keyFrame<fp>(15,2),
			});

		windSoundVolume = math::lerp(windSoundVolume, speedToVolume.getValue(windSpeed), 0.2);
		windSoundPitch = math::lerp(windSoundPitch, speedToPitch.getValue(windSpeed), 0.2);
		const auto& updatePacket = std::make_shared < UpdateSoundPacket>(windSoundID);
		updatePacket->newVolume = windSoundVolume;
		updatePacket->newPitch = windSoundPitch;
		soundPacketsToSend.push_back(updatePacket);
	}
	else {
		//create wind sound
		windSoundID = randomUUID(currentRandom);
		soundPacketsToSend.push_back(std::make_shared<StartSoundPacket>(windSoundID, std::nullopt, windSound->key, 0, 1, 1, true));
	}
}

void gameControl::processInput()
{
	// this function will get called on other times than the tick() function.
	// form::mouseMove(mousePositionPixels, (mb) -1);
	for (const sf::Event& e : mostRecentInput.eventHistory)
	{
		// process keys
		if (const auto key = e.getIf<sf::Event::KeyPressed>())
		{
			cbool& worldFocus = getWorldFocus();
			if (key->code == (vk)keyID::escape)
			{
				if (focusedChild == nullptr)
				{
					options->visible = true;
					focusChild(options);
				}
				else if (focusedChild != videoOptions ||
					videoOptions->focusedChild == nullptr)
				{
					focusedChild->visible = false;
					focusChild(nullptr);
				}
			}
			else if (key->code == (vk)keyID::inventory)
			{
				if (inventoryUI->visible ^ worldFocus)
				{
					switchInventoryGUI();
				}
			}
			else if (worldFocus)
			{
				if (((key->code == (vk)keyID::commandLine) || (key->code == (vk)keyID::text)) &&
					focusedChild == nullptr)
				{
					commandLineTextbox->visible = true;
					focusChild(commandLineTextbox);

					if (key->code == (vk)keyID::text)
					{
						// avoid typing the text keybind
						return;
					}
				}

				if (key->code == (vk)keyID::renderHitboxes && settings::renderDebugData)
				{
					settings::renderHitboxes = !settings::renderHitboxes;
				}
				else if (key->code == (vk)keyID::debug)
				{
					settings::renderDebugData = !settings::renderDebugData;
				}
				else if (key->code == (vk)keyID::headUpDisplay)
				{
					settings::renderHUD = !settings::renderHUD;
				}
				else
				{
					for (int i = 0; i < 9; i++)
					{
						if (key->code == hotbarSlotKeys[i])
						{
							player->desiredRightHandSlotIndex = i;
							break;
						}
					}
				}
			}
			if (focusedChild == commandLineTextbox &&
				key->control && is_in(key->code, sf::Keyboard::Key::C, sf::Keyboard::Key::V))
			{
				if (key->code == sf::Keyboard::Key::V)
				{
					if (copyToClipboard.length())
					{
						// copied and pasted in the same frame
						paste(copyToClipboard);
					}
					else
					{
						wantsClipboardInput = true;
					}
				}
				else
				{
					copyToClipboard = copy();
				}
				continue;
			}
		}
		translator->processEvent(e);
	}
	// process buttons
	// control *highestChild = getHighestChild(mousePositionPixels);
	// prevent from focusing another child
	if (focusedChild)
	{
		// if (highestChild == focusedChild) {
		//     for (mb button = (mb) 0; button < mb::ButtonCount; button = (mb) ((byte) button + 1)) {
		//         if (mostRecentInput.clicked[button]) {
		//             form::mouseDown(mousePositionPixels, button);
		//         }
		//         if (mostRecentInput.clickReleased[button]) {
		//             form::mouseUp(mousePositionPixels, button);
		//         }
		//     }
		// }
		//  clickedFocused can be modified safely, because every time the clientInput gets modified, processInput() is called right after
		fillAllElements(clickedFocused, false);
	}
	else
	{
		if (!touchInput)
		{
			unFocusedMousePosition = mousePositionPixels;
		}
		cfp& hudScale = getHUDScale();
		rectangle2 hotbarDrawRect = rectangle2((rect.w - (hotbarTextureRect.size.x * hudScale)) / 2,
			0, hotbarTextureRect.size.x * hudScale,
			hotbarTextureRect.h * hudScale);
		veci2 pos;
		if (player->hotbarSlots->getSlot(mousePositionPixels, cveci2(hotbarDrawRect.pos0 + (hotbarSpacing - hotbarItemDisplaySize) / 2 * hudScale),
			(int)(hotbarSpacing * hudScale),
			(int)(hotbarItemDisplaySize * hudScale), pos))
		{
			player->desiredRightHandSlotIndex = pos.x;
			fillAllElements(mostRecentInput.clicked, false);
		}
		std::copy(clicked, clicked + sf::Mouse::ButtonCount, clickedFocused);
	}
}

void gameControl::updateTransforms(cvec2& headPosition)
{

	// update the mouse position according to the transform
	// mousePositionPixels = newMousePositionPixels;
	// convert to position in world

	// if (currentGame->focusedChild == nullptr)
	//{

	// the amount of pixels each block occupies on the screen
	cfp& pixelsPerBlock = rect.size.x / (visibleRange.x * 2);
	if (touchInput)
	{
		cameraPosition = headPosition;
	}
	else
	{
		cmat3x3 mouseLookingTransform = getWorldToScreenTransform(headPosition, pixelsPerBlock);
		cmat3x3 inverseMouseLookingTransform = mouseLookingTransform.inverse();
		cameraPosition = inverseMouseLookingTransform.multPointMatrix(
			cvec2(unFocusedMousePosition));
	}

	worldToRenderTargetTransform = getWorldToScreenTransform(cameraPosition, pixelsPerBlock);
}

void gameControl::renderGame(crectanglei2& rect, const texture& renderTarget, cbool& renderHUD)
{
	// start drawing

	// make the image transparent. that way, the sky will only be rendered to parts which are not transparent.
	renderTarget.fill(colorPalette::transparent);

	cfp& secondsBetweenTickAndRender = (fp)(currentFrameStartSeconds - lastTickTime);
	player->updateBodyParts();
	vec2 headPosition = player->getHeadPosition();
	if (player->dimensionIn == player->newDimension)
	{
		headPosition += (player->newPosition - player->position) *
			math::minimum(secondsBetweenTickAndRender, secondsPerTick) *
			ticksPerRealLifeSecond;
	}
	updateTransforms(headPosition);

	// draw the chunk
	visibleRange.y = (fp)rect.size.y / rect.size.x * visibleRange.x;
	cfp& hudScale = getHUDScale();

	//}

	// already update the selectedblockpos so it is drawn correctly
	player->updateSelection();
	dimension* dimensionIn = player->dimensionIn;

	const gameRenderData& targetData = getRenderData(renderTarget,
		math::minimum(secondsBetweenTickAndRender,
			secondsPerTick));

	crectangle2& worldRect = targetData.renderTargetToWorldTransform.multRectMatrix(
		crectangle2(targetData.renderTarget.getClientRect()));

	// render blocks and lightlevels

	veci2 pos00 = floorVector(worldRect.pos0);
	veci2 pos11 = floorVector(worldRect.pos1());

	pos11 = pos11 + 1; // add 1 to also draw the last block
	rectanglei2 blocksToBeDrawn = rectanglei2(pos00, pos11 - pos00);

	pos00 = blocksToBeDrawn.pos0;
	pos11 = blocksToBeDrawn.pos1();

	// the distance from which light levels will be interpolated
	constexpr int averageDistance = 1;
	constexpr int averageDistance2 = averageDistance * 2;
	constexpr int averageDiametre = 1 + averageDistance2;

	//the height in blocks at which the sky will start rendering. this to prevent being able to see the sky in unlit caves
	constexpr int skyRenderHeight = 0;

	crectanglei2 surroundingArrayRect = crectanglei2(blocksToBeDrawn.pos0 - averageDistance,
		blocksToBeDrawn.size + averageDistance * 2);
	// csize_t &surroundingArraySize = surroundingArrayRect.w * surroundingArrayRect.h;

	array2d<lightLevel> surroundingLightLevels[(size_t)lightLevelID::count];

	for (int lightDataTypeIndex = 0;
		lightDataTypeIndex < (int)lightLevelID::count; lightDataTypeIndex++)
	{
		const arrayDataType& dataType = (arrayDataType)((int)arrayDataType::levelType +
			(int)levelID::light +
			(int)lightDataTypeIndex);
		surroundingLightLevels[lightDataTypeIndex] = player->dimensionIn->getArrayValues<lightLevel>(
			surroundingArrayRect, dataType, chunkLoadLevel::worldGenerationLoaded);
	}

	const array2d<blockID>& surroundingBlocks = player->dimensionIn->getArrayValues<blockID>(
		surroundingArrayRect, arrayDataType::blockIDType,
		chunkLoadLevel::worldGenerationLoaded);

	enum LightState : byte {
		shaded = 0b0,//not completely unlit, but also not completely lit
		black = 0b1,//completely unlit
		white = 0b10,//completely lit
		cullBlock = 0b100//when set, this block should be culled from rendering. below y = 0, when the light level = 0, we don't render anything.
	};
	const auto& lightStates = array2d<LightState>(surroundingArrayRect.size);
	const array2d<squareInterpolator<true>>& interpolators = array2d<squareInterpolator<true>>(
		surroundingArrayRect.size);

	// this array specifies which coordinates on the x and y axis are going to be rendered. this optimizes rendering when zoomed out so far that blocks are smaller than one pixel.
	const vect2<fastArray<bool>>& shouldRenderAxis =
		vect2<fastArray<bool>>(
			fastArray<bool>(blocksToBeDrawn.size.x),
			fastArray<bool>(blocksToBeDrawn.size.y));

	// calculate shouldRenderAxis
	for (size_t axis = 0; axis < 2; axis++)
	{
		for (int i = 0; i < blocksToBeDrawn.size[axis]; i++)
		{
			rectangle2 blockRect = rectangle2();
			blockRect.pos0[axis] = (fp)i + pos00[axis];
			blockRect.size[axis] = 1;

			crectanglei2& screenRect = ceilRectangle(
				targetData.worldToRenderTargetTransform.multRectMatrix(blockRect));

			shouldRenderAxis[axis][i] = screenRect.size[axis];
		}
	}

	// calculate if the light color on each block is black, white, or anything between
	for (cveci2& renderPosition : blocksToBeDrawn)
	{
		cveci2& relativePosition = renderPosition - pos00;
		if (shouldRenderAxis[axisID::y][relativePosition.y])
		{
			if (shouldRenderAxis[axisID::x][relativePosition.x])
			{
				if (currentWorld->xray)
				{
					lightStates.setValueUnsafe(relativePosition, LightState::white);
				}
				else
				{
					bool enclosed = true;

					for (int averageY = 0; averageY < averageDiametre; averageY++)
					{
						for (int averageX = 0; averageX < averageDiametre; averageX++)
						{
							cveci2 absoluteArrayPosition = cveci2(relativePosition.x + averageX,
								relativePosition.y + averageY);
							block* const& blockToCheck = blockList[(int)surroundingBlocks.getValue(
								absoluteArrayPosition)];

							for (size_t i = 0; i < 2; i++)
							{
								if (blockToCheck->filterStrength[i] < maxLightLevel ||
									surroundingLightLevels[i].getValue(absoluteArrayPosition))
								{
									enclosed = false;
									goto notEnclosed;
								}
							}
						}
					}
				notEnclosed:;

					LightState& state = lightStates.getValueReferenceUnsafe(relativePosition);
					if (enclosed)
					{
						state = (LightState)(LightState::black | LightState::cullBlock);
					}
					else
					{
						state = (LightState)(LightState::black | LightState::white);
						colorf* cornerColorPtr = interpolators.getValueReferenceUnsafe(
							cveci2(relativePosition))
							.cornerColors;
						// for each corner
						for (int cornerY = relativePosition.y;
							cornerY < relativePosition.y + 2; cornerY++)
						{
							for (int cornerX = relativePosition.x;
								cornerX < relativePosition.x + 2; cornerX++, cornerColorPtr++)
							{
								// for each light level around 'corner'
								lightLevel maxLightLevel[(size_t)lightLevelID::count]{};
								for (int cornerCornerY = cornerY;
									cornerCornerY < cornerY + averageDistance2; cornerCornerY++)
								{
									for (int cornerCornerX = cornerX; cornerCornerX < cornerX +
										averageDistance2;
										cornerCornerX++)
									{
										for (size_t lightDataTypeIndex = 0; lightDataTypeIndex <
											(int)lightLevelID::count;
											lightDataTypeIndex++)
										{
											maxLightLevel[lightDataTypeIndex] = math::maximum(
												maxLightLevel[lightDataTypeIndex],
												surroundingLightLevels[lightDataTypeIndex].getValue(
													cveci2(cornerCornerX, cornerCornerY)));
										}
									}
								}
								*cornerColorPtr = dimensionIn->getColorMultiplier(maxLightLevel[0],
									maxLightLevel[1]);
								if (*cornerColorPtr != colorfPalette::black)
								{
									//can'T be black anymore
									state = (LightState)(state & ~LightState::black);
								}
								if (*cornerColorPtr != colorfPalette::white)
								{
									//can'T be white anymore
									state = (LightState)(state & ~LightState::white);
								}
							}
						}
					}
				}
			}
		}
	}
	// render entities
	std::vector<entity*> nearEntities = dimensionIn->findNearEntities(cameraPosition,
		visibleRange.length() +
		mobSizeMargin);
	for (entity* e : nearEntities)
	{
		if (collides2d(worldRect, e->calculateHitBox()))
		{
			// move the entity a little, to match speed
			cvec2& movement = e->getRenderOffset(targetData);
			gameRenderData entityRenderData = gameRenderData(
				mat3x3::cross(targetData.worldToRenderTargetTransform,
					mat3x3::translate(movement)),
				targetData.renderTarget,
				targetData.screen, targetData.secondsOffset);
			e->render(entityRenderData);
		}
	}
	// draw blocks
	for (cveci2& renderPosition : blocksToBeDrawn)
	{
		cveci2& relativePosition = renderPosition - pos00;
		if (shouldRenderAxis[axisID::y][relativePosition.y])
		{
			if (shouldRenderAxis[axisID::x][relativePosition.x])
			{
				// fill block texture
				const blockID& identifier = surroundingBlocks.getValue(
					relativePosition + averageDistance);
				block* b = blockList[(int)identifier];

				crectangle2& blockScreenRect = targetData.worldToRenderTargetTransform.multRectMatrix(
					crectangle2(renderPosition, cveci2(1)));
				LightState& state = lightStates.getValueReferenceUnsafe(relativePosition);
				if (!(state & LightState::black) || (dimensionIn->identifier == dimensionID::overworld && renderPosition.y >= skyRenderHeight && !blockList[surroundingBlocks.getValueUnsafe(relativePosition + averageDistance)]->willFillSquare))
				{
					// render block
					b->render(targetData, dimensionIn->getBlockData(renderPosition), dimensionIn,
						renderPosition);
				}
				else {
					//block not rendered
					state = (LightState)(state | LightState::cullBlock);
				}

			}
		}
	}
	rectangle2 blockScreenRect;
	veci2 selectionPosToDraw = player->selectedBlockPosition; // break block pos
	if (blockList[(int)player->selectedBlockContainer->getBlockID(
		selectionPosToDraw)]
		->canReplaceBlock)
	{
		selectionPosToDraw = player->adjacentBlockPosition; // cant break blocks, so show the position where to place blocks
	}

	blockScreenRect = mat3x3::cross(targetData.worldToRenderTargetTransform,
		player->selectedBlockContainer->containerToRootTransform)
		.multRectMatrix(
			crectangle2(cvec2(selectionPosToDraw) +
				(player->selectedBlockContainer->speedInRoot *
					targetData.secondsOffset),
				cvec2(1)));

	if (player->selectedBlockDamage)
	{
		// draw dig texture
		// the dig texture also has to have shade
		cfp& destroyStage = player->selectedBlockDamage /
			(blockList[(int)player->selectedBlockContainer->getBlockID(
				player->selectedBlockPosition)]
				->hardness *
				10); // 0 to 1
		// a value from 0 to 8, 10 options (-1 = not started digging, 9 = broken)
		size_t textureIndex =
			math::ceil<size_t>(destroyStage * (fp)(destroyStageTextures.size() + 1)) -
			1;
		if (textureIndex >= destroyStageTextures.size())
		{
			textureIndex = destroyStageTextures.size() - 1;
		}
		resolutionTexture* currentDestroyStageTexture = destroyStageTextures[textureIndex];
		fillTransparentTexture(blockScreenRect, *currentDestroyStageTexture,
			targetData.renderTarget);
	}
	// draw shadows
	for (cveci2& renderPosition : blocksToBeDrawn)
	{
		cveci2& relativePosition = renderPosition - pos00;
		if (shouldRenderAxis[axisID::y][relativePosition.y])
		{
			if (shouldRenderAxis[axisID::x][relativePosition.x])
			{
				crectangle2& blockScreenRect = targetData.worldToRenderTargetTransform.multRectMatrix(
					crectangle2(cvec2(renderPosition), cvec2(1)));

				LightState& state = lightStates.getValueReferenceUnsafe(relativePosition);
				if (state & LightState::cullBlock)
				{
					fillRectangle(targetData.renderTarget, blockScreenRect, brushes::black);
				}
				else
				{
					if (state != LightState::white &&
						settings::videoSettings::showShadows)
					{
						// we can'T optimize this away, as the shadows are drawn over the entities
						// darken
						const auto& interpolator = interpolators.getValueReferenceUnsafe(
							cveci2(relativePosition));

						// https://stackoverflow.com/questions/20287095/checking-if-all-elements-of-a-vector-are-equal-in-c

						if (std::equal(interpolator.cornerColors + 1, interpolator.cornerColors + 4,
							interpolator.cornerColors))
						{
							// all equal
							const auto& solid = solidColorBrush(interpolator.cornerColors[0]);
							if (solid.value == colorPalette::black) {
								//when the color is just black, we'll convert everything to black, but keep the original alpha
								const auto& multipier = alphaMask(targetData.renderTarget, solid);
								fillRectangle(targetData.renderTarget, ceilRectangle(blockScreenRect),
									multipier);
							}
							else {
								const auto& multipier = colorMultiplier(targetData.renderTarget, solid);

								fillRectangle(targetData.renderTarget, ceilRectangle(blockScreenRect),
									multipier);
							}
						}
						else
						{
							const auto& transform = transformBrush(mat3x3::fromRectToRect(blockScreenRect, crectangle2(cvec2(0), cvec2(1))),
								interpolator);
							const auto& multipier = colorMultiplier(targetData.renderTarget, transform);

							fillRectangle(targetData.renderTarget, ceilRectangle(blockScreenRect), multipier);
						}
					}
				}
			}
		}
	}

	if (settings::renderHitboxes)
	{
		const collisionDataCollection& data = player->dimensionIn->getRecursiveHitboxCollisionData(
			crectangle2(blocksToBeDrawn), cvec2());
		data.render(targetData);

		// render hitboxes
		for (entity* e : nearEntities)
		{
			e->renderHitboxes(targetData);
		}
	}

	if (renderHUD && ((player->entityType != entityID::human) ||
		(player->currentGameMode != gameModeID::spectator)))
	{

		// hotbar

		constexpr int barOffset = 0x2;

		cfp& scaledBarOffset = barOffset * hudScale;
		fp currentYrowOffset = scaledBarOffset;

		fp attackIndicatorOffset = currentYrowOffset;

		crectangle2& hotbarDrawRect = rectangle2(
			(targetData.renderTarget.size.x - (hotbarTextureRect.size.x * hudScale)) / 2,
			currentYrowOffset, hotbarTextureRect.size.x * hudScale,
			hotbarTextureRect.h * hudScale);

		// draw crosshair
		cvec2& offsetLookingAt = player->lookingAt + player->getRenderOffset(targetData);

		// todo: somehow update lookingat each render frame
		// this is hard because some functions update body
		crectangle2& unreachableCrosshairDrawRect = crectangle2(
			targetData.worldToRenderTargetTransform.multPointMatrix(
				offsetLookingAt),
			cvec2())
			.expanded(
				crosshairTextureRect.size.x * 0.5 * settings::videoSettings::guiScale);

		auto mult = colorMultiplier<resolutionTexture, solidColorBrush>(globalLoader[hudTextureFolder / "crosshair.png"],
			brushes::red);

		fillTransparentRectangle(crectangle2(crosshairTextureRect),
			unreachableCrosshairDrawRect, mult, targetData.renderTarget);

		cvec2& offsetExactIntersection = (player->selectedUUID
			? player->exactEntityIntersection
			: player->exactBlockIntersection) +
			player->getRenderOffset(targetData);

		crectangle2& crosshairDrawRect = crectangle2(
			targetData.worldToRenderTargetTransform.multPointMatrix(offsetExactIntersection),
			cvec2())
			.expanded(
				crosshairTextureRect.size.x * 0.5 * settings::videoSettings::guiScale);

		fillTransparentRectangle(crectangle2(crosshairTextureRect), crosshairDrawRect,
			globalLoader[hudTextureFolder / "crosshair.png"], targetData.renderTarget);

		const resolutionTexture& hotbarTexture = globalLoader[hudTextureFolder / "hotbar.png"];
		// draw hotbar
		fillTransparentRectangle(crectangle2(hotbarTextureRect), hotbarDrawRect,
			hotbarTexture, targetData.renderTarget);

		rectanglei2 selectorPixelRect = ceilRectangle(blockScreenRect);
		fillRectangleBorders(targetData.renderTarget, selectorPixelRect, 1,
			solidColorBrush(
				colorPalette::black));

		// draw selector
		crectangle2 selectorDrawRect = crectangle2(
			hotbarDrawRect.pos0 + vec2((player->rightHandSlotIndex * hotbarSpacing + hotbarMargin - selectorMargin) * hudScale, 0),
			selectorTextureRect.size * hudScale);
		fillTransparentRectangle((crectangle2)selectorTextureRect, selectorDrawRect,
			globalLoader[hudTextureFolder / "hotbar_selection.png"], targetData.renderTarget);

		// the space between the hotbar and its elements
		player->hotbarSlots->render(targetData, hotbarDrawRect.pos0 + (hotbarPadding + (hotbarSpacing - hotbarItemDisplaySize) / 2) * hudScale,
			hotbarSpacing * hudScale,
			hotbarItemDisplaySize * hudScale);

		currentYrowOffset += (hotbarDrawRect.y + hotbarDrawRect.h);

		attackIndicatorOffset = currentYrowOffset;

		if (player->experience)
		{
			// draw experience
			constexpr rectangle2 expBarTextureRect = crectangle2(0, 0, 182,
				5);

			cvec2& expBarSize = expBarTextureRect.size * hudScale;
			crectangle2& expBarBackGroundDrawRect = crectangle2(
				(rect.size.x - expBarSize.x) / 2, currentYrowOffset, expBarSize.x,
				expBarSize.y);
			fillTransparentRectangle((crectangle2)expBarTextureRect,
				expBarBackGroundDrawRect, globalLoader[hudTextureFolder / "experience_bar_background.png"],
				targetData.renderTarget);

			cfp& power = getExperienceLevel(player->experience);
			cint& currentLevel = math::floor(power);
			cfp& progressToNextLevel = power - currentLevel;
			crectangle2& expBarforeGroundTextureRect = crectangle2(0, 0, 182 * progressToNextLevel,
				5);
			crectangle2& expBarforeGroundDrawRect = crectangle2(
				expBarBackGroundDrawRect.x, expBarBackGroundDrawRect.y,
				expBarBackGroundDrawRect.w * progressToNextLevel,
				expBarBackGroundDrawRect.h);
			fillTransparentRectangle((crectangle2)expBarforeGroundTextureRect,
				expBarforeGroundDrawRect, globalLoader[hudTextureFolder / "experience_bar_progress.png"],
				targetData.renderTarget);

			if (currentLevel)
			{
				const minecraftFont f = minecraftFont(iconSize * hudScale);
				// std::wstring() to make it concatenate the string and not do other stuff
				std::wstring str = std::wstring() + colorCodeChar + L"a" +
					std::to_wstring(currentLevel);
				cvec2& size = f.fontSize * vec2(1, (fp)str.length());
				f.DrawString(str, crectangle2((rect.w - size.x) * 0.5, currentYrowOffset + expBarSize.y - scaledBarOffset, size.x, size.y),
					targetData.renderTarget);
				attackIndicatorOffset += iconSize * hudScale + scaledBarOffset;
			}
			currentYrowOffset += expBarSize.y + scaledBarOffset;
			attackIndicatorOffset += expBarSize.y + scaledBarOffset;
		}

		cbool& canTakeDamage = (gameModeDataList[player->currentGameMode]->canTakeDamage);

		if (canTakeDamage)
		{
			// minimal half a heart to display
			cfp& healthToRender = math::maximum(player->health, (fp)1.0) * 0.5;
			// constexpr int iconTextureSize = 18;

			// draw hearts
			// draw background
			crectangle2 firstHeartDrawRect = crectangle2(hotbarDrawRect.x, currentYrowOffset,
				iconSize * hudScale,
				iconSize * hudScale);
			fp absorptionYLevel = renderIcons(healthToRender, player->getMaxHealth() / 2,
				globalLoader[heartTextureFolder / "full.png"],
				globalLoader[heartTextureFolder / "half.png"],
				globalLoader[heartTextureFolder / "container.png"],
				firstHeartDrawRect, firstHeartDrawRect.w, targetData.renderTarget);

			// render absorption hearts
			if (player->absorptionHealth > 0)
			{
				crectangle2 firstAbsorptionHeartDrawRect = crectangle2(firstHeartDrawRect.x, absorptionYLevel,
					iconSize * hudScale,
					iconSize * hudScale);

				renderIcons(player->absorptionHealth / 2, player->absorptionHealth / 2,
					globalLoader[heartTextureFolder / "absorbing_full.png"],
					globalLoader[heartTextureFolder / "absorbing_half.png"],
					globalLoader[heartTextureFolder / "container.png"],
					firstHeartDrawRect, firstHeartDrawRect.w, targetData.renderTarget);
			}

			crectangle2 firstDrumstickDrawRect = crectangle2(
				hotbarDrawRect.x + hotbarDrawRect.w - iconSize * hudScale,
				currentYrowOffset, iconSize * hudScale, iconSize * hudScale);
			renderIcons(player->foodlevel * 0.5, maxhumanfoodlevel * 0.5, globalLoader[hudTextureFolder / "food_full.png"],
				globalLoader[hudTextureFolder / "food_half.png"],
				globalLoader[hudTextureFolder / "food_empty.png"], firstDrumstickDrawRect,
				-firstDrumstickDrawRect.w, targetData.renderTarget);
			currentYrowOffset += iconSize * hudScale + scaledBarOffset;
		}
		// draw attack cooldown indicator
		cfp totalCoolDownTime = player->getTotalCoolDownTime();

		if (player->ticksSinceToolUsed < totalCoolDownTime)
		{
			cfp progress = player->ticksSinceToolUsed / totalCoolDownTime; // 0 to 1
			cbool& reloaded = progress > cooldownTreshold;

			constexpr rectangle2 fullCoolDownPixelRect = crectangle2(0, 0, 0x10, 0x10);
			constexpr rectangle2 loadingCoolDownPixelRect = crectangle2(0, 0, 0x10, 0x4);
			crectangle2& coolDownPixelRect = reloaded ? fullCoolDownPixelRect : loadingCoolDownPixelRect;
			cvec2& scaledCoolDownIndicatorSize = cvec2(coolDownPixelRect.size * hudScale);
			crectangle2 coolDownDrawRect = crectangle2(
				(rect.w - scaledCoolDownIndicatorSize.x) * 0.5, attackIndicatorOffset,
				scaledCoolDownIndicatorSize.x, scaledCoolDownIndicatorSize.y);
			if (reloaded)
			{
				fillTransparentRectangle((crectangle2)coolDownPixelRect, coolDownDrawRect,
					globalLoader[hudTextureFolder / "crosshair_attack_indicator_full.png"],
					targetData.renderTarget);
			}
			else
			{
				fillTransparentRectangle((crectangle2)coolDownPixelRect, coolDownDrawRect,
					globalLoader[hudTextureFolder / "crosshair_attack_indicator_background.png"],
					targetData.renderTarget);

				fillTransparentRectangle(
					crectangle2(coolDownPixelRect.x, coolDownPixelRect.y, coolDownPixelRect.w * progress, coolDownPixelRect.h),
					crectangle2(coolDownDrawRect.x, coolDownDrawRect.y, coolDownDrawRect.w * progress, coolDownDrawRect.h),
					globalLoader[hudTextureFolder / "crosshair_attack_indicator_progress.png"],
					targetData.renderTarget);
			}
		}
		if (canTakeDamage)
		{

			// render defence points
			cfp defenceValue = player->getDefencePoints();
			if (defenceValue > 0)
			{
				crectangle2 firstChestplateDrawRect = crectangle2(hotbarDrawRect.x,
					currentYrowOffset,
					iconSize * hudScale,
					iconSize * hudScale);

				renderIcons(defenceValue * 0.5, maxArmor * 0.5, globalLoader[hudTextureFolder / "armor_full.png"],
					globalLoader[hudTextureFolder / "armor_half.png"],
					globalLoader[hudTextureFolder / "armor_empty.png"], firstChestplateDrawRect,
					firstChestplateDrawRect.w, targetData.renderTarget);
			}
		}
	}
	int bossBarIndex = 0;
	cvec2 barTextureSize = cvec2(182, 5);
	constexpr int maxBossBarCount = 7;
	// render boss bars
	for (entity* e : nearEntities)
	{
		if (isBossMob(e->entityType))
		{

			crectangle2 bossBarBackGroundTextureRect = crectangle2(0, 0,
				barTextureSize.x,
				barTextureSize.y);
			crectangle2 bossBarBackGroundScreenRect = crectangle2(
				rect.x + ((rect.w - barTextureSize.x * hudScale) / 2),
				rect.y + rect.h - (barTextureSize.y * (bossBarIndex + 1) * hudScale),
				barTextureSize.x * hudScale, barTextureSize.y * hudScale);
			fillTransparentRectangle(bossBarBackGroundTextureRect, bossBarBackGroundScreenRect,
				globalLoader[spritesTextureFolder / "boss_bar" / "pink_background.png"], targetData.renderTarget);
			if (e->health > 0)
			{
				cfp barPart = e->health / (entityDataList[(int)e->entityType])->maxHealth;
				crectangle2 bossBarForeGroundTextureRect = crectangle2(0, 0,
					barTextureSize.x * barPart,
					barTextureSize.y);
				crectangle2 bossBarForeGroundScreenRect = crectangle2(bossBarBackGroundScreenRect.x,
					bossBarBackGroundScreenRect.y,
					bossBarBackGroundScreenRect.w *
					barPart,
					bossBarBackGroundScreenRect.h);
				fillTransparentRectangle(bossBarForeGroundTextureRect, bossBarForeGroundScreenRect,
					globalLoader[spritesTextureFolder / "boss_bar" / "pink_progress.png"], targetData.renderTarget);
			}

			if (++bossBarIndex >= maxBossBarCount)
			{
				break;
			}
		}
	}

	// finally, render sky on all transparent spots

	for (cveci2& renderPosition : blocksToBeDrawn)
	{
		cveci2& relativePosition = renderPosition - pos00;
		if (shouldRenderAxis[axisID::y][relativePosition.y])
		{
			if (shouldRenderAxis[axisID::x][relativePosition.x])
			{
				block* const& blockToCheck = blockList[(int)surroundingBlocks.getValue(
					relativePosition + averageDistance)];
				const LightState& state = lightStates.getValueUnsafe(relativePosition);
				// only render sky when the block doesn'T completely cover
				if (!blockToCheck->willFillSquare && !(state & LightState::cullBlock) && (dimensionIn->identifier != dimensionID::overworld || renderPosition.y >= skyRenderHeight))
				{
					// render sky
					crectangle2& blockRect = crectangle2(renderPosition, cveci2(1));
					crectangle2& screenRect = targetData.worldToRenderTargetTransform.multRectMatrix(
						blockRect);
					dimensionIn->renderSky(blockRect, screenRect, targetData);
				}
			}
		}
	}
}

void gameControl::layout(crectanglei2& newRect)
{
	form::layout(newRect);
	options->layout(newRect);
	videoOptions->layout(newRect);
	soundOptions->layout(newRect);
	currentCredits->layout(newRect);
	rectanglei2 inventoryRect = crectanglei2(veci2(0x200, 0x200));
	inventoryRect.moveToCenter(rect);
	inventoryUI->layout(inventoryRect);
	// rectanglei2 bigUIRect = crectanglei2(veci2(0x300, 0x200));
	// bigUIRect.moveToCenter(rect);
	structureBlockOptions->layout(newRect);
	jigsawOptions->layout(newRect);
	cint& commandLineHeight = (int)defaultTheme().font->fontSize + defaultTheme().borderSize * 2;
	rectanglei2 commandLineRect;
	if (touchInput)
	{
		commandLineRect = rectanglei2(commandLineHeight, rect.size.y - commandLineHeight,
			rect.size.x - commandLineHeight * 2,
			commandLineHeight);
		cint& joystickSize = rect.size.x / 0x10 * 0x4;
		cint& joystickOffset = joystickSize / 0x8;

		moveJoystick->layout(rectanglei2(veci2(joystickOffset), veci2(joystickSize)));
		interactJoystick->layout(
			rectanglei2(veci2(rect.x + rect.size.x - (joystickOffset + joystickSize),
				joystickOffset),
				veci2(joystickSize)));
		cveci2& pos1 = rect.pos1();
		chatButton->layout(
			crectanglei2(0, pos1.y - commandLineHeight, commandLineHeight, commandLineHeight));
		settingsButton->layout(crectanglei2(pos1.x - commandLineHeight, pos1.y - commandLineHeight,
			commandLineHeight, commandLineHeight));
		// above the joystick with one button as margin
		inventoryButton->layout(crectanglei2(pos1.x - commandLineHeight,
			joystickOffset + joystickSize + commandLineHeight,
			commandLineHeight, commandLineHeight));
	}
	else
	{
		commandLineRect = rectanglei2(0, rect.size.y - commandLineHeight, rect.size.x,
			commandLineHeight);
	}
	commandLineTextbox->layout(commandLineRect);
}

void gameControl::focus()
{
	focused = true;
	if (focusedChild)
	{
		focusedChild->focus();
	}
}

void gameControl::lostFocus()
{
	focused = false;
	touching = false;
	if (focusedChild)
	{
		focusedChild->lostFocus();
	}
}

gameRenderData gameControl::getRenderData(const texture& renderTarget, cfp& secondsOffset) const
{
	return gameRenderData(worldToRenderTargetTransform, renderTarget, *this, secondsOffset);
}

mat3x3 gameControl::getWorldToScreenTransform(cvec2& middleWorldPosition, cfp& pixelsPerBlock) const
{
	return mat3x3::cross(
		mat3x3::scale(vec2(pixelsPerBlock)),
		mat3x3::translate(vec2(-middleWorldPosition + visibleRange)));
}

bool gameControl::getWorldFocus() const
{
	return !inventoryUI->visible &&
		touchInput
		? is_in(
			focusedChild,
			moveJoystick,
			interactJoystick,
			nullptr)
		: !(focusedChild);
}

fp renderIcons(cfp& value, cfp& maxValue, const resolutionTexture& fullIcon, const resolutionTexture& halfIcon, const resolutionTexture& containerIcon,
	crectangle2& firstIconRect, cfp& xOffset, const texture& renderTarget)
{
	vec2 position = firstIconRect.pos0;

	// first render containers
	for (int renderIndex = 0; renderIndex < maxValue; renderIndex++)
	{
		if (renderIndex)
		{
			if (renderIndex % 10 == 0)
			{
				position.x = firstIconRect.x;
				position.y += firstIconRect.h;
			}
			else
			{
				position.x += xOffset;
			}
		}
		fillTransparentRectangle((rectangle2)iconTextureRect,
			rectangle2(position, firstIconRect.size), containerIcon,
			renderTarget);
	}
	// render icons
	position = firstIconRect.pos0;
	for (int renderIndex = 0; renderIndex < value; renderIndex++)
	{
		if (renderIndex)
		{
			if (renderIndex % 10 == 0)
			{
				position.x = firstIconRect.x;
				position.y += firstIconRect.h;
			}
			else
			{
				position.x += xOffset;
			}
		}
		const resolutionTexture& textureToUse = value < renderIndex + 0.5 ? halfIcon : fullIcon;
		fillTransparentRectangle((rectangle2)iconTextureRect,
			rectangle2(position, firstIconRect.size), textureToUse,
			renderTarget);
	}
	return position.y;
}

gameControl::gameControl(playerSocket& socket) : form(),
translator(new eventTranslator(*this)),
socket(socket)

{
	options->visible = false;
	videoOptions->visible = false;
	soundOptions->visible = false;
	inventoryUI->visible = false;
	commandLineTextbox->visible = false;
	structureBlockOptions->visible = false;
	jigsawOptions->visible = false;
	currentCredits->visible = false;

	addChildren({ options,
				 videoOptions,
				 soundOptions,
				 inventoryUI,
				 commandLineTextbox,
				 structureBlockOptions,
				 jigsawOptions,
				 currentCredits });
	addEventHandlers(&gameControl::commandLineKeyPressed, commandLineTextbox->onKeyUp);
}

void gameControl::addTouchInput()
{
	touchInput = true;
	// from -2 to 2
	moveJoystick = new touchJoystick(rectangle2(vec2(-2), vec2(4)));
	constexpr fp armRangePlusMargin = humanArmRange + 1;
	// from -1
	interactJoystick = new touchJoystick(
		rectangle2(vec2(-armRangePlusMargin), vec2(armRangePlusMargin * 2)));

	chatButton = new pictureBox(chatButtonTexture->scaledTextures[0]);
	settingsButton = new pictureBox(settingsButtonTexture->scaledTextures[0]);
	inventoryButton = new pictureBox(inventoryButtonTexture->scaledTextures[0]);
	addEventHandlers(&gameControl::onButtonClick, chatButton->onClick, settingsButton->onClick,
		inventoryButton->onClick);

	// since the other controls aren'T laid out as well, we don'T have to lay them out
	addChildren({ moveJoystick, interactJoystick, chatButton, settingsButton, inventoryButton });
	addEventHandlers(&gameControl::onJoystickTouch, interactJoystick->onMouseDown, onMouseDown);
	addEventHandlers(&gameControl::onJoystickTouchEnd, interactJoystick->onDrop, onDrop);
	addEventHandlers(&gameControl::processScreenTouch, onDrag);
}

bool gameControl::processScreenTouch(const dragEventArgs& args)
{
	for (control* c : children)
	{
		if (c->visible && c->rect.expanded(moveJoystick->rect.x).contains(args.originalPosition))
		{
			return false;
		}
	}

	cvec2& worldPos = worldToRenderTargetTransform.inverse().multPointMatrix(
		cvec2(args.position));
	interactJoystick->value = worldPos - player->getHeadPosition();
	return true;

	// if (inventoryUI->visible && !(inventoryUI->rect.contains(args.position) ||
	//                             (getHighestChild(args.position) == commandLineTextbox))) {
	//     //open inventory
	//     switchInventoryGUI();
	// }
	// if (commandLineTextbox->visible && !(commandLineTextbox->rect.contains(args.position))) {
	//     commandLineTextbox->visible = false;
	// }
}

void gameControl::onJoystickTouch(const mouseButtonEventArgs& args)
{
	if (&args.sender == moveJoystick)
	{
	}
	else
	{ // interactJoystick
		// wants to click or dig
		// calculate relative position
		if (&args.sender == this && !processScreenTouch(
			// original position = position, since we just started dragging
			dragEventArgs(args.sender, args.position, args.position, args.button)))
			return;
		touchStarted = true;
		touching = true;
		// cvec2 relativePosition = player->getHeadPosition() + interactJoystick->value;
	}
}

void gameControl::onJoystickTouchEnd(const dragEventArgs& args)
{
	// touching always has to be set to false
	touching = false;
	touchEnded = true;
	// if (&args.sender == this && !processScreenTouch(args))
	//     return;
}

void gameControl::switchInventoryGUI()
{
	if (inventoryUI->visible)
	{
		player->closeBlockGUI();
	}
	else
	{
		inventoryUI->visible = true;
		if (inventoryUI->visible)
		{
			player->humanSlots->linkUp(inventoryUI);
			focusChild(inventoryUI);
		}
	}
}

void renderOptionsBackGround(crectanglei2& rect, const texture& renderTarget)
{
	if (settings::videoSettings::currentGraphicsMode == graphicsMode::fast)
	{
		fillRectangle(renderTarget, rect, solidColorBrush(colorPalette::black));
	}
	else
	{
		cvec2 halfSize = cvec2(rect.size) * 0.5;
		constexpr fp guiDirtTextureSize = 0x80;
		const auto& currentTransformBrush = transformBrush<resolutionTexture>(
			mat3x3::fromRectToRect(crectangle2(0, 0, guiDirtTextureSize, guiDirtTextureSize),
				blockTextureRect),
			*dirtTexture);
		const auto& currentRepeatingBrush = repeatingBrush<transformBrush<resolutionTexture>>(
			currentTransformBrush, cvec2(guiDirtTextureSize));
		const auto& brush = vignetteBrush<repeatingBrush<transformBrush<resolutionTexture>>>(
			cvec2(rect.pos0) + halfSize, 1, halfSize.length(), currentRepeatingBrush);
		fillRectangle(renderTarget, rect, brush);
	}
}

void gameControl::commandLineKeyPressed(const keyEventArgs& e)
{
	if (e.keyCode == sf::Keyboard::Key::Enter)
	{
		// execute the commandline
		std::wstring line = commandLineTextbox->text;
		currentWorld->currentChat.say(*player, line);

		focusChild(nullptr); // back to the game
		commandLineTextbox->visible = false;
		commandLineTextbox->text = std::wstring();
		commandLineTextbox->cursorIndex = 0;
	}
}

void gameControl::serializeMusicPreference(nbtSerializer& serializer)
{
	if (serializer.push(L"music"))
	{
		const auto& prefer = [&serializer](std::shared_ptr<audioCollection> music)
			{
				serializeNBTValue(serializer, L"prefer", music->key);
			};
		bool boatInWater;
		entity* entityRidingOn = player->dimensionIn->findUUID(player->position,
			ridingEntitySearchRadius,
			player->UUIDRidingOn);
		if (entityRidingOn && entityRidingOn->entityType == entityID::boat &&
			(entityRidingOn->getFluidArea(entityRidingOn->calculateHitBox(), { blockID::water }) >
				0))
		{
			boatInWater = true;
		}
		else
		{
			// did not find the boat
			boatInWater = false;
		}

		cbool inEnd = player->dimensionIn->identifier == dimensionID::end;
		if (inEnd && ((end*)currentWorld->dimensions[(int)dimensionID::end])->dragonAlive)
		{
			serializeNBTValue(serializer, L"replace", bossMusic->key);
			// replaceMusic(bossMusic);
		}
		else
		{
			const biomeID biomeIn = player->dimensionIn->getBiome(player->position);
			cbool inNether = player->dimensionIn->identifier == dimensionID::nether;
			if (inNether)
			{
				if (biomeIn == biomeID::soulsand_valley)
				{
					prefer(soulSandValleyMusic);
				}
				else if (biomeIn == biomeID::nether_wastes)
				{
					prefer(netherWastesMusic);
				}
				else if (biomeIn == biomeID::warped_forest)
				{
					// prefer nothing
				}
				else if (biomeIn != biomeID::warped_forest)
				{
					prefer(netherMusic);
				}
			}
			else
			{
				cbool inWater = (player->getFluidArea(player->calculateHitBox(), { blockID::water }) >
					0);
				cbool inOcean = player->dimensionIn->getBiome(player->position) == biomeID::ocean;
				cbool inCreative = player->currentGameMode == gameModeID::creative;
				prefer(inNether ? netherMusic : inEnd ? endMusic
					: ((boatInWater || inWater) && inOcean) ? waterMusic
					: inCreative ? creativeModeMusic
					: overWorldBackgroundMusic);
			}
		}
	}
}

void gameControl::onButtonClick(const controlEventArgs& args)
{
	if (&args.sender == inventoryButton)
	{
		switchInventoryGUI();
	}
	else if (&args.sender == chatButton)
	{
		if ((commandLineTextbox->visible = !commandLineTextbox->visible))
		{
			focusChild(commandLineTextbox);
		}
	}
	else
	{ // settings button
		options->visible = true;
		focusChild(options);
	}
}
