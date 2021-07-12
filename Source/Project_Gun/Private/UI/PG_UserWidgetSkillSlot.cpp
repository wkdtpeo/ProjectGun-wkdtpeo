// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PG_UserWidgetSkillSlot.h"

#include <Components/Overlay.h>
#include <Components/Button.h>

void UPG_UserWidgetSkillSlot::NativeConstruct()
{
	Super::NativeConstruct();

	PG_WIDGET_LINK(UButton, BtnSkill);
	PG_WIDGET_LINK(UOverlay, OverlayLightBox);
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