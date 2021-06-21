// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <Engine/DataTable.h>
#include <Engine/GameInstance.h>
#include <Engine/StreamableManager.h>
#include "Define/PG_TableData.h"
#include "Game/Console/PG_Console.h"
#include "Game/AD/PG_ADData.h"
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
	void NextStage();
	void ResetStage();

	UFUNCTION(BlueprintCallable, Category = "PG Stage")
	bool OnNextSelectStage();
	UFUNCTION(BlueprintCallable, Category = "PG Stage")
	bool OnPrevSelectStage();
	UFUNCTION(BlueprintCallable, Category = "PG Stage")
	bool SetPlayStageID(int32 nPlayStageID);
	UFUNCTION(BlueprintCallable, Category = "PG Stage")
	int32 GetPlayStageID()	{ return PalyStageID; }
	UFUNCTION(BlueprintCallable, Category = "PG Stage")
	bool StartGame();


public :
	// ĳ����
	FPGCharacterMovementData*	GetTableCharacterMovementData();
	FPGCharacterCameraData*		GetTableCharacterCameraData();

	// ���� ��
	const FPGWeaponTableData*	GetWeaponTableData(int32 nIndex);
	const bool					GetAllWeaponTableData(OUT TArray<const FPGWeaponTableData*>& a_rkWeaponTableDatas);

	// ��������
	const FPGStageData*			GetTableStageData(int32 nID);
	const bool					GetAllTableStageData(OUT TArray<const FPGStageData*>& a_rkStageDatas);

	// �ʻ��
	const FPGLethalAttackData*	GetTableLethalAttackData(int32 nID);

	// ����
	const FPGADTableData*		GetADTableData(EADType eADType);
	int32						GetADRemainTime(EADType eADType);
	bool						IsAbleADView(EADType eADType);
	void						ResetADRemainTime(EADType eADType);

	// �ܼ�
	class PG_Console*			GetConsole()	{ return &m_kConsole; }

private :
	bool LoadTable();
	bool LoadConsoleCommand();

public :
	FStreamableManager StreamableManager;

	int32 PalyStageID; // ù��° ���������� ID ���� 1���� �����Ѵ�.
	FString GetStageName(int32 StageID);

private :

	// ���̺� Ŭ���� ���� �ʿ�..
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

	// �ܼ� ��ɾ�
	class PG_Console m_kConsole;

	// ����
	class PG_ADData m_kADData;
};
