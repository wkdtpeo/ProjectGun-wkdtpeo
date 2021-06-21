// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Blueprint/UserWidget.h"
#include "PG_UserWidgetScreenStageFailed.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetScreenStageFailed : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

public :
	void SetPopupTitle(FString kTitle);

	UFUNCTION(BlueprintCallable)
	bool IsAbleViewStagePlayAD();

	UFUNCTION(BlueprintCallable)
	void UpdateLayout();

protected:
	// Popup Title
	PG_DECLAR_WIDGET(UTextBlock, TextTitle);

	// Mission State
	PG_DECLAR_WIDGET(UButton, BtnFlag);
	PG_DECLAR_WIDGET(UButton, BtnLethal);
	PG_DECLAR_WIDGET(UButton, BtnTime);

	// Menu Button
	PG_DECLAR_WIDGET(UButton, BtnHome);
	PG_DECLAR_WIDGET(UButton, BtnRetry);

	UPROPERTY(EditAnywhere, Category = "PG Sound")
	class USoundBase* PopupPlaySound = nullptr;

protected:
	UFUNCTION(BlueprintCallable)
	void OnBtnHome();
	UFUNCTION(BlueprintCallable)
	void OnBtnRetry();

	//============================================================================================
	// 광고 시청에 대한 결과
	//============================================================================================
	UFUNCTION(BlueprintCallable)
	void OnADPlayStageVideoSucessed();	// 스테이지 플레이형 광고를 성공적으로 보았음
	UFUNCTION(BlueprintCallable)
	void OnADPlayStageVideoClosed();	// 스테이지 플레이형 광고를 닫거나 실패하였음
	//============================================================================================
};
