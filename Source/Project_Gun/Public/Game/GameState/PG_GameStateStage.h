// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <GameFramework/GameStateBase.h>
#include "Define/PG_GamePlayData.h"
#include "PG_GameStateStage.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStageTimer);
DECLARE_MULTICAST_DELEGATE(FOnStageClear);
DECLARE_MULTICAST_DELEGATE(FOnStageFailed);
DECLARE_MULTICAST_DELEGATE(FOnUdpateWidgetPlayStage);
DECLARE_MULTICAST_DELEGATE(FOnGateOpen);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnViewAD, bool);

/**
 * GameState : ������ ���¸� �����Ѵ�. ������ ���۵Ǿ�����, �ߴܵǾ�����, A���� ���� �������� ���� �����Ѵ�.
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

	bool	IsStageClear();

	void	MonsterDie(class APG_MonChar* KillMon, bool IsLethalAttack);

	int32	GetLevelInstallMonsterCount();
	int32	GetRemainStageTimer();
	int32	GetMaxLethalKill();

	void	UpdateMaxLethalKill(int32 a_nMaxLethalKill);		// �ʻ��� ���� ���� �� ����

	const struct FPGStagePlayData* GetStagePlayData() { return &StagePlayData; }

	//===================================================
	// AD (����)
	//===================================================
	bool IsViewAD();		// ���� ���Ҵ��� Ȯ��
	void SetViewAD(bool bEnable);
	bool RequestViewAD();	// ���� ��û

public:
	//==========================================================
	// Stage
	//==========================================================
	FOnStageTimer	StageTimerDelegate;		// �������� Ÿ�̸�
	FOnStageClear	StageClearDelegate;		// �������� Ŭ����
	FOnStageFailed	StageFailedDelegate;	// �������� ����
	FOnGateOpen		GateOpenDelegate;		// �������� Ŭ���� ����Ʈ ����
	FOnUdpateWidgetPlayStage UdpateWidgetPlayStage;		// �������� Ŭ���� ��ǥ ��Ȳ ������Ʈ ��û

	//==========================================================
	// AD
	//==========================================================
	FOnViewAD		OnViewAD;				// ���� ������

public:
	void	OnCheckStageClear();
	void	OnCheckStageFailed();

	UFUNCTION()
	void	OnKillAddRewardPoint(int32 a_nAddRewardPoint);	// ���� óġ�� ���� ����Ʈ ȹ��

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
	// AD (����)
	//===================================================
	bool m_bViewAD = false;
};
