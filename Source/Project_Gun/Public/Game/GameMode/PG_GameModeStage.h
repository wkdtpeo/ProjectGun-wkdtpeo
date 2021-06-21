// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <GameFramework/GameModeBase.h>
#include "Game/GameMode/PG_GameModeBase.h"
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

	// 현재 각 타입의 미션 클리어 여부
	virtual bool IsClearMission(EStageClearType a_ekStageClearType) override;	
	// 이전에 각 타입의 미션 클리어 여부
	bool IsWasClearMisiion(EStageClearType a_ekStageClearType);
	// 스테이지 클리어 여부
	bool IsStageClear();

	// 플레이중인 스테이지 ID
	int32 GetPlayStageID();
	// 획득한 포인트 (총합 : 스테이지 클리어 + 미션 클리어 + 몬스터 처치 + 광고)
	int32 GetClearTotalPoint();
	// 스테이지 클리어 포인트
	int32 GetClearStagePoint();
	// 미션 클리어 포인트
	int32 GetClearMissionPoint();
	// 몬스터 처치 포인트
	int32 GetClearMonsterKillPoint();

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
