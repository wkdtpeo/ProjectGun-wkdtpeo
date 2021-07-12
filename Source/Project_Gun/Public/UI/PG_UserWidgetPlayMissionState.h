// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Blueprint/UserWidget.h"

#include "PG_UserWidgetPlayMissionState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetPlayMissionState : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public :
	void UpdateMissionFlag();
	void UpdateMissionLethal();
	void UpdateMissionTime();

	void SetDataFlag(bool bClear, int32 nCurrValue, int32 nMaxValue);
	void SetDataLethal(bool bClear, int32 nCurrValue, int32 nMaxValue);
	void SetDataTime(bool bClear, int32 nCurrTime);

protected:
	// 현재 몬스터 킬수 업데이트
	UFUNCTION(BlueprintCallable)
	void OnUpdateFlag();
	// 몬스터 필살기 킬수 업데이트
	UFUNCTION(BlueprintCallable)
	void OnUpdateLethal();
	// 타이머 업데이트
	UFUNCTION(BlueprintCallable)
	void OnUpdateTime();

protected:
	PG_WIDGET_DECLAR(UButton, BtnFlag);
	PG_WIDGET_DECLAR(UButton, BtnLethal);
	PG_WIDGET_DECLAR(UButton, BtnTime);

	PG_WIDGET_DECLAR(UTextBlock, TextCurrFlag);
	PG_WIDGET_DECLAR(UTextBlock, TextCurrLethal);
	PG_WIDGET_DECLAR(UTextBlock, TextCurrTime);

	PG_WIDGET_DECLAR(UTextBlock, TextMaxFlag);
	PG_WIDGET_DECLAR(UTextBlock, TextMaxLethal);
};
