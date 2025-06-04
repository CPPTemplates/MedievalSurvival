#include "audioCollection.h"
#include <memory>
#include <itemID.h>
std::shared_ptr<audioCollection> overWorldBackgroundMusic = nullptr;
std::shared_ptr<audioCollection> netherMusic = nullptr;
std::shared_ptr<audioCollection> endMusic = nullptr;
std::shared_ptr<audioCollection> bossMusic = nullptr;
std::shared_ptr<audioCollection> creditsMusic = nullptr;
std::shared_ptr<audioCollection> mainMenuBackgroundMusic = nullptr;
std::shared_ptr<audioCollection> waterMusic = nullptr;
std::shared_ptr<audioCollection> creativeModeMusic = nullptr;
std::shared_ptr<audioCollection> crimsonForestMusic = nullptr;
std::shared_ptr<audioCollection> netherWastesMusic = nullptr;
std::shared_ptr<audioCollection> soulSandValleyMusic = nullptr;
std::shared_ptr<audioCollection> recordsMusic[musicDiscTypeCount]{};