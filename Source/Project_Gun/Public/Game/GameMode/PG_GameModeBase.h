﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"

#include <GameFramework/GameModeBase.h>
#include "PG_GameModeBase.generated.h"

extern UWorld* G_PGWorld = nullptr;
/**
 * 
 */
UCLASS()
class PROJECT_GUN_API APG_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public :
	APG_GameModeBase();
	virtual void PostLoad() override;
	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void Tick(float DeltaTime) override;
	
public :
	// 각각의 GameMode에 따라 IsClearMission 검사 조건이 틀림
	virtual bool IsClearMission(EStageClearType a_ekStageClearType) { return false; }

	//===================================================
	// Game
	//===================================================
	UFUNCTION(BlueprintCallable, Category = "PG Stage")
	bool	StartGame();

	void NextStage();
	void ResetStage();

	UFUNCTION(BlueprintCallable, Category = "PG Stage")
	bool OnNextSelectStage();
	UFUNCTION(BlueprintCallable, Category = "PG Stage")
	bool OnPrevSelectStage();
	UFUNCTION(BlueprintCallable, Category = "PG Stage")
	bool SetPlayStageID(int32 nPlayStageID);
	UFUNCTION(BlueprintCallable, Category = "PG Stage")
	int32 GetPlayStageID()	{ return m_nPalyStageID; }

	//===================================================
	// Stage
	//===================================================
	//UFUNCTION(BlueprintCallable, Category = "PG Stage")
	//void	SetPlayStageID(int32 a_PlayStageID) { m_nPalyStageID = a_PlayStageID; }
	//UFUNCTION(BlueprintCallable, Category = "PG Stage")
	//int32	GetPlayStageID() { return m_nPalyStageID; }
	bool	ChangeSelectStageData(int32 a_nStageID);		// 스테이지 데이터를 교체한다.
	bool	IsFirstPlayStage(int32 a_nStageID);
	

	//===================================================
	// Weapon
	//===================================================
	// 무기 언락
	bool IsUnlockWeapon(int32 a_nWeaponIndex);
	bool IsMaxLevelPower(int32 a_nWeaponIndex);
	bool IsMaxLevelAmmo(int32 a_nWeaponIndex);
	bool IsAbleBuyRewardPointPower(int32 a_nWeaponIndex);
	bool IsAbleBuyRewardPointAmmo(int32 a_nWeaponIndex);
	// 무기 구매
	bool BuyWeapon(int32 a_nWeaponIndex);
	bool BuyPowerUp(int32 a_nWeaponIndex);
	bool BuyAmmoUp(int32 a_nWeaponIndex);
	FString GetLastBuyErrorMsg();
	// 무기 속성
	int32 GetMaxPower(int32 a_nWeaponIndex);
	int32 GetMaxAmmo(int32 a_nWeaponIndex);

	//===================================================
	// Save Data
	//===================================================
	class UPG_SavePlayerData*	GetSavePlayerData()			{ return m_pSavePlayerData; }
	class UPG_SaveStageData*	GetSelectedSaveStageData()	{ return m_pSelectSaveStageData; }
	// 저장
	virtual bool SavePlayerData();		// m_pSavePlayerData에 쓰여진 플레이어 데이터를 저장한다
	virtual bool SaveStageData();		// m_pSelectSaveStageData에 쓰여진 스테이지 플레이 데이터를 저장한다

protected:
	//==========================================================
	// SavePlayerData (플레이어 저장 데이터)
	//==========================================================
	class UPG_SavePlayerData*	LoadSavePlayerData();

	//==========================================================
	// SaveStageData (스테이지 저장 데이터)
	//==========================================================
	class UPG_SaveStageData*	LoadSaveStageData(int32 nStageID);

private:
	void UpdateGameInstance(float DeltaTime);

protected :
	UPROPERTY()
	class UPG_SavePlayerData*	m_pSavePlayerData = nullptr;			// 저장된 유저 데이터
	UPROPERTY()
	class UPG_SaveStageData*	m_pSelectSaveStageData = nullptr;		// 저장된 스테이지 데이터
	UPROPERTY()
	FString m_kLastBuyErrorMsg;
	UPROPERTY()
	int32 m_nPalyStageID = 0;	// 현재 스테이지ID
};
