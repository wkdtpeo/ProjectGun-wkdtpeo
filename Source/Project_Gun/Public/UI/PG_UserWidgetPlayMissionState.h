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
	// ���� ���� ų�� ������Ʈ
	UFUNCTION(BlueprintCallable)
	void OnUpdateFlag();
	// ���� �ʻ�� ų�� ������Ʈ
	UFUNCTION(BlueprintCallable)
	void OnUpdateLethal();
	// Ÿ�̸� ������Ʈ
	UFUNCTION(BlueprintCallable)
	void OnUpdateTime();

protected:
	PG_DECLAR_WIDGET(UButton, BtnFlag);
	PG_DECLAR_WIDGET(UButton, BtnLethal);
	PG_DECLAR_WIDGET(UButton, BtnTime);

	PG_DECLAR_WIDGET(UTextBlock, TextCurrFlag);
	PG_DECLAR_WIDGET(UTextBlock, TextCurrLethal);
	PG_DECLAR_WIDGET(UTextBlock, TextCurrTime);

	PG_DECLAR_WIDGET(UTextBlock, TextMaxFlag);
	PG_DECLAR_WIDGET(UTextBlock, TextMaxLethal);
};
