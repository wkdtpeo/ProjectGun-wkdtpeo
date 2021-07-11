// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <GameFramework/PlayerController.h>
#include "PG_PlayerController.generated.h"


/**
 * PlayerController : 플레이어의 행동 관련하여 처리하도록 한다.
 */
UCLASS()
class PROJECT_GUN_API APG_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APG_PlayerController();

	virtual void CreateTouchInterface() override;		//  조이스틱의 터치 이벤트를 컨트롤하기 위한 override
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public :
	//==========================================================
	// Monster 처리
	//==========================================================
	void MonsterKill(class APG_MonChar* KillMon);					// 일반적으로 몬스터를 죽였을경우
	void MonsterLethalAttackKill(class APG_MonChar* KillMon);		// 필살기를 이용하여 몬스터를 죽였을경우

	//==========================================================
	// Player Input 처리
	//==========================================================
	UFUNCTION(BlueprintCallable, Category = "PG TouchInterface")
	void SetLockJoystickInput(bool a_bLock);

public :
	void OnTryLethalAttack();
	void OnFire();

protected :
	void OnStateClear();
	void OnStateFailed(); 

protected:
	//==========================================================
	// UI
	//==========================================================
	//UPROPERTY()
	//class UPG_UserWidgetPlayStage* UserWidgetPlayStage;
	UPROPERTY()
	class UPG_UserWidgetScreenPlayStage* UserWidgetScreenPlayStage;
	UPROPERTY()
	class UPG_UserWidgetScreenStageClear* WidgetStageClear;
	UPROPERTY()
	class UPG_UserWidgetScreenStageFailed* UserWidgetScreenStageFailed;
};
