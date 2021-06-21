// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <Blueprint/UserWidget.h>
#include "Define/PG_TableData.h"
#include "PG_UserWidgetWeaponBox.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnBtnWeaponClicked_WeaponIndex, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBtnWeaponClicked_SlotIndex, int32);
/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetWeaponBox : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public :
	UFUNCTION(BlueprintCallable)
	void SetWeaponIndex(class APG_GameModeBase* pGameMode, int32 nWeaponIndex);
	UFUNCTION(BlueprintCallable)
	void SetSlotIndex(int32 nSlotIndex);
	UFUNCTION(BlueprintCallable)
	void SetAmmo(int32 nAmmo);
	UFUNCTION(BlueprintCallable)
	void SetEnable(bool bEnable);
	void SetEnableLightBox(bool bEnable);

	UFUNCTION(BlueprintCallable)
	int32 GetWeaponIndex()		{ return WeaponIndex;	}

public :
	// Weapon ��ư�� ������ ������ WeaponIndex�� �����Ѵ�.
	FOnBtnWeaponClicked_WeaponIndex OnBtnWeaponClicked_WeaponIndex;

	// Weapon ��ư�� ������ ������ SlotIndex�� �����Ѵ�.
	FOnBtnWeaponClicked_SlotIndex OnBtnWeaponClicked_SlotIndex;

private :
	UFUNCTION()
	void OnBtnWeaponClicked();

protected :
	PG_DECLAR_WIDGET(UButton, Btn_Weapon);
	PG_DECLAR_WIDGET(UImage, Img_IconAmmo);
	PG_DECLAR_WIDGET(UTextBlock, Text_Ammo);
	PG_DECLAR_WIDGET(UOverlay, OverlayLightBox);

	UPROPERTY(BlueprintReadWrite)
	int32 WeaponIndex = PG_INVALID_INDEX;

	UPROPERTY(BlueprintReadWrite)
	int32 SlotIndex = PG_INVALID_INDEX;

private :
	UPROPERTY()
	TWeakObjectPtr<class APG_GameModeBase>	m_pGameMode;
};
