// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PG_UserWidgetMessagePopup.h"

#include <Components/Button.h>
#include <Components/TextBlock.h>

void UPG_UserWidgetMessagePopup::NativeConstruct()
{
	Super::NativeConstruct();

	ABCHECK(nullptr != Text_TitleName);
	ABCHECK(nullptr != Text_Description);
	ABCHECK(nullptr != Btn_OK);

	Btn_OK->OnClicked.AddDynamic(this, &UPG_UserWidgetMessagePopup::OnBtnOK);
}

void UPG_UserWidgetMessagePopup::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if (ESlateVisibility::Hidden != InVisibility)
	{
		if (PopupPlaySound)
			PlaySound(PopupPlaySound);
	}
}

void UPG_UserWidgetMessagePopup::SetTitle(FString kTitle)
{
	ABCHECK(nullptr != Text_TitleName);
	Text_TitleName->SetText(FText::FromString(kTitle));
}

void UPG_UserWidgetMessagePopup::SetDescription(FString kDescription)
{
	ABCHECK(nullptr != Text_Description);
	Text_Description->SetText(FText::FromString(kDescription));
}

void UPG_UserWidgetMessagePopup::SetValue(int32 nValue)
{
	m_nValue = nValue;
}

void UPG_UserWidgetMessagePopup::OnBtnOK()
{
	OnBtnClickedOK.Broadcast();
	OnBtnClickedOK_OneValue.Broadcast(m_nValue);
}
