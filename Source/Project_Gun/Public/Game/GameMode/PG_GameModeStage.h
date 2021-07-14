// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Game/GameMode/PG_GameModeBase.h"

#include <GameFramework/GameModeBase.h>
#include "PG_GameModeStage.generated.h"

/**
 * GameMode : 레벨당 한 개의 게임 모드를 가지며 게임의 룰을 정의한다.
 */
UCLASS()
class PROJECT_GUN_API APG_GameModeStage : public APG_GameModeBase
{
	GENERATED_BODY()
	
public:
	APG_GameModeStage();

	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;

public :
	//===================================================
	// Mission
	//===================================================
	virtual bool IsClearMission(EStageClearType a_ekStageClearType) override;		// 현재 각 타입의 미션 클리어 여부
	bool IsWasClearMisiion(EStageClearType a_ekStageClearType);						// 이전에 각 타입의 미션 클리어 여부
	bool IsStageClear();															// 스테이지 클리어 여부

	//===================================================
	// Save Data
	//===================================================
	virtual bool SavePlayerData() override;
	virtual bool SaveStageData() override;

public:
	//============================================
	// 플레이어 데이터 (Player)
	//============================================
	bool SetSavePlayerData(const struct FPGPlayerData& a_rkPlayerData);

	//============================================
	// 스테이지 데이터 (Stage)
	//============================================
	bool SetSaveStageData(const struct FPGStagePlayData& a_rkStagePlayData);

};
