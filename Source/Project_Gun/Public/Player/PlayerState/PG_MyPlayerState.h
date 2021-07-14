// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"

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
 * PlayerState : 플레이어의 상태를 정의한다. 플레이어의 캐릭터가 무슨 직업인지, 이름은 무엇인지 등을 포함한다.
 * -. 남들이 알아야하는 내 정보 (멀티플레이 환경에 의미가 더 도드라진다)
 */
UCLASS()
class PROJECT_GUN_API APG_MyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public :
	APG_MyPlayerState();
	
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public :
	bool InitPlayerData();		// 스테이지 시작전 플레이어 데이터를 초기화한다.

	//==========================================================
	// Reward Point
	//==========================================================
	void AddRewardPoint(int32 a_nAddPoint);
	void SetRewardPoint(int32 a_nSetPoint);
	UFUNCTION(BlueprintCallable, Category = "PG RewardPoint")
	int32 GetRewardPoint() { return m_kPlayingPlayerData.RewardPoint; }

	//==========================================================
	// PlayerData
	//==========================================================
	const FPGPlayerData* GetOriginalPlayerData() { return &m_kOrignalPlayerData; }		// 스테이지 플레이하기전 오리지널 플레이어 데이터
	const FPGPlayerData* GetPlayingPlayerData() { return &m_kPlayingPlayerData; }		// 스테이지 플레이 시작 후 변경되는 플레이어 데이터

	//==========================================================
	// Monster
	//==========================================================
	void MonsterKill(class APG_MonChar* KillMon);
	void MonsterLethalAttackKill(class APG_MonChar* KillMon);

	//==========================================================
	// Weapon
	//==========================================================
	bool UseAmmo();					// 현재 장찬된 무기의 탄약 소비
	void ChangeDefaultWeapon();		// 기본 무기로 변경
	bool ChangeWeapon(int32 nSlotIndex, int32 nWeaponTableIndex);	// 무기 변경

	//==========================================================
	// Lethal Attack
	//==========================================================
	void ReadyToUseTheSkill();		// 필살기 사용 준비
	void TryUseTheSkill();			// 필살기 사용 시도

public :
	//==========================================================
	// Point
	//==========================================================
	FOnUpdateRewardPoint	OnUpdateRewardPoint;			// 포인트 획득 (몬스터 킬 + 스테이지 클리어 획득 재화 포함)
	FOnAddRewardPoint		OnKillAddRewardPoint;			// 몬스터 처치로 인한 포인트 획득 (스테이지 클리어 획득 재화 미포함)

	//==========================================================
	// Monster
	//==========================================================
	FOnMonsterKill			OnBasicAttackMonsterKill;		// 일반공격으로 인한 몬스터 처치

	//==========================================================
	// LethalAttack
	//==========================================================
	FOnReadyToUseTheLethalAttack	OnReadyToUseTheLethalAttack;	// 스킬 사용 준비
	FOnTryUseTheLethalAttack		OnTryUseTheLethalAttack;		// 스킬 사용 시도
	FOnUseTheLethalAttackFail		OnUseTheLethalAttackFail;		// 스킬 사용 실패

	//==========================================================
	// Weapon
	//==========================================================
	FOnChangeWeapon OnChangeWeapon;		// 무기 교체 알림

protected :
	// 스테이지 플레이중 변경되는 플레이어 데이터
	UPROPERTY(Transient, VisibleAnywhere)
	struct FPGPlayerData m_kPlayingPlayerData;

	// 스테이지 시작시 변하지 않는 원본 플레이어 데이터
	UPROPERTY(Transient, VisibleAnywhere)
	struct FPGPlayerData m_kOrignalPlayerData;
};
