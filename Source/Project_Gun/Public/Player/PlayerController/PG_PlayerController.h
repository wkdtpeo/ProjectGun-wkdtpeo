// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"

#include <GameFramework/PlayerController.h>
#include "PG_PlayerController.generated.h"


/**
 * PlayerController : 플레이어의 행동 관련하여 처리하도록 한다.
 * 클라이언트 느낌
-. 키입력을 받아 처리
-. UI 처리에 하기에 좋다
-. 틱마다 검사하는 일을 구현하기 좋다
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
	// Player Input 처리
	//==========================================================
	UFUNCTION(BlueprintCallable, Category = "PG TouchInterface")
	void SetLockJoystickInput(bool a_bLock);

	//==========================================================
	// Player 행동 처리
	//==========================================================
	void OnFire();			// 무기 발사
	void OnStateClear();	// 스테이지 클리어
	void OnStateFailed();	// 스테이지 클리어 실패

protected:
	//==========================================================
	// UI
	//==========================================================
	//UPROPERTY()
	//class UPG_UserWidgetPlayStage* UserWidgetPlayStage;
	UPROPERTY()
	class UPG_UserWidgetScreenPlayStage* m_pUserWidgetScreenPlayStage;
	UPROPERTY()
	class UPG_UserWidgetScreenStageClear* m_pWidgetStageClear;
	UPROPERTY()
	class UPG_UserWidgetScreenStageFailed* m_pUserWidgetScreenStageFailed;
};
