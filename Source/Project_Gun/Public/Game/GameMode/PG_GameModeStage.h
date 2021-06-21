// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <GameFramework/GameModeBase.h>
#include "Game/GameMode/PG_GameModeBase.h"
#include "PG_GameModeStage.generated.h"

/**
 * GameMode : ������ �� ���� ���� ��带 ������ ������ ���� �����Ѵ�.
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

	// ���� �� Ÿ���� �̼� Ŭ���� ����
	virtual bool IsClearMission(EStageClearType a_ekStageClearType) override;	
	// ������ �� Ÿ���� �̼� Ŭ���� ����
	bool IsWasClearMisiion(EStageClearType a_ekStageClearType);
	// �������� Ŭ���� ����
	bool IsStageClear();

	// �÷������� �������� ID
	int32 GetPlayStageID();
	// ȹ���� ����Ʈ (���� : �������� Ŭ���� + �̼� Ŭ���� + ���� óġ + ����)
	int32 GetClearTotalPoint();
	// �������� Ŭ���� ����Ʈ
	int32 GetClearStagePoint();
	// �̼� Ŭ���� ����Ʈ
	int32 GetClearMissionPoint();
	// ���� óġ ����Ʈ
	int32 GetClearMonsterKillPoint();

	//===================================================
	// Save Data
	//===================================================
	virtual bool SavePlayerData() override;
	virtual bool SaveStageData() override;

public:
	//============================================
	// �÷��̾� ������ (Player)
	//============================================
	bool SetSavePlayerData(const struct FPGPlayerData& a_rkPlayerData);

	//============================================
	// �������� ������ (Stage)
	//============================================
	bool SetSaveStageData(const struct FPGStagePlayData& a_rkStagePlayData);

};
