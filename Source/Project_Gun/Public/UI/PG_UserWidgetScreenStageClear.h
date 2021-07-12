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
	// 스테이지 플레이 광고 재생 가능한지 검사
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
	// 광고 시청에 대한 결과
	//============================================================================================
	UFUNCTION(BlueprintCallable)
	void OnADRewardVideoSucessed();		// 유저가 보상형 광고를 성공적으로 보았음 (3배 보상)
	UFUNCTION(BlueprintCallable)
	void OnADRewardVideoError();		// 유저가 보상형 광고 보는중 닫거나 실패하였음	(3배 보상)
	UFUNCTION(BlueprintCallable)
	void OnADPlayStageVideoSucessed();	// 스테이지 플레이형 광고를 성공적으로 보았음
	UFUNCTION(BlueprintCallable)
	void OnADPlayStageVideoClosed();	// 스테이지 플레이형 광고를 닫거나 실패하였음
	//============================================================================================
	UFUNCTION()
	void OnViewResultAD(bool bEnable);

	// 메시지 팝업박스
	UFUNCTION()
	void OnMessagePopupBoxClose();

protected:
	PG_WIDGET_DECLAR(UTextBlock, TextTitle);

	//===================================================
	// Canvas MissionState
	//===================================================
	// Flag
	PG_WIDGET_DECLAR(UButton, BtnFlag);
	PG_WIDGET_DECLAR(UImage, ImgIconFlagPrice);
	PG_WIDGET_DECLAR(UTextBlock, TextFlagPoint);
	// Lethal
	PG_WIDGET_DECLAR(UButton, BtnLethal);
	PG_WIDGET_DECLAR(UImage, ImgIconLethalPrice);
	PG_WIDGET_DECLAR(UTextBlock, TextLethalPoint);
	// Time
	PG_WIDGET_DECLAR(UButton, BtnTime);
	PG_WIDGET_DECLAR(UImage, ImgIconTimePrice);
	PG_WIDGET_DECLAR(UTextBlock, TextTimePoint);
	
	//===================================================
	// CanvasClearPointList
	//===================================================
	PG_WIDGET_DECLAR(UTextBlock, TextStageClearPoint);		// 기본 스테이지 클리어 포인트 보상
	PG_WIDGET_DECLAR(UTextBlock, TextMonsterKillPoint);		// 몬스터 킬 획득 포인트
	PG_WIDGET_DECLAR(UTextBlock, TextMissionPoint);			// 미션 성공 포인트

	//===================================================
	// CanvasViewAD
	//===================================================
	PG_WIDGET_DECLAR(UButton, BtnADView);
	PG_WIDGET_DECLAR(UTextBlock, TextClearADTimer);

	//===================================================
	// CanvasTotalPoint
	//===================================================
	PG_WIDGET_DECLAR(UTextBlock, TextTotalPoint);

	//===================================================
	// CanvasBottomMenu
	//===================================================
	PG_WIDGET_DECLAR(UButton, BtnGoHome);
	PG_WIDGET_DECLAR(UButton, BtnRetry);
	PG_WIDGET_DECLAR(UButton, BtnNext);


	bool m_bWasViewStageClear3X = false;

	UPROPERTY(EditAnywhere, Category = "PG Sound")
	class USoundBase* PopupPlaySound = nullptr;

	// 메시지 팝업
	UPROPERTY()
	class UPG_UserWidgetMessagePopup* m_pUserWidgetMessagePopup = nullptr;
};
