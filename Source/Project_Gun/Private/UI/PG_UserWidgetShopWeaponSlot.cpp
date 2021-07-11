// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PG_UserWidgetShopWeaponSlot.h"
#include "UI/PG_UserWidgetBuyPopup.h"
#include "UI/PG_UserWidgetMessagePopup.h"

#include "Game/GameInstance/PG_GameInstance.h"
#include "Game/GameMode/PG_GameModeBase.h"

#include "Player/PlayerState/PG_MyPlayerState.h"

#include "SaveData/PG_SavePlayerData.h"

#include "Define/PG_TableData.h"

#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/CanvasPanel.h>

void UPG_UserWidgetShopWeaponSlot::NativeConstruct()
{
	Super::NativeConstruct();

	// Weapon
	PG_CONNECT_WIDGET(UButton, BtnWeapon);
	PG_CONNECT_WIDGET(UImage, ImgWeaponLock);
	PG_CONNECT_WIDGET(UTextBlock, TextWeaponPrice);
	PG_CONNECT_WIDGET(UCanvasPanel, CanvasWeaponPrice);

	// Power
	PG_CONNECT_WIDGET(UButton, BtnPower);
	PG_CONNECT_WIDGET(UTextBlock, TextPowerLevel);
	PG_CONNECT_WIDGET(UTextBlock, TextPowerPrice);
	PG_CONNECT_WIDGET(UCanvasPanel, CanvasPowerPrice);

	// Ammo
	PG_CONNECT_WIDGET(UButton, BtnAmmo);
	PG_CONNECT_WIDGET(UTextBlock, TextAmmoLevel);
	PG_CONNECT_WIDGET(UTextBlock, TextAmmoPrice);
	PG_CONNECT_WIDGET(UCanvasPanel, CanvasAmmoPrice);


	{
		m_pUserWidgetBuyPopup = CreateWidget<UPG_UserWidgetBuyPopup>(GetWorld(), LoadClass<UPG_UserWidgetBuyPopup>(NULL, TEXT("WidgetBlueprint'/Game/PG/Blueprints/WidgetBP/WB_BuyPopup.WB_BuyPopup_C'")));
		ABCHECK(nullptr != m_pUserWidgetBuyPopup);

		m_pUserWidgetBuyPopup->OnBtnClickedOK.AddUObject(this, &UPG_UserWidgetShopWeaponSlot::OnBuyOK);
		m_pUserWidgetBuyPopup->OnOnBtnClickedCancel.AddUObject(this, &UPG_UserWidgetShopWeaponSlot::OnBuyCancel);
		m_pUserWidgetBuyPopup->SetVisibility(ESlateVisibility::Hidden);
		m_pUserWidgetBuyPopup->AddToViewport(999);
	}

	{
		m_pUserWidgetMessagePopup = CreateWidget<UPG_UserWidgetMessagePopup>(GetWorld(), LoadClass<UPG_UserWidgetMessagePopup>(NULL, TEXT("WidgetBlueprint'/Game/PG/Blueprints/WidgetBP/WB_MessagePopup.WB_MessagePopup_C'")));
		ABCHECK(nullptr != m_pUserWidgetMessagePopup);

		m_pUserWidgetMessagePopup->OnBtnClickedOK.AddUObject(this, &UPG_UserWidgetShopWeaponSlot::OnMessagePopupBoxClose);
		m_pUserWidgetMessagePopup->SetVisibility(ESlateVisibility::Hidden);
		m_pUserWidgetMessagePopup->AddToViewport(999);
	}
}

void UPG_UserWidgetShopWeaponSlot::UpdateLayout()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	ABCHECK(nullptr != m_pGameMode);

	auto pPlayerSaveData = m_pGameMode->GetSavePlayerData();
	ABCHECK(nullptr != pPlayerSaveData);

	const FPGWeaponTableData* pWeaponShopData = pGameInstance->GetWeaponTableData(m_nWeaponIndex);
	if (nullptr == pWeaponShopData)
	{
		ABLOG(Error, TEXT("GetWeaponTableData is Null (WeaponIndex : %d)"), m_nWeaponIndex);
		SetUnlockLayout(false);
		return;
	}

	// 무기 판매 가능한지 검사
	if (false == pWeaponShopData->ServiceOpen)
	{
		SetUnlockLayout(false);
		return;
	}

	// 무기 이미지 설정
	m_pkBtnWeapon->WidgetStyle.SetNormal(pWeaponShopData->WeaponImage);
	m_pkBtnWeapon->WidgetStyle.SetHovered(pWeaponShopData->WeaponImage);
	m_pkBtnWeapon->WidgetStyle.SetPressed(pWeaponShopData->WeaponImage);
	m_pkBtnWeapon->WidgetStyle.SetDisabled(pWeaponShopData->WeaponImage);

	m_bIsMaxPower = m_pGameMode->IsMaxLevelPower(m_nWeaponIndex);
	m_bIsMaxAmmo = m_pGameMode->IsMaxLevelAmmo(m_nWeaponIndex);
	m_bIsAbleBuyWepaon = !m_pGameMode->IsUnlockWeapon(m_nWeaponIndex);
	m_bIsAbleBuyPower = m_pGameMode->IsAbleBuyRewardPointPower(m_nWeaponIndex);
	m_bIsAbleBuyAmmo = m_pGameMode->IsAbleBuyRewardPointAmmo(m_nWeaponIndex);

	// 무기 데이터 설정
	auto pWeaponOwnData = pPlayerSaveData->WeaponOwnData.Find(m_nWeaponIndex);
	if (pWeaponOwnData)
	{
		SetWepaon(pWeaponShopData->CostUnlock);
		SetPowerLevel(pWeaponOwnData->PowerLevel, pWeaponShopData->CostUpgradePower);
		SetAmmoLevel(pWeaponOwnData->MagLevel, pWeaponShopData->CostUpgradeAmmo);
		SetUnlockLayout(pWeaponOwnData->IsUnlock);
	}
	else
	{
		SetWepaon(pWeaponShopData->CostUnlock);
		SetPowerLevel(0, pWeaponShopData->CostUpgradePower);
		SetAmmoLevel(0, pWeaponShopData->CostUpgradeAmmo);
		SetUnlockLayout(false);
	}
}

void UPG_UserWidgetShopWeaponSlot::SetUnlockLayout(bool bUnlock)
{
	if (bUnlock)
	{
		// Weapon
		{
			m_pkBtnWeapon->SetVisibility(ESlateVisibility::HitTestInvisible);
			m_pkBtnWeapon->SetIsEnabled(false);
			m_pkImgWeaponLock->SetVisibility(ESlateVisibility::Hidden);
			m_pkCanvasWeaponPrice->SetVisibility(ESlateVisibility::Hidden);
		}

		// Power
		{
			m_pkBtnPower->SetIsEnabled(true);
			if (m_bIsMaxPower)
			{
				m_pkBtnPower->SetVisibility(ESlateVisibility::HitTestInvisible);
				m_pkCanvasPowerPrice->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				m_pkBtnPower->SetVisibility(ESlateVisibility::Visible);
				m_pkCanvasPowerPrice->SetVisibility(ESlateVisibility::HitTestInvisible);
			}

			m_pkTextPowerLevel->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		
		// Ammo
		{
			m_pkBtnAmmo->SetIsEnabled(true);
			if (m_bIsMaxAmmo)
			{
				m_pkBtnAmmo->SetVisibility(ESlateVisibility::HitTestInvisible);
				m_pkCanvasAmmoPrice->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				m_pkBtnAmmo->SetVisibility(ESlateVisibility::Visible);
				m_pkCanvasAmmoPrice->SetVisibility(ESlateVisibility::HitTestInvisible);
			}

			m_pkTextAmmoLevel->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
	else
	{
		// Weapon
		m_pkBtnWeapon->SetVisibility(ESlateVisibility::Visible);
		m_pkBtnWeapon->SetIsEnabled(m_bIsAbleBuyWepaon);
		m_pkImgWeaponLock->SetVisibility(ESlateVisibility::HitTestInvisible);
		m_pkCanvasWeaponPrice->SetVisibility(ESlateVisibility::HitTestInvisible);

		// Power
		m_pkBtnPower->SetVisibility(ESlateVisibility::HitTestInvisible);
		m_pkBtnPower->SetIsEnabled(false);
		m_pkTextPowerLevel->SetVisibility(ESlateVisibility::Hidden);

		// Ammo
		m_pkBtnAmmo->SetVisibility(ESlateVisibility::HitTestInvisible);
		m_pkBtnAmmo->SetIsEnabled(false);
		m_pkTextAmmoLevel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPG_UserWidgetShopWeaponSlot::SetWeaponIndex(class APG_GameModeBase* pGameMode, int32 nWeaponIndex, int32 nSlotIndex)
{
	ABCHECK(nullptr != m_pkBtnWeapon);
	ABCHECK(nullptr != pGameMode);

	m_pGameMode = pGameMode;
	m_nWeaponIndex = nWeaponIndex;
	m_nSlotIndex = nSlotIndex;
}

void UPG_UserWidgetShopWeaponSlot::SetWepaon(int32 nPrice)
{
	m_pkTextWeaponPrice->SetText(FText::FromString(*FString::Printf(TEXT("%d"), nPrice)));
}

void UPG_UserWidgetShopWeaponSlot::SetPowerLevel(int32 nPowerLevel, int32 nPrice)
{
	if (m_bIsMaxPower)
		m_pkTextPowerLevel->SetText(FText::FromString(TEXT("MAX")));
	else
		m_pkTextPowerLevel->SetText(FText::FromString(*FString::Printf(TEXT("LV %d"), nPowerLevel)));

	m_pkTextPowerPrice->SetText(FText::FromString(*FString::Printf(TEXT("%d"), nPrice)));
}

void UPG_UserWidgetShopWeaponSlot::SetAmmoLevel(int32 nAmmoLevel, int32 nPrice)
{
	if (m_bIsMaxAmmo)
		m_pkTextAmmoLevel->SetText(FText::FromString(TEXT("MAX")));
	else
		m_pkTextAmmoLevel->SetText(FText::FromString(*FString::Printf(TEXT("LV %d"), nAmmoLevel)));

	m_pkTextAmmoPrice->SetText(FText::FromString(*FString::Printf(TEXT("%d"), nPrice)));
}


void UPG_UserWidgetShopWeaponSlot::ShowBuyPopupBox(enum EBuyType ekBuyType, FString kTitle, FString kDescription, int32 nPrice)
{
	ABCHECK(nullptr != m_pUserWidgetBuyPopup);
	m_pUserWidgetBuyPopup->SetValue((int32)ekBuyType);
	m_pUserWidgetBuyPopup->SetTitle(kTitle);
	m_pUserWidgetBuyPopup->SetPrice(nPrice);
	m_pUserWidgetBuyPopup->SetDescription(kDescription);
	m_pUserWidgetBuyPopup->SetVisibility(ESlateVisibility::Visible);
}

void UPG_UserWidgetShopWeaponSlot::ShowMessageBox(FString kTitle, FString kDescription, int32 nValue)
{
	ABCHECK(nullptr != m_pUserWidgetMessagePopup);
	m_pUserWidgetMessagePopup->SetValue(nValue);
	m_pUserWidgetMessagePopup->SetTitle(kTitle);
	m_pUserWidgetMessagePopup->SetDescription(kDescription);
	m_pUserWidgetMessagePopup->SetVisibility(ESlateVisibility::Visible);
}

void UPG_UserWidgetShopWeaponSlot::OnBtnWeapon()
{
	if (false == m_pGameMode.IsValid(true))
	{
		ABLOG(Error, TEXT("m_pGameMode is nullptr"));
		ShowMessageBox(TEXT("Error"), TEXT("Purchase failed."), PG_INVALID_INDEX);
		return;
	}

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	const FPGWeaponTableData* pWeaponShopData = pGameInstance->GetWeaponTableData(m_nWeaponIndex);
	if (nullptr == pWeaponShopData)
	{
		ABLOG(Error, TEXT("GetWeaponTableData is Null (WeaponIndex : %d)"), m_nWeaponIndex);
		//SetEnable(false);
		ShowMessageBox(TEXT("Error"), TEXT("Purchase failed."), PG_INVALID_INDEX);
		return;
	}

	ShowBuyPopupBox(EBuyType::eBT_Weapon, pWeaponShopData->WeaponData.Name, TEXT("Do you want to buy?"), pWeaponShopData->CostUnlock);
}

void UPG_UserWidgetShopWeaponSlot::OnBtnPower()
{
	if (false == m_pGameMode.IsValid(true))
	{
		ABLOG(Error, TEXT("m_pGameMode is nullptr"));
		ShowMessageBox(TEXT("Error"), TEXT("Purchase failed."), PG_INVALID_INDEX);
		return;
	}

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	const FPGWeaponTableData* pWeaponShopData = pGameInstance->GetWeaponTableData(m_nWeaponIndex);
	if (nullptr == pWeaponShopData)
	{
		ABLOG(Error, TEXT("GetWeaponTableData is Null (WeaponIndex : %d)"), m_nWeaponIndex);
		//SetEnable(false);
		ShowMessageBox(TEXT("Error"), TEXT("Purchase failed."), PG_INVALID_INDEX);
		return;
	}

	ShowBuyPopupBox(EBuyType::eBT_Power, FString::Printf(TEXT("%s Power"), *pWeaponShopData->WeaponData.Name), TEXT("Do you want to buy?"), pWeaponShopData->CostUpgradePower);
}

void UPG_UserWidgetShopWeaponSlot::OnBtnAmmo()
{
	ABCHECK(nullptr != m_pUserWidgetBuyPopup);

	if (false == m_pGameMode.IsValid(true))
	{
		ABLOG(Error, TEXT("m_pGameMode is nullptr"));
		ShowMessageBox(TEXT("Error"), TEXT("Purchase failed."), PG_INVALID_INDEX);
		return;
	}

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	const FPGWeaponTableData* pWeaponShopData = pGameInstance->GetWeaponTableData(m_nWeaponIndex);
	if (nullptr == pWeaponShopData)
	{
		ABLOG(Error, TEXT("GetWeaponTableData is Null (WeaponIndex : %d)"), m_nWeaponIndex);
		//SetEnable(false);
		ShowMessageBox(TEXT("Error"), TEXT("Purchase failed."), PG_INVALID_INDEX);
		return;
	}

	ShowBuyPopupBox(EBuyType::eBT_Ammo, FString::Printf(TEXT("%s Ammo"), *pWeaponShopData->WeaponData.Name), TEXT("Do you want to buy?"), pWeaponShopData->CostUpgradeAmmo);
}

void UPG_UserWidgetShopWeaponSlot::OnBuyOK(int32 nValue)
{
	ABCHECK(nullptr != m_pUserWidgetBuyPopup);
	m_pUserWidgetBuyPopup->SetVisibility(ESlateVisibility::Hidden);

	if (false == m_pGameMode.IsValid(true))
	{
		ABLOG(Error, TEXT("m_pGameMode is nullptr"));
		ShowMessageBox(TEXT("Error"), TEXT("Purchase failed."), PG_INVALID_INDEX);
		return;
	}

	auto pMyPlayerState = Cast<APG_MyPlayerState>(GetOwningPlayerState());
	ABCHECK(nullptr != pMyPlayerState);

	// 구매 진행
	bool bBuySuccesed = false;
	switch ((EBuyType)nValue)
	{
	case EBuyType::eBT_Weapon:		bBuySuccesed = m_pGameMode->BuyWeapon(m_nWeaponIndex);		break;
	case EBuyType::eBT_Power:		bBuySuccesed = m_pGameMode->BuyPowerUp(m_nWeaponIndex);		break;
	case EBuyType::eBT_Ammo:		bBuySuccesed = m_pGameMode->BuyAmmoUp(m_nWeaponIndex);		break;
	default:		break;
	}

	// 구매 실패
	if (false == bBuySuccesed)
	{
		auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
		ABCHECK(nullptr != pGameInstance);

		const FPGWeaponTableData* pWeaponShopData = pGameInstance->GetWeaponTableData(m_nWeaponIndex);
		if (nullptr == pWeaponShopData)
		{
			ABLOG(Error, TEXT("GetWeaponTableData is Null (WeaponIndex : %d)"), m_nWeaponIndex);
			//SetEnable(false);
			return;
		}

		ShowMessageBox(pWeaponShopData->WeaponData.Name, m_pGameMode->GetLastBuyErrorMsg(), nValue);
	}

	OnItemBuyWeapon.Broadcast();

	UpdateLayout();
}

void UPG_UserWidgetShopWeaponSlot::OnBuyCancel(int32 nValue)
{
	ABCHECK(nullptr != m_pUserWidgetBuyPopup);
	m_pUserWidgetBuyPopup->SetVisibility(ESlateVisibility::Hidden);

	UpdateLayout();
}

void UPG_UserWidgetShopWeaponSlot::OnMessagePopupBoxClose()
{
	ABCHECK(nullptr != m_pUserWidgetMessagePopup);
	m_pUserWidgetMessagePopup->SetVisibility(ESlateVisibility::Hidden);

	UpdateLayout();
}