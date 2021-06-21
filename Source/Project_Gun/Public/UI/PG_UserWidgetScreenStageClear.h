// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Blueprint/UserWidget.h"
#include "PG_UserWidgetScreenStageClear.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetScreenStageClear : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

public :
	// �������� �÷��� ���� ��� �������� �˻�
	UFUNCTION(BlueprintCallable)
	bool IsAbleViewStagePlayAD();

	UFUNCTION(BlueprintCallable)
	void UpdateClearStageLayout();

	UFUNCTION()
	void ShowMessageBox(FString kTitle, FString kDescription);

	void SetBonusFlag(bool bWasClear, int32 nBonus);
	void SetBonusLethal(bool bWasClear, int32 nBonus);
	void SetBonusTime(bool bWasClear, int32 nBonus);
	void SetBonusStageClear(int32 nStageClearBonus);
	void SetBonusMonsterKill(int32 nMonsterKillBonus);
	void SetBonusMissionPoint(int32 nStarBonus);
	void SetBtnBonusADTime(int32 nTime);
	void SetBtnEnableAD(bool bEnable);
	void SetTotalReward(int32 nTotalReward);

protected:
	UFUNCTION(BlueprintCallable)
	void OnBtnADview();
	UFUNCTION(BlueprintCallable)
	void OnBtnGoHome();
	UFUNCTION(BlueprintCallable)
	void OnBtnRetry();
	UFUNCTION(BlueprintCallable)
	void OnBtnNext();

	//============================================================================================
	// ���� ��û�� ���� ���
	//============================================================================================
	UFUNCTION(BlueprintCallable)
	void OnADRewardVideoSucessed();		// ������ ������ ���� ���������� ������ (3�� ����)
	UFUNCTION(BlueprintCallable)
	void OnADRewardVideoError();		// ������ ������ ���� ������ �ݰų� �����Ͽ���	(3�� ����)
	UFUNCTION(BlueprintCallable)
	void OnADPlayStageVideoSucessed();	// �������� �÷����� ���� ���������� ������
	UFUNCTION(BlueprintCallable)
	void OnADPlayStageVideoClosed();	// �������� �÷����� ���� �ݰų� �����Ͽ���
	//============================================================================================
	UFUNCTION()
	void OnViewResultAD(bool bEnable);

	// �޽��� �˾��ڽ�
	UFUNCTION()
	void OnMessagePopupBoxClose();

protected:
	PG_DECLAR_WIDGET(UTextBlock, TextTitle);

	//===================================================
	// Canvas MissionState
	//===================================================
	// Flag
	PG_DECLAR_WIDGET(UButton, BtnFlag);
	PG_DECLAR_WIDGET(UImage, ImgIconFlagPrice);
	PG_DECLAR_WIDGET(UTextBlock, TextFlagPoint);
	// Lethal
	PG_DECLAR_WIDGET(UButton, BtnLethal);
	PG_DECLAR_WIDGET(UImage, ImgIconLethalPrice);
	PG_DECLAR_WIDGET(UTextBlock, TextLethalPoint);
	// Time
	PG_DECLAR_WIDGET(UButton, BtnTime);
	PG_DECLAR_WIDGET(UImage, ImgIconTimePrice);
	PG_DECLAR_WIDGET(UTextBlock, TextTimePoint);
	
	//===================================================
	// CanvasClearPointList
	//===================================================
	PG_DECLAR_WIDGET(UTextBlock, TextStageClearPoint);		// �⺻ �������� Ŭ���� ����Ʈ ����
	PG_DECLAR_WIDGET(UTextBlock, TextMonsterKillPoint);		// ���� ų ȹ�� ����Ʈ
	PG_DECLAR_WIDGET(UTextBlock, TextMissionPoint);			// �̼� ���� ����Ʈ

	//===================================================
	// CanvasViewAD
	//===================================================
	PG_DECLAR_WIDGET(UButton, BtnADView);
	PG_DECLAR_WIDGET(UTextBlock, TextClearADTimer);

	//===================================================
	// CanvasTotalPoint
	//===================================================
	PG_DECLAR_WIDGET(UTextBlock, TextTotalPoint);

	//===================================================
	// CanvasBottomMenu
	//===================================================
	PG_DECLAR_WIDGET(UButton, BtnGoHome);
	PG_DECLAR_WIDGET(UButton, BtnRetry);
	PG_DECLAR_WIDGET(UButton, BtnNext);


	bool m_bWasViewStageClear3X = false;

	UPROPERTY(EditAnywhere, Category = "PG Sound")
	class USoundBase* PopupPlaySound = nullptr;

	// �޽��� �˾�
	UPROPERTY()
	class UPG_UserWidgetMessagePopup* m_pUserWidgetMessagePopup = nullptr;
};
