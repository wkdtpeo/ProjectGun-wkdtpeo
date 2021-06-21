// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Define/PG_GamePlayData.h"
#include <GameFramework/PlayerState.h>
#include "PG_MyPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnUpdateRewardPoint);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddRewardPoint, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMonsterKill, class APG_MonChar*);
DECLARE_MULTICAST_DELEGATE(FOnReadyToUseTheLethalAttack);
DECLARE_MULTICAST_DELEGATE(FOnTryUseTheLethalAttack);
DECLARE_MULTICAST_DELEGATE(FOnUseTheLethalAttackFail);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangeWeapon, int32);

/**
 * PlayerState : �÷��̾��� ���¸� �����Ѵ�. �÷��̾��� ĳ���Ͱ� ���� ��������, �̸��� �������� ���� �����Ѵ�.
 */
UCLASS()
class PROJECT_GUN_API APG_MyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public :
	APG_MyPlayerState();
	
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	bool InitPlayerData();
	//bool SavePlayerData();

	void AddRewardPoint(int32 a_nAddPoint);
	void SetRewardPoint(int32 a_nSetPoint);

	void MonsterKill(class APG_MonChar* KillMon);
	void MonsterLethalAttackKill(class APG_MonChar* KillMon);

	bool UseAmmo();
	void ChangeDefaultWeapon();	// �⺻ ����� ����
	bool ChangeWeapon(int32 nSlotIndex, int32 nWeaponTableIndex);
	
	const FPGPlayerData* GetPlayingPlayerData()		{ return &PlayingPlayerData;	}
	const FPGPlayerData* GetOriginalPlayerData()	{ return &OrignalPlayerData;	}

	UFUNCTION(BlueprintCallable, Category = "PG RewardPoint")
	int32 GetRewardPoint()	{ return PlayingPlayerData.RewardPoint; }

	//==========================================================
	// Lethal Attack
	//==========================================================
	void ReadyToUseTheSkill();		// �ʻ�� ��� �غ�
	void TryUseTheSkill();			// �ʻ�� ��� �õ�

public :
	//==========================================================
	// Point
	//==========================================================
	FOnUpdateRewardPoint	OnUpdateRewardPoint;		// ����Ʈ ȹ�� (���� ų + �������� Ŭ���� ȹ�� ��ȭ ����)
	FOnAddRewardPoint		OnKillAddRewardPoint;		// ���� óġ�� ���� ����Ʈ ȹ�� (�������� Ŭ���� ȹ�� ��ȭ ������)

	//==========================================================
	// Monster
	//==========================================================
	FOnMonsterKill	OnBasicAttackMonsterKill;		// �Ϲݰ������� ���� ���� óġ
	//FOnMonsterKill	OnLethalAttackMonsterKill;	// �ʻ�� �������� ���� ���� óġ

	//==========================================================
	// LethalAttack
	//==========================================================
	FOnReadyToUseTheLethalAttack		OnReadyToUseTheLethalAttack;	// ��ų ��� �غ�
	FOnTryUseTheLethalAttack			OnTryUseTheLethalAttack;		// ��ų ��� �õ�
	FOnUseTheLethalAttackFail			OnUseTheLethalAttackFail;		// ��ų ��� ����

	// ���� ��ü �˸�
	FOnChangeWeapon OnChangeWeapon;

protected :
	// �������� �÷����� ����Ǵ� �÷��̾� ������
	UPROPERTY(Transient, VisibleAnywhere)
	struct FPGPlayerData PlayingPlayerData;

	// �������� ���۽� ������ �ʴ� ���� �÷��̾� ������
	UPROPERTY(Transient, VisibleAnywhere)
	struct FPGPlayerData OrignalPlayerData;
};
