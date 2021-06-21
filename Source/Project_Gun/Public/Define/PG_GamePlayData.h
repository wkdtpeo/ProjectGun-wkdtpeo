#pragma once

#include "Project_Gun/Project_Gun.h"
#include "PG_GamePlayData.generated.h"

//=========================================================================================
// �÷��̾� ������
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

	// �������� ������ Inventory�� Index (SlotIndex ����)
	UPROPERTY()
	int32 EquipWeaponInventoryIndex;

	// �κ��丮 (���� 3������ Ȯ�� ����)
	UPROPERTY()
	bool bExtendWeaponSlot;

	// �κ��丮 (���� 3������ Ȯ�� ���� - WeaponTableIndex ����)
	UPROPERTY()
	TArray<int32> PlayerWeaponInventory;

	// ź�� ��� (SlotIndex ����)
	UPROPERTY()
	TArray<int32> PlayerWeaponAmmo;

	// �������� ��ȭ ����Ʈ
	UPROPERTY()
	int32 RewardPoint;
};

//=========================================================================================
// �������� ���� ������
//=========================================================================================
USTRUCT(BlueprintType)
struct FPGWeaponOwnData
{
	GENERATED_BODY()

public:
	FPGWeaponOwnData() : IsUnlock(false), PowerLevel(1), MagLevel(1){ }

	// ���� ���� ����
	UPROPERTY()
	bool IsUnlock;

	// ���� ���ݷ� ����
	UPROPERTY()
	int32 PowerLevel;

	// źâ ���� ����
	UPROPERTY()
	int32 MagLevel;
};

//=========================================================================================
// �������� �÷��� ������
//=========================================================================================
USTRUCT(BlueprintType)
struct FPGStagePlayData
{
	GENERATED_BODY()

public:
	FPGStagePlayData() : StageID(0), NumberOfMonsters(0), NumberOfMonstersKilled(0), 
		NumberOfMonstersMuiltiKill(0), NumberOfMonstersMuiltiKilled(0), PlayTime(0), RemainTime(0), KillRewardPoint(0){ }

	// ���� �÷������� �������� ID
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 StageID;

	// ��ġ�� ������ �� (Ŭ���� ��ǥ)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 NumberOfMonsters;

	// ���� ������ ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 NumberOfMonstersKilled;

	// ���� óġ�ؾ� �� ������ �� (��Ƽų Ŭ���� ��ǥ)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 NumberOfMonstersMuiltiKill;

	// ���� óġ�� ������ �� (��Ƽų)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 NumberOfMonstersMuiltiKilled;

	// �÷��� �ð�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 PlayTime;

	// ���� �ð�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 RemainTime;

	// ���� óġ�κ��� ���� ��ȭ (�������� Ŭ����� ���� ��ȭ�� ���Ե��� ������ ������ ��)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 KillRewardPoint;
};

enum EStageClearType
{
	eSCT_AmountKill = 0,		// ��ǥ�� ���� óġ �� ��ǥ���� ����
	eSCT_ClearTime,				// ���ѽð� �� ��� ���� óġ �� ��ǥ���� ����
	eSCT_Lethal,				// �ʻ�� ������� ��ǥ ��Ƽų �̻��� �޼� �� ��ǥ���� ����

	eSCT_Max
};

