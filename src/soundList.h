#pragma once
#include "audioCollection.h"
#include "armorTier.h"
#include "noteTypeID.h"

extern std::shared_ptr<audioCollection> popSound;
extern std::shared_ptr<audioCollection> experienceSound;
extern std::shared_ptr<audioCollection> levelUpSound;
extern std::shared_ptr<audioCollection> chestOpenSound;
extern std::shared_ptr<audioCollection> chestCloseSound;
extern std::shared_ptr<audioCollection> enderChestOpenSound;
extern std::shared_ptr<audioCollection> enderChestCloseSound;
extern std::shared_ptr<audioCollection> barrelOpenSound;
extern std::shared_ptr<audioCollection> barrelCloseSound;
extern std::shared_ptr<audioCollection> tillSound;
extern std::shared_ptr<audioCollection> stripLogSound;

extern std::shared_ptr<audioCollection> eatSound;
extern std::shared_ptr<audioCollection> burpSound;

extern std::shared_ptr<audioCollection> drinkingSound;
extern std::shared_ptr<audioCollection> honeyDrinkingSound;
extern std::shared_ptr<audioCollection> bottleFillSound;
extern std::shared_ptr<audioCollection> bottleEmptySound;
extern std::shared_ptr<audioCollection> bottleFillDragonBreathSound;

extern std::shared_ptr<audioCollection> waterBucketFillSound;
extern std::shared_ptr<audioCollection> lavaBucketFillSound;
extern std::shared_ptr<audioCollection> waterBucketEmptySound;
extern std::shared_ptr<audioCollection> lavaBucketEmptySound;
extern std::shared_ptr<audioCollection> milkSound;
extern std::shared_ptr<audioCollection> waterPaddleSound;
extern std::shared_ptr<audioCollection> weakAttackSound;
extern std::shared_ptr<audioCollection> strongAttackSound;
extern std::shared_ptr<audioCollection> criticalAttackSound;
extern std::shared_ptr<audioCollection> enchantSound;

extern std::shared_ptr<audioCollection> woodenDoorOpenSound;
extern std::shared_ptr<audioCollection> woodenDoorCloseSound;

extern std::shared_ptr<audioCollection> ironDoorOpenSound;
extern std::shared_ptr<audioCollection> ironDoorCloseSound;

extern std::shared_ptr<audioCollection> woodenTrapDoorOpenSound;
extern std::shared_ptr<audioCollection> woodenTrapDoorCloseSound;

extern std::shared_ptr<audioCollection> ironTrapDoorOpenSound;
extern std::shared_ptr<audioCollection> ironTrapDoorCloseSound;

extern std::shared_ptr<audioCollection> fenceGateOpenSound;
extern std::shared_ptr<audioCollection> fenceGateCloseSound;

extern std::shared_ptr<audioCollection> flintAndSteelSound;
extern std::shared_ptr<audioCollection> extinguishSound;
extern std::shared_ptr<audioCollection> sheepShearingSound;
extern std::shared_ptr<audioCollection> burningDamageSound;
extern std::shared_ptr<audioCollection> portalDepartSound;
extern std::shared_ptr<audioCollection> portalArriveSound;
extern std::shared_ptr<audioCollection> teleportFromSound;
extern std::shared_ptr<audioCollection> teleportToSound;
extern std::shared_ptr<audioCollection> shootSound;
extern std::shared_ptr<audioCollection> eyeOfEnderLaunchSound;
extern std::shared_ptr<audioCollection> eyeOfEnderDropSound;
extern std::shared_ptr<audioCollection> eyeOfEnderPlaceSound;
extern std::shared_ptr<audioCollection> endPortalOpenSound;
extern std::shared_ptr<audioCollection> fuseSound;
extern std::shared_ptr<audioCollection> explosionSound;
extern std::shared_ptr<audioCollection> enderDragonWingsSound;
extern std::shared_ptr<audioCollection> fireBallShootSound;
extern std::shared_ptr<audioCollection> endermanStareSound;
extern std::shared_ptr<audioCollection> endermanScreamSound;
extern std::shared_ptr<audioCollection> fireSound;
extern std::shared_ptr<audioCollection> ghastWarningSound;
extern std::shared_ptr<audioCollection> enderDragonDeathSound;
extern std::shared_ptr<audioCollection> bowHitSound;
extern std::shared_ptr<audioCollection> brewingFinishSound;
extern std::shared_ptr<audioCollection> clickSound;
extern std::shared_ptr<audioCollection> pistonExtendSound;
extern std::shared_ptr<audioCollection> pistonRetractSound;
extern std::shared_ptr<audioCollection> noteSounds[(size_t)noteTypeID::count];
extern std::shared_ptr<audioCollection> pumpkinCarveSound;
extern std::shared_ptr<audioCollection> toolBreakSound;
extern std::shared_ptr<audioCollection> smithingTableUseSound;
extern std::shared_ptr<audioCollection> anvilUseSound;
extern std::shared_ptr<audioCollection> anvilLandSound;
extern std::shared_ptr<audioCollection> anvilBreakSound;

extern std::shared_ptr<audioCollection> smallSlimeSound;
extern std::shared_ptr<audioCollection> bigSlimeSound;
extern std::shared_ptr<audioCollection> slimeAttackSound;
extern std::shared_ptr<audioCollection> windSound;

extern std::shared_ptr<audioCollection> equipSound;
extern std::shared_ptr<audioCollection> armorEquipSound[armorTierCount];

extern std::shared_ptr<audioCollection> villagerNoSound;
extern std::shared_ptr<audioCollection> villagerYesSound;
extern std::shared_ptr<audioCollection> villagerHaggleSound;