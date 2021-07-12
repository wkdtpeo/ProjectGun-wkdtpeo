// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PG_UserWidgetStageClearTarget.h"

#include <Components/Overlay.h>
#include <Components/TextBlock.h>

void UPG_UserWidgetStageClearTarget::NativeConstruct()
{
	Super::NativeConstruct();

	ABCHECK(nullptr != Flag_Clear);
	ABCHECK(nullptr != Flag_InProgress);
	ABCHECK(nullptr != Text_CurrFlag);
	ABCHECK(nullptr != Text_MaxFlag);
	ABCHECK(nullptr != Lethal_Clear);
	ABCHECK(nullptr != Text_CurrLethal);
	ABCHECK(nullptr != Text_MaxLethal);
	ABCHECK(nullptr != Time_Clear);
	ABCHECK(nullptr != Text_CurrTime);

}

void UPG_UserWidgetStageClearTarget::SetDataFlag(bool bClear, int32 nCurrValue, int32 nMaxValue)
{
	if (bClear)
	{
		Text_CurrFlag->SetColorAndOpacity(FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f)));
		Flag_Clear->SetVisibility(ESlateVisibility::HitTestInvisible);
		Flag_InProgress->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Text_CurrFlag->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f)));
		Flag_Clear->SetVisibility(ESlateVisibility::Hidden);
		Flag_InProgress->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	Text_CurrFlag->SetText(FText::FromString(FString::Printf(TEXT("%d"), nCurrValue)));
	Text_MaxFlag->SetText(FText::FromString(FString::Printf(TEXT("%d"), nMaxValue)));
}

void UPG_UserWidgetStageClearTarget::SetDataLethal(bool bClear, int32 nCurrValue, int32 nMaxValue)
{
	if (bClear)
	{
		Text_CurrLethal->SetColorAndOpacity(FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f)));
		Lethal_Clear->SetVisibility(ESlateVisibility::HitTestInvisible);
		Lethal_InProgress->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Text_CurrLethal->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f)));
		Lethal_Clear->SetVisibility(ESlateVisibility::Hidden);
		Lethal_InProgress->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	Text_CurrLethal->SetText(FText::FromString(FString::Printf(TEXT("%d"), nCurrValue)));
	Text_MaxLethal->SetText(FText::FromString(FString::Printf(TEXT("%d"), nMaxValue)));
}

void UPG_UserWidgetStageClearTarget::SetDataTime(bool bClear, int32 nCurrTime)
{
	if (bClear)
	{
		Text_CurrTime->SetColorAndOpacity(FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f)));
		Time_Clear->SetVisibility(ESlateVisibility::HitTestInvisible);
		Time_InProgress->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Text_CurrTime->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f)));
		Time_Clear->SetVisibility(ESlateVisibility::Hidden);
		Time_InProgress->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	Text_CurrTime->SetText(FText::FromString(FString::Printf(TEXT("%d"), nCurrTime)));
}