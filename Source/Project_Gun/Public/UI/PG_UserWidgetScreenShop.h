// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Blueprint/UserWidget.h"

#include "PG_UserWidgetScreenShop.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetScreenShop : public UUserWidget
{
	GENERATED_BODY()

	enum
	{
		// 사용하지 않도록 개선 예정
		eMaxLethalShopItemSlot = 3,		// 필살기 
		eMaxWeaponShopItemSlot = 3,		// 무기
	};

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

public :
	void Init();

	UFUNCTION(BlueprintCallable)
	void UpdateLethalShopLayout();
	UFUNCTION(BlueprintCallable)
	void UpdateWeaponShopLayout();
	UFUNCTION(BlueprintImplementableEvent, Category = "RewardPoint")
	void UpdateRewardPoint();

	void SetBtnEnableAD(bool bEnable);
	void SetADRemainTime(int32 nTime);

	void ShowMessageBox(FString kTitle, FString kDescription);

protected:
	UFUNCTION(BlueprintCallable)
	void OnBtnGotoLobby();
	UFUNCTION(BlueprintCallable)
	void OnBtnADView();

	//============================================================================================
	// 광고 시청에 대한 결과
	//============================================================================================
	UFUNCTION(BlueprintCallable)
	void OnADRewardVideoSucessed();		// 유저가 보상형 광고를 성공적으로 보았음
	UFUNCTION(BlueprintCallable)
	void OnADRewardVideoError();		// 유저가 보상형 광고 보는중 닫거나 실패하였음
	//============================================================================================

	// 메시지 팝업박스
	UFUNCTION()
	void OnMessagePopupBoxClose();

protected:
	PG_WIDGET_DECLAR(UButton, BtnGotoLobby);
	PG_WIDGET_DECLAR(UButton, BtnADView);
	PG_WIDGET_DECLAR(UTextBlock, TextADRemainTime);
	
	PG_WIDGET_DECLAR_TArray(UPG_UserWidgetSkillSlot, WB_SkillSlot);
	PG_WIDGET_DECLAR_TArray(UPG_UserWidgetShopWeaponSlot, WB_ShopWeaponSlot);

public:
	// 메인로비
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PG Setting WidgetClass")
	TSubclassOf <class UPG_UserWidgetMainLobby_Renewal> WB_MainLobbyClass;
	UPROPERTY()
	class UPG_UserWidgetMainLobby_Renewal* m_pWBMainLobby = nullptr;

	// 메시지 팝업
	UPROPERTY()
	class UPG_UserWidgetMessagePopup* m_pUserWidgetMessagePopup = nullptr;
};
