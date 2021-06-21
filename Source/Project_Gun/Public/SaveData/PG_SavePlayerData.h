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

	// ���������� �÷����� �������� ID
	UPROPERTY(VisibleAnywhere, Category = "PG StageData")
	int32 m_nLastPlayStageID;

	// �������� ����Ʈ
	UPROPERTY(VisibleAnywhere, Category = "PG StageData")
	int32 m_nRewardPoint;

	// 3��° ���� ���� Ȯ�� ���� (����� Ȯ�� ����)
	UPROPERTY(VisibleAnywhere, Category = "PG PlayerData")
	bool m_bExtendWeaponSlot;

	// ���� �������� ���� ���� (�ִ� 3��), TArray<PG_MAX_WEAPON_SLOT>
	UPROPERTY(VisibleAnywhere, Category = "PG PlayerData")
	TArray<int32> m_kEquipWeaponTableIndex;

	// �����Ͽ� �������� ���� ����, TMap<WeaponTableIndex, FPGWeaponOwnData>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG WeaponInfo")
	TMap<int32, struct FPGWeaponOwnData> WeaponOwnData;
	
public :
	static FString GetSlotName();
	static int32 GetPlayerIndex();
};
