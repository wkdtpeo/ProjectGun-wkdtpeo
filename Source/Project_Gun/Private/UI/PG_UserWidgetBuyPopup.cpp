// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PG_UserWidgetBuyPopup.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>

void UPG_UserWidgetBuyPopup::NativeConstruct()
{
	Super::NativeConstruct();

	ABCHECK(nullptr != Text_TitleName);
	ABCHECK(nullptr != Text_Description);
	ABCHECK(nullptr != Text_Price);
	ABCHECK(nullptr != Btn_OK);
	ABCHECK(nullptr != Btn_Cancel);

	Btn_OK->OnClicked.AddDynamic(this, &UPG_UserWidgetBuyPopup::OnBtnOK);
	Btn_Cancel->OnClicked.AddDynamic(this, &UPG_UserWidgetBuyPopup::OnBtnCancel);
}

void UPG_UserWidgetBuyPopup::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if (ESlateVisibility::Hidden != InVisibility)
	{
		if (PopupPlaySound)
			PlaySound(PopupPlaySound);
	}
}

void UPG_UserWidgetBuyPopup::SetTitle(FString kTitle)
{
	ABCHECK(nullptr != Text_TitleName);
	Text_TitleName->SetText(FText::FromString(kTitle));
}

void UPG_UserWidgetBuyPopup::SetPrice(int32 nPrice)
{
	ABCHECK(nullptr != Text_Price);
	Text_Price->SetText(FText::FromString(FString::Printf(TEXT("%d"), nPrice)));
}

void UPG_UserWidgetBuyPopup::SetDescription(FString kDescription)
{
	ABCHECK(nullptr != Text_Description);
	Text_Description->SetText(FText::FromString(kDescription));
}

void UPG_UserWidgetBuyPopup::SetValue(int32 nValue)
{
	m_nValue = nValue;
}

void UPG_UserWidgetBuyPopup::OnBtnOK()
{
	OnBtnClickedOK.Broadcast(m_nValue);
}

void UPG_UserWidgetBuyPopup::OnBtnCancel()
{
	OnOnBtnClickedCancel.Broadcast(m_nValue);
}