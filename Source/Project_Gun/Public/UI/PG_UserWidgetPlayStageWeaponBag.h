// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Blueprint/UserWidget.h"
#include "PG_UserWidgetPlayStageWeaponBag.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetPlayStageWeaponBag : public UUserWidget
{
	GENERATED_BODY()

	enum
	{
		eMaxWeaponSlot = PG_MAX_WEAPON_SLOT,
	};

protected:
	virtual void NativeConstruct() override;

public :
	bool SetSlotWaponIndex(int32 nSlotIndex, int32 nWeaponIndex);
	bool SetSlotWeaponAmmo(int32 nSlotIndex, int32 nAmmo);
	bool SetEnableSlot(int32 nSlotIndex, bool bEnable);
	void SetEnableLightBox(int32 nEnableSlotIndex);

protected:
	void OnBtnWeaponSlotClicked(int32 nSlotIndex);

protected:
	PG_DECLAR_WIDGET_TArray(UPG_UserWidgetWeaponBox, WB_WeaponSlot);
};
