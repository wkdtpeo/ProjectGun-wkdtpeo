// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Game/GameMode/PG_GameModeBase.h"

#include <GameFramework/GameModeBase.h>
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
	// MainLobby에서 사용하는 미션 클리어 검사
	virtual bool IsClearMission(enum EStageClearType a_ekStageClearType) override;

	// 무기를 장착한다.
	void SetEquipWeapon(int32 nSlotIndex, int32 nWeaponIndex);
};
