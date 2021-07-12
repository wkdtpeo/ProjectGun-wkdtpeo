// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Blueprint/UserWidget.h"

#include "PG_UserWidgetShopWeaponSlot.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnItemBuyWeapon);

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetShopWeaponSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public :
	UFUNCTION()
	void UpdateLayout();
	UFUNCTION()
	void SetUnlockLayout(bool bUnlock);
	UFUNCTION()
	void SetWeaponIndex(class APG_GameModeBase* pGameMode, int32 nWeaponIndex, int32 nSlotIndex);
	UFUNCTION()
	void SetWepaon(int32 nPrice);
	UFUNCTION()
	void SetPowerLevel(int32 nPowerLevel, int32 nPrice);
	UFUNCTION()
	void SetAmmoLevel(int32 nAmmoLevel, int32 nPrice);
	UFUNCTION()
	void ShowBuyPopupBox(enum EBuyType ekBuyType, FString kTitle, FString kDescription, int32 nPrice);
	UFUNCTION()
	void ShowMessageBox(FString kTitle, FString kDescription, int32 nValue);

protected :
	// 무기 언락 요청
	UFUNCTION(BlueprintCallable)
	void OnBtnWeapon();
	// 파워 레벨업 요청
	UFUNCTION(BlueprintCallable)
	void OnBtnPower();
	// 탄약 확장 요청
	UFUNCTION(BlueprintCallable)
	void OnBtnAmmo();

	UFUNCTION()
	void OnBuyOK(int32 nValue);
	UFUNCTION()
	void OnBuyCancel(int32 nValue);
	UFUNCTION()
	void OnMessagePopupBoxClose();
	
public:
	FOnItemBuyWeapon OnItemBuyWeapon;

protected :
	// Weapon
	PG_WIDGET_DECLAR(UButton, BtnWeapon);
	PG_WIDGET_DECLAR(UImage, ImgWeaponLock);
	PG_WIDGET_DECLAR(UTextBlock, TextWeaponPrice);
	PG_WIDGET_DECLAR(UCanvasPanel, CanvasWeaponPrice);
	

	// Power
	PG_WIDGET_DECLAR(UButton, BtnPower);
	PG_WIDGET_DECLAR(UTextBlock, TextPowerLevel);
	PG_WIDGET_DECLAR(UTextBlock, TextPowerPrice);
	PG_WIDGET_DECLAR(UCanvasPanel, CanvasPowerPrice);

	// Ammo
	PG_WIDGET_DECLAR(UButton, BtnAmmo);
	PG_WIDGET_DECLAR(UTextBlock, TextAmmoLevel);
	PG_WIDGET_DECLAR(UTextBlock, TextAmmoPrice);
	PG_WIDGET_DECLAR(UCanvasPanel, CanvasAmmoPrice);

private :
	int32 m_nSlotIndex = PG_INVALID_INDEX;
	int32 m_nWeaponIndex = PG_INVALID_INDEX;
	int32 m_nPowerLevel = PG_INVALID_INDEX;
	int32 m_nAmmoLevel = PG_INVALID_INDEX;
	bool m_bIsMaxPower = false;
	bool m_bIsMaxAmmo = false;
	bool m_bIsAbleBuyWepaon = false;
	bool m_bIsAbleBuyPower = false;
	bool m_bIsAbleBuyAmmo = false;

	UPROPERTY()
	TWeakObjectPtr<class APG_GameModeBase>	m_pGameMode;

	// 임시 작업
	UPROPERTY()
	class UPG_UserWidgetBuyPopup* m_pUserWidgetBuyPopup = nullptr;
	UPROPERTY()
	class UPG_UserWidgetMessagePopup* m_pUserWidgetMessagePopup = nullptr;
};
