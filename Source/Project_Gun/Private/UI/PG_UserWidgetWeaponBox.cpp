// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PG_UserWidgetWeaponBox.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Define/PG_TableData.h"
#include "Game/GameMode/PG_GameModeBase.h"
#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/Overlay.h>

void UPG_UserWidgetWeaponBox::NativeConstruct()
{
	Super::NativeConstruct();

	PG_CONNECT_WIDGET(UButton, Btn_Weapon);
	PG_CONNECT_WIDGET(UImage, Img_IconAmmo);
	PG_CONNECT_WIDGET(UTextBlock, Text_Ammo);
	PG_CONNECT_WIDGET(UOverlay, OverlayLightBox);

	m_pkBtn_Weapon->OnClicked.AddDynamic(this, &UPG_UserWidgetWeaponBox::OnBtnWeaponClicked);
}

void UPG_UserWidgetWeaponBox::SetWeaponIndex(class APG_GameModeBase* pGameMode, int32 nWeaponIndex)
{
	ABCHECK(nullptr != pGameMode);

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	m_pGameMode = pGameMode;
	WeaponIndex = nWeaponIndex;

	const FPGWeaponTableData* pWeaponShopData = pGameInstance->GetWeaponTableData(nWeaponIndex);
	if (nullptr == pWeaponShopData)
	{
		ABLOG(Error, TEXT("GetWeaponTableData is Null (WeaponIndex : %d)"), nWeaponIndex);
		SetEnable(false);
		return;
	}

	if (pWeaponShopData->ServiceOpen && pGameMode->IsUnlockWeapon(nWeaponIndex))
	{
		SetEnable(true);
		m_pkBtn_Weapon->WidgetStyle.SetNormal(pWeaponShopData->WeaponImage);
		m_pkBtn_Weapon->WidgetStyle.SetHovered(pWeaponShopData->WeaponImage);
		m_pkBtn_Weapon->WidgetStyle.SetPressed(pWeaponShopData->WeaponImage);

		SetAmmo(pGameMode->GetMaxAmmo(nWeaponIndex));
	}
	else
	{
		SetEnable(false);
	}
}

void UPG_UserWidgetWeaponBox::SetSlotIndex(int32 nSlotIndex)
{
	SlotIndex = nSlotIndex;
}

void UPG_UserWidgetWeaponBox::SetAmmo(int32 nAmmo)
{
	// ¹«ÇÑ´ë
	if (nAmmo == PG_INFINITY_AMMO)
		m_pkText_Ammo->SetText(FText::FromString(FString::Printf(TEXT(" X ---"))));
	else
		m_pkText_Ammo->SetText(FText::FromString(FString::Printf(TEXT(" X %d"), nAmmo)));
}

void UPG_UserWidgetWeaponBox::SetEnable(bool bEnable)
{
	if (bEnable)
	{
		m_pkBtn_Weapon->SetVisibility(ESlateVisibility::Visible);
		m_pkBtn_Weapon->SetIsEnabled(true);

		m_pkImg_IconAmmo->SetVisibility(ESlateVisibility::HitTestInvisible);
		m_pkText_Ammo->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		m_pkBtn_Weapon->SetVisibility(ESlateVisibility::HitTestInvisible);
		m_pkBtn_Weapon->SetIsEnabled(false);

		m_pkImg_IconAmmo->SetVisibility(ESlateVisibility::Hidden);
		m_pkText_Ammo->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPG_UserWidgetWeaponBox::SetEnableLightBox(bool bEnable)
{
	if (bEnable)
		m_pkOverlayLightBox->SetVisibility(ESlateVisibility::Visible);
	else
		m_pkOverlayLightBox->SetVisibility(ESlateVisibility::Hidden);
}

void UPG_UserWidgetWeaponBox::OnBtnWeaponClicked()
{
	OnBtnWeaponClicked_WeaponIndex.Broadcast(WeaponIndex);
	OnBtnWeaponClicked_SlotIndex.Broadcast(SlotIndex);
}