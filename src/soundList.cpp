#include <memory>
#include "audioCollection.h"
#include "armorTier.h"
#include "soundList.h"
std::shared_ptr<audioCollection> popSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> experienceSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> levelUpSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> chestOpenSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> chestCloseSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> enderChestOpenSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> enderChestCloseSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> barrelOpenSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> barrelCloseSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> tillSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> stripLogSound = std::shared_ptr<audioCollection>();

std::shared_ptr<audioCollection> eatSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> burpSound = std::shared_ptr<audioCollection>();

std::shared_ptr<audioCollection> drinkingSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> honeyDrinkingSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> bottleFillSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> bottleEmptySound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> bottleFillDragonBreathSound = std::shared_ptr<audioCollection>();

std::shared_ptr<audioCollection> waterBucketFillSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> lavaBucketFillSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> waterBucketEmptySound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> lavaBucketEmptySound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> milkSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> waterPaddleSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> weakAttackSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> strongAttackSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> criticalAttackSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> enchantSound = std::shared_ptr<audioCollection>();

std::shared_ptr<audioCollection> woodenDoorOpenSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> woodenDoorCloseSound = std::shared_ptr<audioCollection>();

std::shared_ptr<audioCollection> ironDoorOpenSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> ironDoorCloseSound = std::shared_ptr<audioCollection>();

std::shared_ptr<audioCollection> woodenTrapDoorOpenSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> woodenTrapDoorCloseSound = std::shared_ptr<audioCollection>();

std::shared_ptr<audioCollection> ironTrapDoorOpenSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> ironTrapDoorCloseSound = std::shared_ptr<audioCollection>();

std::shared_ptr<audioCollection> fenceGateOpenSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> fenceGateCloseSound = std::shared_ptr<audioCollection>();

std::shared_ptr<audioCollection> flintAndSteelSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> extinguishSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> sheepShearingSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> burningDamageSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> portalDepartSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> portalArriveSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> teleportFromSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> teleportToSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> shootSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> eyeOfEnderLaunchSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> eyeOfEnderDropSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> eyeOfEnderPlaceSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> endPortalOpenSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> fuseSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> explosionSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> enderDragonWingsSound = std::shared_ptr<audioCollection>();

std::shared_ptr<audioCollection> ghastWarningSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> fireBallShootSound = std::shared_ptr<audioCollection>();

std::shared_ptr<audioCollection> endermanStareSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> endermanScreamSound = std::shared_ptr<audioCollection>();

std::shared_ptr<audioCollection> fireSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> enderDragonDeathSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> bowHitSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> brewingFinishSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> clickSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> pistonExtendSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> pistonRetractSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> noteSounds[(size_t)noteTypeID::count]{};
std::shared_ptr<audioCollection> pumpkinCarveSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> toolBreakSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> smithingTableUseSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> anvilUseSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> anvilLandSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> anvilBreakSound = std::shared_ptr<audioCollection>();

std::shared_ptr<audioCollection> smallSlimeSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> bigSlimeSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> slimeAttackSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> windSound = std::shared_ptr<audioCollection>();

std::shared_ptr<audioCollection> equipSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> armorEquipSound[armorTierCount];
std::shared_ptr<audioCollection> villagerNoSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> villagerYesSound = std::shared_ptr<audioCollection>();
std::shared_ptr<audioCollection> villagerHaggleSound = std::shared_ptr<audioCollection>();