// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Define/PG_GamePlayData.h"
#include "Define/PG_TableData.h"
#include <GameFramework/SaveGame.h>
#include "PG_SavePlayerData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_SavePlayerData : public USaveGame
{
	GENERATED_BODY()
	
public :
	UPG_SavePlayerData();

	// 마지막으로 플레이한 스테이지 ID
	UPROPERTY(VisibleAnywhere, Category = "PG StageData")
	int32 m_nLastPlayStageID;

	// 보유중인 포인트
	UPROPERTY(VisibleAnywhere, Category = "PG StageData")
	int32 m_nRewardPoint;

	// 3번째 무기 슬롯 확장 여부 (유료로 확장 가능)
	UPROPERTY(VisibleAnywhere, Category = "PG PlayerData")
	bool m_bExtendWeaponSlot;

	// 현재 장착중인 무기 정보 (최대 3개), TArray<PG_MAX_WEAPON_SLOT>
	UPROPERTY(VisibleAnywhere, Category = "PG PlayerData")
	TArray<int32> m_kEquipWeaponTableIndex;

	// 구매하여 소유중인 무기 정보, TMap<WeaponTableIndex, FPGWeaponOwnData>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG WeaponInfo")
	TMap<int32, struct FPGWeaponOwnData> WeaponOwnData;
	
public :
	static FString GetSlotName();
	static int32 GetPlayerIndex();
};
