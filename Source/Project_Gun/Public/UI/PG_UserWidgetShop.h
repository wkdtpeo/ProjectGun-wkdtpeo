// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <Blueprint/UserWidget.h>
#include "PG_UserWidgetShop.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetShop : public UUserWidget
{
	GENERATED_BODY()
	
	enum
	{	
		// 사용하지 않도록 개선 예정
		eMaxLethalShopItemSlot = 3,		// 필살기 
		eMaxWeaponShopItemSlot = 5,		// 무기
	};
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

public :
	UFUNCTION(BlueprintCallable)
	void UpdateLethalShopLayout();
	UFUNCTION(BlueprintCallable)
	void UpdateWeaponShopLayout();
	UFUNCTION(BlueprintImplementableEvent, Category = "RewardPoint")
	void UpdateRewardPoint();

	void SetBtnEnableAD(bool bEnable);
	void SetADRemainTime(int32 nTime);

public :
	//============================================================================================
	// 광고 시청에 대한 결과
	//============================================================================================
	UFUNCTION(BlueprintCallable)
	void OnADRewardVideoSucessed();		// 유저가 보상형 광고를 성공적으로 보았음
	UFUNCTION(BlueprintCallable)
	void OnADRewardVideoClosed();		// 유저가 보상형 광고 보는중 닫거나 실패하였음
	//============================================================================================

	UPROPERTY()
	class UButton* m_pBtn_ADView;

	//UPROPERTY()
	//class UTextBlock* Text_ADRemainTime;
	PG_DECLAR_WIDGET(UTextBlock, Text_ADRemainTime);
};
