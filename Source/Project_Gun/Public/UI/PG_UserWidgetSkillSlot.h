// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Blueprint/UserWidget.h"
#include "PG_UserWidgetSkillSlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetSkillSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public :
	void SetEnable(bool bEnalbe);

protected :
	PG_DECLAR_WIDGET(UButton, BtnSkill);
	PG_DECLAR_WIDGET(UOverlay, OverlayLightBox);
};
