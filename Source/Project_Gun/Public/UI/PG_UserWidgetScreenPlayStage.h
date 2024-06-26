﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"

#include <Blueprint/UserWidget.h>
#include "PG_UserWidgetScreenPlayStage.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetScreenPlayStage : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public :
	void Init();

public :
	// 포인트 획득
	UFUNCTION(BlueprintImplementableEvent)
	void OnChangeRewardPoint();

	void OnFire();
	void OnChangeWeapon();

private :
	void InitPlayStageWeaponBag();
	void InitPlayMissionState();

	void UpdateWeaponSlotLightBox();

private :
	PG_WIDGET_DECLAR(UPG_UserWidgetPlayMissionState, WB_MissionState);
	PG_WIDGET_DECLAR(UPG_UserWidgetPlayStageWeaponBag, WB_PlayStageWeaponBag);
};
