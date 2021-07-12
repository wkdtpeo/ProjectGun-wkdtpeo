// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Game/Console/PG_Console.h"
#include "Game/AD/PG_ADData.h"

#include <Engine/DataTable.h>
#include <Engine/GameInstance.h>
#include <Engine/StreamableManager.h>
#include "PG_GameInstance.generated.h"

/**
 * 
 */

UCLASS()
class PROJECT_GUN_API UPG_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public :
	UPG_GameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;

	// From GameMode Tick()
	void FromGameModeTick(float DeltaTime);


public :
	//void StartGame_NextStage();
	//void RestartGame();

	//UFUNCTION(BlueprintCallable, Category = "PG Stage")
	//bool OnNextSelectStage();
	//UFUNCTION(BlueprintCallable, Category = "PG Stage")
	//bool OnPrevSelectStage();
	//UFUNCTION(BlueprintCallable, Category = "PG Stage")
	//bool SetPlayStageID(int32 nPlayStageID);
	//UFUNCTION(BlueprintCallable, Category = "PG Stage")
	//int32 GetPlayStageID()	{ return PalyStageID; }
	//UFUNCTION(BlueprintCallable, Category = "PG Stage")
	//bool StartGame();


public :
	// 캐릭터
	FPGCharacterMovementData*	GetTableCharacterMovementData();
	FPGCharacterCameraData*		GetTableCharacterCameraData();

	// 무기 샵
	const FPGWeaponTableData*	GetWeaponTableData(int32 nIndex);
	const bool					GetAllWeaponTableData(OUT TArray<const FPGWeaponTableData*>& a_rkWeaponTableDatas);

	// 스테이지
	const FPGStageData*			GetTableStageData(int32 nID);
	const bool					GetAllTableStageData(OUT TArray<const FPGStageData*>& a_rkStageDatas);

	// 필살기
	const FPGLethalAttackData*	GetTableLethalAttackData(int32 nID);

	// 광고
	const FPGADTableData*		GetADTableData(EADType eADType);
	int32						GetADRemainTime(EADType eADType);
	bool						IsAbleADView(EADType eADType);
	void						ResetADRemainTime(EADType eADType);

	// 콘솔
	class PG_Console*			GetConsole()	{ return &m_kConsole; }

private :
	bool LoadTable();
	bool LoadConsoleCommand();

public :
	FStreamableManager StreamableManager;

	//int32 PalyStageID; // 첫번째 스테이지의 ID 값은 1부터 시작한다.
	FString GetStageName(int32 StageID);

private :
	// 테이블 클래스 정리 필요..
	UPROPERTY()
	class UDataTable* m_pCharacterMovementTable = nullptr;

	UPROPERTY()
	class UDataTable* m_pCharacterCameraTable = nullptr;

	UPROPERTY()
	class UDataTable* m_pWeaponTable = nullptr;

	UPROPERTY()
	class UDataTable* m_pLethalAttackTable = nullptr;

	UPROPERTY()
	class UDataTable* m_pStageTable = nullptr;

	UPROPERTY()
	class UDataTable* m_pADTable = nullptr;

	// 콘솔 명령어
	class PG_Console m_kConsole;

	// 광고
	class PG_ADData m_kADData;


};
