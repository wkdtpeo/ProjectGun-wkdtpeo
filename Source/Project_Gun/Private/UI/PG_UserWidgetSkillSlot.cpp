// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PG_UserWidgetSkillSlot.h"

#include <Components/Overlay.h>

void UPG_UserWidgetSkillSlot::NativeConstruct()
{
	Super::NativeConstruct();

	PG_CONNECT_WIDGET(UButton, BtnSkill);
	PG_CONNECT_WIDGET(UOverlay, OverlayLightBox);
}

void UPG_UserWidgetSkillSlot::SetEnable(bool bEnable)
{
	ABCHECK(nullptr != m_pkBtnSkill);
	ABCHECK(nullptr != m_pkOverlayLightBox);

	m_pkBtnSkill->SetIsEnabled(bEnable);

	if (bEnable)
		m_pkOverlayLightBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	else
		m_pkOverlayLightBox->SetVisibility(ESlateVisibility::Hidden);
}