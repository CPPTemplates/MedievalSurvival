#include "workAI.h"
#include "Villager.h"
#include "tickableBlockContainer.h"
#include <LinkableBlockData.h>
#include <Linkable.h>

bool WorkAI::shouldExecute()
{
	Villager* connectedVillager = (Villager*)connectedEntity;
	if (isEmployed(connectedVillager->profession)) {
		return true;
	}
	//look for job site blocks
	connectedVillager->updateSelection();
	blockID selectedBlock = connectedVillager->selectedBlockContainer->getBlockID(connectedVillager->selectedBlockPosition);
	ProfessionID newProfession = getProfession(selectedBlock);
	if (newProfession) {
		connectedVillager->profession = newProfession;
		LinkableBlockData* workStationData = dynamic_cast<LinkableBlockData*>(connectedVillager->selectedBlockContainer->getBlockData(connectedVillager->selectedBlockPosition));
		connectedVillager->workStation = workStationData->identifier;
	}
	return false;
}

void WorkAI::updateTask()
{
	Villager* connectedVillager = (Villager*)connectedEntity;
	LinkableBlockData* workStationData = ((LinkableBlockData*)getLinkable(connectedVillager->workStation));
	if (workStationData)
	{
		connectedVillager->goToPosition(workStationData->containerIn->containerToRootTransform.multPointMatrix(workStationData->position));
	}
	else {
		//lost workstation
		connectedVillager->profession = ProfessionID::Unemployed;
	}
}
