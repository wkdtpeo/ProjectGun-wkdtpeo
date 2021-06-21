// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Game/GameMode/PG_GameModeBase.h"
#include <Blueprint/UserWidget.h>
#include "PG_UserWidgetShopWeaponBox.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnItemBuyWeapon);

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetShopWeaponBox : public UUserWidget
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
	void SetPowerLevel(int32 nPowerLevel);
	UFUNCTION(BlueprintCallable)
	void SetAmmoLevel(int32 nAmmoLevel);
	UFUNCTION(BlueprintCallable)
	void SetEnable(bool bEnable);

	UFUNCTION(BlueprintCallable)
	void UpdateWeaponShopLayout();

private :
	UFUNCTION()
	void ShowBuyPopupBox(enum EBuyType ekBuyType, FString kTitle, FString kDescription, int32 nPrice);
	UFUNCTION()
	void ShowMessageBox(FString kTitle, FString kDescription, int32 nValue);


public :
	UFUNCTION()
	void OnBuyOK(int32 nValue);
	UFUNCTION()
	void OnBuyCancel(int32 nValue);
	UFUNCTION()
	void OnMessagePopupBoxClose();

protected :
	// 무기 언락 요청
	UFUNCTION()
	void OnRequestUnlockWeapon();
	// 파워 레벨업 요청
	UFUNCTION()
	void OnRequestLevelupPower();
	// 탄약 확장 요청
	UFUNCTION()
	void OnRequestLevelupAmmo();

public :
	FOnItemBuyWeapon OnItemBuyWeapon;
	
protected :
	UPROPERTY(BlueprintReadWrite)
	int32 WeaponIndex = PG_INVALID_INDEX;
	UPROPERTY(BlueprintReadWrite)
	int32 SlotIndex = PG_INVALID_INDEX;
	UPROPERTY(BlueprintReadWrite)
	int32 PowerLevel = PG_INVALID_INDEX;
	UPROPERTY(BlueprintReadWrite)
	int32 AmmoLevel = PG_INVALID_INDEX;
	UPROPERTY(BlueprintReadWrite)
	bool IsMaxPower = false;
	UPROPERTY(BlueprintReadWrite)
	bool IsMaxAmmo = false;
	UPROPERTY(BlueprintReadWrite)
	bool IsAbleBuyWepaon = false;
	UPROPERTY(BlueprintReadWrite)
	bool IsAbleBuyPower = false;
	UPROPERTY(BlueprintReadWrite)
	bool IsAbleBuyAmmo = false;

	UPROPERTY(BlueprintReadWrite)
	class UButton* Btn_Weapon;
	UPROPERTY(BlueprintReadWrite)
	class UButton* Btn_Power;
	UPROPERTY(BlueprintReadWrite)
	class UButton* Btn_Ammo;
	UPROPERTY(BlueprintReadWrite)
	class UTextBlock* Text_PowerLevel;
	UPROPERTY(BlueprintReadWrite)
	class UTextBlock* Text_AmmoLevel;

	UPROPERTY(BlueprintReadWrite)
	class UImage* Img_WeaponLock;
	UPROPERTY(BlueprintReadWrite)
	class UImage* Img_PowerUp;
	UPROPERTY(BlueprintReadWrite)
	class UImage* Img_AmmoUp;

private :
	UPROPERTY()
	TWeakObjectPtr<class APG_GameModeBase>	m_pGameMode;

	UPROPERTY()
	class UPG_UserWidgetBuyPopup*		m_pUserWidgetBuyPopup;
	UPROPERTY()
	class UPG_UserWidgetMessagePopup*	m_pUserWidgetMessagePopup;
	
};
