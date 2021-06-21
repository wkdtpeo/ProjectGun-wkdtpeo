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
		// ������� �ʵ��� ���� ����
		eMaxLethalShopItemSlot = 3,		// �ʻ�� 
		eMaxWeaponShopItemSlot = 5,		// ����
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
	// ���� ��û�� ���� ���
	//============================================================================================
	UFUNCTION(BlueprintCallable)
	void OnADRewardVideoSucessed();		// ������ ������ ���� ���������� ������
	UFUNCTION(BlueprintCallable)
	void OnADRewardVideoClosed();		// ������ ������ ���� ������ �ݰų� �����Ͽ���
	//============================================================================================

	UPROPERTY()
	class UButton* m_pBtn_ADView;

	//UPROPERTY()
	//class UTextBlock* Text_ADRemainTime;
	PG_DECLAR_WIDGET(UTextBlock, Text_ADRemainTime);
};
