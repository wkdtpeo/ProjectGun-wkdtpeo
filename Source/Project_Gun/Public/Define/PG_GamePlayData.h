#pragma once

#include "Project_Gun/Project_Gun.h"
#include "PG_GamePlayData.generated.h"

//=========================================================================================
// 플레이어 데이터
//=========================================================================================
USTRUCT(BlueprintType)
struct FPGPlayerData
{
	GENERATED_BODY()

public:
	FPGPlayerData() : EquipWeaponInventoryIndex(0), bExtendWeaponSlot(false), RewardPoint(0)
	{ 
		PlayerWeaponInventory.Init(0, PG_MAX_WEAPON_SLOT);
		PlayerWeaponAmmo.Init(0, PG_MAX_WEAPON_SLOT);
	}

	// 장착중인 무기의 Inventory의 Index (SlotIndex 저장)
	UPROPERTY()
	int32 EquipWeaponInventoryIndex;

	// 인벤토리 (현재 3개까지 확장 가능)
	UPROPERTY()
	bool bExtendWeaponSlot;

	// 인벤토리 (현재 3개까지 확장 가능 - WeaponTableIndex 저장)
	UPROPERTY()
	TArray<int32> PlayerWeaponInventory;

	// 탄약 재고량 (SlotIndex 저장)
	UPROPERTY()
	TArray<int32> PlayerWeaponAmmo;

	// 보유중인 재화 포인트
	UPROPERTY()
	int32 RewardPoint;
};

//=========================================================================================
// 보유중인 무기 데이터
//=========================================================================================
USTRUCT(BlueprintType)
struct FPGWeaponOwnData
{
	GENERATED_BODY()

public:
	FPGWeaponOwnData() : IsUnlock(false), PowerLevel(1), MagLevel(1){ }

	// 무기 구매 여부
	UPROPERTY()
	bool IsUnlock;

	// 무기 공격력 레벨
	UPROPERTY()
	int32 PowerLevel;

	// 탄창 증가 레벨
	UPROPERTY()
	int32 MagLevel;
};

//=========================================================================================
// 스테이지 플레이 데이터
//=========================================================================================
USTRUCT(BlueprintType)
struct FPGStagePlayData
{
	GENERATED_BODY()

public:
	FPGStagePlayData() : StageID(0), NumberOfMonsters(0), NumberOfMonstersKilled(0), 
		NumberOfMonstersMuiltiKill(0), NumberOfMonstersMuiltiKilled(0), PlayTime(0), RemainTime(0), KillRewardPoint(0){ }

	// 현재 플레이중인 스테이지 ID
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 StageID;

	// 설치된 몬스터의 수 (클리어 목표)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 NumberOfMonsters;

	// 죽인 몬스터의 수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 NumberOfMonstersKilled;

	// 다중 처치해야 할 몬스터의 수 (멀티킬 클리어 목표)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 NumberOfMonstersMuiltiKill;

	// 다중 처치한 몬스터의 수 (멀티킬)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 NumberOfMonstersMuiltiKilled;

	// 플레이 시간
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 PlayTime;

	// 남은 시간
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 RemainTime;

	// 몬스터 처치로부터 얻은 재화 (스테이지 클리어로 얻은 재화는 포함되지 않으니 주의할 것)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 KillRewardPoint;
};

enum EStageClearType
{
	eSCT_AmountKill = 0,		// 목표한 몬스터 처치 후 목표지점 도달
	eSCT_ClearTime,				// 제한시간 내 모든 몬스터 처치 후 목표지점 도달
	eSCT_Lethal,				// 필살기 사용으로 목표 멀티킬 이상을 달성 후 목표지점 도달

	eSCT_Max
};

