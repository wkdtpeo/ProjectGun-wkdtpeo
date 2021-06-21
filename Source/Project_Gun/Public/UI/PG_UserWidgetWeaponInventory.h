// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <Blueprint/UserWidget.h>
#include "PG_UserWidgetWeaponInventory.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnBtnClickedClose, int32, int32);	// FOnBtnClickedClose, SlotIndex, WeaponIndex
/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetWeaponInventory : public UUserWidget
{
	GENERATED_BODY()
	
	enum
	{
		eMaxInvenIndex = 6,
	};
	
protected:
	virtual void NativeConstruct() override;

public :
	UFUNCTION(BlueprintCallable)
	void UpdateWeaponInventory();

	UFUNCTION(BlueprintCallable)
	void SetOpenSlotIndex(int32 nSlotIndex);

protected :
	UFUNCTION()
	void OnSelectWeaponIndex(int32 nWeaponIndex);

	UFUNCTION(BlueprintCallable)
	void OnBtnClose();

public :
	FOnBtnClickedClose OnBtnClickedClose;

private:
	int32 m_nOpenSlotIndex = PG_INVALID_INDEX;
};
