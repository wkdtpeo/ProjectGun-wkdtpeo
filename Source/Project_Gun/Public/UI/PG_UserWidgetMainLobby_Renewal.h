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
	// ���õ� �������� �̼� ������Ʈ
	UFUNCTION(BlueprintCallable)
	void UpdateLayoutMission();
	// ���� ���� ����
	UFUNCTION(BlueprintCallable)
	void UpdateLayoutWeaponSlot();
	// ����Ʈ ����
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRewardPoint();

public :
	// �ɼ�
	UFUNCTION(BlueprintCallable)
	void OnBtnOption();

	// �������� ����
	UFUNCTION(BlueprintCallable)
	void OnBtnSelectStageNext();
	UFUNCTION(BlueprintCallable)
	void OnBtnSelectStagePrev();
	
	// ���ӽ���
	UFUNCTION(BlueprintCallable)
	void OnBtnGameStart();

	// �����
	UFUNCTION(BlueprintCallable)
	void OnBtnShop();

	// ���� ����
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
	//PG_DECLAR_WIDGET(UUserWidget, WB_RewardPoint);
	PG_DECLAR_WIDGET(UButton, BtnOption);
	
	//===========================================
	// Canvas StageSelect
	//===========================================
	PG_DECLAR_WIDGET(UButton, BtnSelectStagePrev);
	PG_DECLAR_WIDGET(UButton, BtnSelectStageNext);
	PG_DECLAR_WIDGET(UTextBlock, TextSelectedStage);
	PG_DECLAR_WIDGET(UCanvasPanel, CanvasSelectStageLock);
	

	//===========================================
	// Canvas Mission
	//===========================================
	PG_DECLAR_WIDGET(UButton, BtnFlag);
	PG_DECLAR_WIDGET(UButton, BtnLethal);
	PG_DECLAR_WIDGET(UButton, BtnTime);
	PG_DECLAR_WIDGET(UTextBlock, TextMainLobbyFlag);
	PG_DECLAR_WIDGET(UTextBlock, TextMainLobbyLethal);
	PG_DECLAR_WIDGET(UTextBlock, TextMainLobbyTime);

	//===========================================
	// Canvas MenuButtons
	//===========================================
	PG_DECLAR_WIDGET(UButton, BtnGameStart);
	PG_DECLAR_WIDGET(UButton, BtnShop);
	PG_DECLAR_WIDGET(UCanvasPanel, CanvasGameStart);
	

	//===========================================
	// Canvas WeaponBox
	//===========================================
	PG_DECLAR_WIDGET_TArray(UPG_UserWidgetWeaponBox, WB_WeaponBox);

	//===========================================
	// Canvas ADBanner
	//===========================================

	//===========================================
	// Canvas WB_WeaponInventory 
	//===========================================
	PG_DECLAR_WIDGET(UScaleBox, ScaleWeaponInventory);
	PG_DECLAR_WIDGET(UPG_UserWidgetWeaponInventory, WB_WeaponInventory);

public :
	// �����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PG Setting WidgetClass")
	TSubclassOf <class UPG_UserWidgetScreenShop> WB_ShopClass;
	UPROPERTY()
	class UPG_UserWidgetScreenShop* m_pWBShop = nullptr;
};
