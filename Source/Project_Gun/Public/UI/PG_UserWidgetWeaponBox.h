// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"

#include <Blueprint/UserWidget.h>
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
	// Weapon 버튼을 누르면 설정된 WeaponIndex를 전달한다.
	FOnBtnWeaponClicked_WeaponIndex OnBtnWeaponClicked_WeaponIndex;

	// Weapon 버튼을 누르면 장착된 SlotIndex를 전달한다.
	FOnBtnWeaponClicked_SlotIndex OnBtnWeaponClicked_SlotIndex;

private :
	UFUNCTION()
	void OnBtnWeaponClicked();

protected :
	PG_WIDGET_DECLAR(UButton, Btn_Weapon);
	PG_WIDGET_DECLAR(UImage, Img_IconAmmo);
	PG_WIDGET_DECLAR(UTextBlock, Text_Ammo);
	PG_WIDGET_DECLAR(UOverlay, OverlayLightBox);

	UPROPERTY(BlueprintReadWrite)
	int32 WeaponIndex = PG_INVALID_INDEX;

	UPROPERTY(BlueprintReadWrite)
	int32 SlotIndex = PG_INVALID_INDEX;

private :
	UPROPERTY()
	TWeakObjectPtr<class APG_GameModeBase>	m_pGameMode;
};
