// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"

#include <GameFramework/GameStateBase.h>
#include "PG_GameStateStage.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStageTimer);
DECLARE_MULTICAST_DELEGATE(FOnStageClear);
DECLARE_MULTICAST_DELEGATE(FOnStageFailed);
DECLARE_MULTICAST_DELEGATE(FOnUdpateWidgetPlayStage);
DECLARE_MULTICAST_DELEGATE(FOnGateOpen);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnViewAD, bool);

/**
 * GameState : 게임의 상태를 정의한다. 게임이 시작되었는지, 중단되었는지, A팀의 스토어가 몇점인지 등을 포함한다.
 */
UCLASS()
class PROJECT_GUN_API APG_GameStateStage : public AGameStateBase
{
	GENERATED_BODY()

public:
	APG_GameStateStage();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:
	bool	InitStageData();

	void	MonsterDie(class APG_MonChar* KillMon, bool IsLethalAttack);

	//===================================================
	// Stage Info.
	//===================================================
	const struct FPGStagePlayData*	GetStagePlayData()	{ return &StagePlayData; }
	int32	GetLevelInstallMonsterCount();	// 현재 레벨에 설치된 몬스터의 수
	int32	GetRemainStageTimer();			// 현재 남은 시간
	void	SetMaxLethalKill(int32 a_nMaxLethalKill);	// 필살기로 죽인 몬스터 수 갱신
	int32	GetMaxLethalKill();							// 필살기로 죽은 최대 몬스터의 수
	// Stage Clear Info.
	int32	GetClearTotalPoint();			// 획득한 포인트 (총합 : 스테이지 클리어 + 미션 클리어 + 몬스터 처치 + 광고)
	int32	GetClearStagePoint();			// 스테이지 클리어 포인트
	int32	GetClearMissionPoint();			// 미션 클리어 포인트
	int32	GetClearMonsterKillPoint();		// 몬스터 처치 포인트


	//===================================================
	// AD (광고)
	//===================================================
	bool	IsViewAD();					// 광고를 보았는지 확인
	void	SetViewAD(bool bEnable);
	bool	RequestViewAD();			// 광고 요청

public:
	//==========================================================
	// Stage
	//==========================================================
	FOnStageTimer	StageTimerDelegate;		// 스테이지 타이머
	FOnStageClear	StageClearDelegate;		// 스테이지 클리어
	FOnStageFailed	StageFailedDelegate;	// 스테이지 실패
	FOnGateOpen		GateOpenDelegate;		// 스테이지 클리어 게이트 오픈
	FOnUdpateWidgetPlayStage UdpateWidgetPlayStage;		// 스테이지 클리어 목표 현황 업데이트 요청

	//==========================================================
	// AD
	//==========================================================
	FOnViewAD		OnViewAD;				// 광고를 보았음

public:
	void	OnCheckStageClear();
	void	OnCheckStageFailed();

	UFUNCTION()
	void	OnKillAddRewardPoint(int32 a_nAddRewardPoint);	// 몬스터 처치로 인한 포인트 획득

protected :
	void	OnStageTimerTick();

private :
	bool	CheckStageData();

private :
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG StageData", Meta = (AllowPrivateAccess = true))
	struct FPGStagePlayData StagePlayData;

	UPROPERTY()
	FTimerHandle StageTimerHandle;

	const struct FPGStageData* StageData;

	//===================================================
	// AD (광고)
	//===================================================
	bool m_bViewAD = false;
};
