// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <GameFramework/GameModeBase.h>
#include "Game/GameMode/PG_GameModeBase.h"
#include "PG_GameModeMainLobby.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API APG_GameModeMainLobby : public APG_GameModeBase
{
	GENERATED_BODY()
	
public :
	APG_GameModeMainLobby();

	virtual void BeginPlay() override;
	
public :
	// MainLobby���� ����ϴ� �̼� Ŭ���� �˻�
	virtual bool IsClearMission(enum EStageClearType a_ekStageClearType) override;

	// ���⸦ �����Ѵ�.
	void SetEquipWeapon(int32 nSlotIndex, int32 nWeaponIndex);
};
