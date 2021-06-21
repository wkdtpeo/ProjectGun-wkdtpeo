// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <GameFramework/SaveGame.h>
#include "Define/PG_GamePlayData.h"
#include "PG_SaveStageData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_SaveStageData : public USaveGame
{
	GENERATED_BODY()
	
public :
	UPG_SaveStageData();

	UPROPERTY(VisibleAnyWhere, Category = "PG StageSaveData")
	int32 m_nStageID = 0;

	// 목표로 설정된 몬스터 제거
	UPROPERTY(VisibleAnyWhere, Category = "PG StageSaveData")
	struct FPGStagePlayData m_kStagePlayData;

	UPROPERTY(VisibleAnyWhere, Category = "PG StageSaveData")
	bool m_bWasClearAmount = false;

	UPROPERTY(VisibleAnyWhere, Category = "PG StageSaveData")
	bool m_bWasClearTime = false;

	UPROPERTY(VisibleAnyWhere, Category = "PG StageSaveData")
	bool m_bWasClearLethal = false;

public :
	UFUNCTION()
	static FString GetSlotName(int32 a_nStageID);
	UFUNCTION()
	static int32 GetPlayerIndex();
};
