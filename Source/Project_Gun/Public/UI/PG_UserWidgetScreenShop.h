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
		// ������� �ʵ��� ���� ����
		eMaxLethalShopItemSlot = 3,		// �ʻ�� 
		eMaxWeaponShopItemSlot = 3,		// ����
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
	// ���� ��û�� ���� ���
	//============================================================================================
	UFUNCTION(BlueprintCallable)
	void OnADRewardVideoSucessed();		// ������ ������ ���� ���������� ������
	UFUNCTION(BlueprintCallable)
	void OnADRewardVideoError();		// ������ ������ ���� ������ �ݰų� �����Ͽ���
	//============================================================================================

	// �޽��� �˾��ڽ�
	UFUNCTION()
	void OnMessagePopupBoxClose();

protected:
	PG_DECLAR_WIDGET(UButton, BtnGotoLobby);
	PG_DECLAR_WIDGET(UButton, BtnADView);
	PG_DECLAR_WIDGET(UTextBlock, TextADRemainTime);
	
	PG_DECLAR_WIDGET_TArray(UPG_UserWidgetSkillSlot, WB_SkillSlot);
	PG_DECLAR_WIDGET_TArray(UPG_UserWidgetShopWeaponSlot, WB_ShopWeaponSlot);

public:
	// ���ηκ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PG Setting WidgetClass")
	TSubclassOf <class UPG_UserWidgetMainLobby_Renewal> WB_MainLobbyClass;
	UPROPERTY()
	class UPG_UserWidgetMainLobby_Renewal* m_pWBMainLobby = nullptr;

	// �޽��� �˾�
	UPROPERTY()
	class UPG_UserWidgetMessagePopup* m_pUserWidgetMessagePopup = nullptr;
};
