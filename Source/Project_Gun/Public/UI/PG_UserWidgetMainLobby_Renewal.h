// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <Blueprint/UserWidget.h>
#include "PG_UserWidgetMainLobby_Renewal.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetMainLobby_Renewal : public UUserWidget
{
	GENERATED_BODY()

	enum
	{
		eMaxWeaponSlot = PG_MAX_WEAPON_SLOT,
	};

protected:
	virtual void NativeConstruct() override;

public :
	UFUNCTION(BlueprintCallable)
	void UpdateLayoutStage();
	// 선택된 스테이지 미션 업데이트
	UFUNCTION(BlueprintCallable)
	void UpdateLayoutMission();
	// 무기 슬롯 갱신
	UFUNCTION(BlueprintCallable)
	void UpdateLayoutWeaponSlot();
	// 포인트 갱신
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRewardPoint();

public :
	// 옵션
	UFUNCTION(BlueprintCallable)
	void OnBtnOption();

	// 스테이지 선택
	UFUNCTION(BlueprintCallable)
	void OnBtnSelectStageNext();
	UFUNCTION(BlueprintCallable)
	void OnBtnSelectStagePrev();
	
	// 게임시작
	UFUNCTION(BlueprintCallable)
	void OnBtnGameStart();

	// 무기고
	UFUNCTION(BlueprintCallable)
	void OnBtnShop();

	// 무기 선택
	UFUNCTION()
	void OnOpenWeaponInventory(int32 nSlotIndex);
	UFUNCTION()
	void OnCloseWeaponInventory(int32 nSlotIndex, int32 nWeaponIndex);
	UFUNCTION(BlueprintCallable)
	void OnDoNotChangeCloseWeaponInventory();

public:
	//===========================================
	// Canvas TopMenu
	//===========================================
	//PG_WIDGET_DECLAR(UUserWidget, WB_RewardPoint);
	PG_WIDGET_DECLAR(UButton, BtnOption);
	
	//===========================================
	// Canvas StageSelect
	//===========================================
	PG_WIDGET_DECLAR(UButton, BtnSelectStagePrev);
	PG_WIDGET_DECLAR(UButton, BtnSelectStageNext);
	PG_WIDGET_DECLAR(UTextBlock, TextSelectedStage);
	PG_WIDGET_DECLAR(UCanvasPanel, CanvasSelectStageLock);
	

	//===========================================
	// Canvas Mission
	//===========================================
	PG_WIDGET_DECLAR(UButton, BtnFlag);
	PG_WIDGET_DECLAR(UButton, BtnLethal);
	PG_WIDGET_DECLAR(UButton, BtnTime);
	PG_WIDGET_DECLAR(UTextBlock, TextMainLobbyFlag);
	PG_WIDGET_DECLAR(UTextBlock, TextMainLobbyLethal);
	PG_WIDGET_DECLAR(UTextBlock, TextMainLobbyTime);

	//===========================================
	// Canvas MenuButtons
	//===========================================
	PG_WIDGET_DECLAR(UButton, BtnGameStart);
	PG_WIDGET_DECLAR(UButton, BtnShop);
	PG_WIDGET_DECLAR(UCanvasPanel, CanvasGameStart);
	

	//===========================================
	// Canvas WeaponBox
	//===========================================
	PG_WIDGET_DECLAR_TArray(UPG_UserWidgetWeaponBox, WB_WeaponBox);

	//===========================================
	// Canvas ADBanner
	//===========================================

	//===========================================
	// Canvas WB_WeaponInventory 
	//===========================================
	PG_WIDGET_DECLAR(UScaleBox, ScaleWeaponInventory);
	PG_WIDGET_DECLAR(UPG_UserWidgetWeaponInventory, WB_WeaponInventory);

public :
	// 무기고
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PG Setting WidgetClass")
	TSubclassOf <class UPG_UserWidgetScreenShop> WB_ShopClass;
	UPROPERTY()
	class UPG_UserWidgetScreenShop* m_pWBShop = nullptr;
};
