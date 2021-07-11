// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PG_UserWidgetShopWeaponBox.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Player/PlayerState/PG_MyPlayerState.h"
#include "Define/PG_TableData.h"
#include "SaveData/PG_SavePlayerData.h"
#include "Game/GameMode/PG_GameModeBase.h"
#include "UI/PG_UserWidgetBuyPopup.h"
#include "UI/PG_UserWidgetMessagePopup.h"
#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>

void UPG_UserWidgetShopWeaponBox::NativeConstruct()
{
	Super::NativeConstruct();

	ABCHECK(nullptr != Btn_Weapon);
	ABCHECK(nullptr != Btn_Power);
	ABCHECK(nullptr != Btn_Ammo);
	ABCHECK(nullptr != Text_PowerLevel);
	ABCHECK(nullptr != Text_AmmoLevel);
	ABCHECK(nullptr != Img_WeaponLock);
	ABCHECK(nullptr != Img_PowerUp);
	ABCHECK(nullptr != Img_AmmoUp);

	Btn_Weapon->OnClicked.AddDynamic(this, &UPG_UserWidgetShopWeaponBox::OnRequestUnlockWeapon);
	Btn_Power->OnClicked.AddDynamic(this, &UPG_UserWidgetShopWeaponBox::OnRequestLevelupPower);
	Btn_Ammo->OnClicked.AddDynamic(this, &UPG_UserWidgetShopWeaponBox::OnRequestLevelupAmmo);

	{
		m_pUserWidgetBuyPopup = CreateWidget<UPG_UserWidgetBuyPopup>(GetWorld(), LoadClass<UPG_UserWidgetBuyPopup>(NULL, TEXT("WidgetBlueprint'/Game/PG/Blueprints/WidgetBP/WB_BuyPopup.WB_BuyPopup_C'")));
		ABCHECK(nullptr != m_pUserWidgetBuyPopup);

		m_pUserWidgetBuyPopup->OnBtnClickedOK.AddUObject(this, &UPG_UserWidgetShopWeaponBox::OnBuyOK);
		m_pUserWidgetBuyPopup->OnOnBtnClickedCancel.AddUObject(this, &UPG_UserWidgetShopWeaponBox::OnBuyCancel);
		m_pUserWidgetBuyPopup->SetVisibility(ESlateVisibility::Hidden);
		m_pUserWidgetBuyPopup->AddToViewport(999);
	}

	{
		m_pUserWidgetMessagePopup = CreateWidget<UPG_UserWidgetMessagePopup>(GetWorld(), LoadClass<UPG_UserWidgetMessagePopup>(NULL, TEXT("WidgetBlueprint'/Game/PG/Blueprints/WidgetBP/WB_MessagePopup.WB_MessagePopup_C'")));
		ABCHECK(nullptr != m_pUserWidgetBuyPopup);

		m_pUserWidgetMessagePopup->OnBtnClickedOK.AddUObject(this, &UPG_UserWidgetShopWeaponBox::OnMessagePopupBoxClose);
		m_pUserWidgetMessagePopup->SetVisibility(ESlateVisibility::Hidden);
		m_pUserWidgetMessagePopup->AddToViewport(999);
	}
}

void UPG_UserWidgetShopWeaponBox::SetWeaponIndex(class APG_GameModeBase* pGameMode, int32 nWeaponIndex)
{
	ABCHECK(nullptr != Btn_Weapon);
	ABCHECK(nullptr != pGameMode);

	auto pPlayerSaveData = pGameMode->GetSavePlayerData();
	ABCHECK(nullptr != pPlayerSaveData);

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

	// 무기 판매 가능한지 검사
	if (false == pWeaponShopData->ServiceOpen)
	{
		SetEnable(false);
		return;
	}

	// 무기 이미지 설정
	Btn_Weapon->WidgetStyle.SetNormal(pWeaponShopData->WeaponImage);
	Btn_Weapon->WidgetStyle.SetHovered(pWeaponShopData->WeaponImage);
	Btn_Weapon->WidgetStyle.SetPressed(pWeaponShopData->WeaponImage);
	Btn_Weapon->WidgetStyle.SetDisabled(pWeaponShopData->WeaponImage);

	IsMaxPower = pGameMode->IsMaxLevelPower(nWeaponIndex);
	IsMaxAmmo = pGameMode->IsMaxLevelAmmo(nWeaponIndex);
	IsAbleBuyWepaon = !pGameMode->IsUnlockWeapon(nWeaponIndex);
	IsAbleBuyPower = pGameMode->IsAbleBuyRewardPointPower(nWeaponIndex);
	IsAbleBuyAmmo = pGameMode->IsAbleBuyRewardPointAmmo(nWeaponIndex);

	// 무기 데이터 설정
	auto pWeaponOwnData = pPlayerSaveData->WeaponOwnData.Find(nWeaponIndex);
	if (pWeaponOwnData)
	{
		SetPowerLevel(pWeaponOwnData->PowerLevel);
		SetAmmoLevel(pWeaponOwnData->MagLevel);
		SetEnable(pWeaponOwnData->IsUnlock);
	}
	else
	{
		SetEnable(false);
		return;
	}
}

void UPG_UserWidgetShopWeaponBox::SetSlotIndex(int32 nSlotIndex)
{
	SlotIndex = nSlotIndex;
}

void UPG_UserWidgetShopWeaponBox::SetPowerLevel(int32 nPowerLevel)
{
	PowerLevel = nPowerLevel;
}

void UPG_UserWidgetShopWeaponBox::SetAmmoLevel(int32 nAmmoLevel)
{
	AmmoLevel = nAmmoLevel;
}

void UPG_UserWidgetShopWeaponBox::SetEnable(bool bEnable)
{
	ABCHECK(nullptr != Btn_Weapon);
	ABCHECK(nullptr != Btn_Power);
	ABCHECK(nullptr != Btn_Ammo);
	ABCHECK(nullptr != Text_PowerLevel);
	ABCHECK(nullptr != Text_AmmoLevel);
	ABCHECK(nullptr != Img_WeaponLock);
	ABCHECK(nullptr != Img_PowerUp);
	ABCHECK(nullptr != Img_AmmoUp);

	if (bEnable)
	{
		Btn_Weapon->SetVisibility(ESlateVisibility::HitTestInvisible);
		Btn_Weapon->SetIsEnabled(false);
		Btn_Power->SetVisibility(ESlateVisibility::Visible);
		Btn_Power->SetIsEnabled(!(IsMaxPower || false == IsAbleBuyPower));
		Btn_Ammo->SetVisibility(ESlateVisibility::Visible);
		Btn_Ammo->SetIsEnabled(!(IsMaxAmmo || false == IsAbleBuyAmmo));

		if (IsMaxPower)
			Text_PowerLevel->SetText(FText::FromString(TEXT("MAX")));
		else
			Text_PowerLevel->SetText(FText::FromString(*FString::Printf(TEXT("LV %d"), PowerLevel)));

		if (IsMaxAmmo)
			Text_AmmoLevel->SetText(FText::FromString(TEXT("MAX")));
		else
			Text_AmmoLevel->SetText(FText::FromString(*FString::Printf(TEXT("LV %d"), AmmoLevel)));

		Img_WeaponLock->SetVisibility(ESlateVisibility::Hidden);

		if (IsMaxPower || false == IsAbleBuyPower)
			Img_PowerUp->SetVisibility(ESlateVisibility::Hidden);
		else
			Img_PowerUp->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (IsMaxAmmo || false == IsAbleBuyAmmo)
			Img_AmmoUp->SetVisibility(ESlateVisibility::Hidden);
		else
			Img_AmmoUp->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		Btn_Weapon->SetVisibility(ESlateVisibility::Visible);
		Btn_Weapon->SetIsEnabled(IsAbleBuyWepaon);
		Btn_Power->SetVisibility(ESlateVisibility::HitTestInvisible);
		Btn_Ammo->SetVisibility(ESlateVisibility::HitTestInvisible);

		Img_WeaponLock->SetVisibility(ESlateVisibility::HitTestInvisible);
		Img_PowerUp->SetVisibility(ESlateVisibility::Hidden);
		Img_AmmoUp->SetVisibility(ESlateVisibility::Hidden);

		Text_PowerLevel->SetText(FText::FromString(TEXT("LV --")));
		Text_AmmoLevel->SetText(FText::FromString(TEXT("LV --")));
	}
}

void UPG_UserWidgetShopWeaponBox::UpdateWeaponShopLayout()
{
	ABCHECK(PG_INVALID_INDEX != WeaponIndex);
	ABCHECK(nullptr != m_pGameMode);

	SetWeaponIndex(m_pGameMode.Get(), WeaponIndex);
}

void UPG_UserWidgetShopWeaponBox::ShowBuyPopupBox(EBuyType ekBuyType, FString kTitle, FString kDescription, int32 nPrice)
{
	ABCHECK(nullptr != m_pUserWidgetBuyPopup);
	m_pUserWidgetBuyPopup->SetValue((int32)ekBuyType);
	m_pUserWidgetBuyPopup->SetTitle(kTitle);
	m_pUserWidgetBuyPopup->SetPrice(nPrice);
	m_pUserWidgetBuyPopup->SetDescription(kDescription);
	m_pUserWidgetBuyPopup->SetVisibility(ESlateVisibility::Visible);
}

void UPG_UserWidgetShopWeaponBox::ShowMessageBox(FString kTitle, FString kDescription, int32 nValue)
{
	ABCHECK(nullptr != m_pUserWidgetMessagePopup);
	m_pUserWidgetMessagePopup->SetValue(nValue);
	m_pUserWidgetMessagePopup->SetTitle(kTitle);
	m_pUserWidgetMessagePopup->SetDescription(kDescription);
	m_pUserWidgetMessagePopup->SetVisibility(ESlateVisibility::Visible);
}

void UPG_UserWidgetShopWeaponBox::OnBuyOK(int32 nValue)
{
	ABCHECK(nullptr != m_pUserWidgetBuyPopup);
	m_pUserWidgetBuyPopup->SetVisibility(ESlateVisibility::Hidden);

	if (false == m_pGameMode.IsValid(true))
	{
		ABLOG(Error, TEXT("m_pGameMode is nullptr"));
		ShowMessageBox(TEXT("Error"), TEXT("Purchase failed."), PG_INVALID_INDEX);
		return;
	}
	
	auto pMyPlayerState =Cast<APG_MyPlayerState>(GetOwningPlayerState());
	ABCHECK(nullptr != pMyPlayerState);

	// 구매 진행
	bool bBuySuccesed = false;
	switch ((EBuyType)nValue)
	{
	case EBuyType::eBT_Weapon:		bBuySuccesed = m_pGameMode->BuyWeapon(WeaponIndex);		break;
	case EBuyType::eBT_Power:		bBuySuccesed = m_pGameMode->BuyPowerUp(WeaponIndex);	break;
	case EBuyType::eBT_Ammo:		bBuySuccesed = m_pGameMode->BuyAmmoUp(WeaponIndex);		break;
	default:		break;
	}

	// 구매 실패
	if (false == bBuySuccesed)
	{
		auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
		ABCHECK(nullptr != pGameInstance);

		const FPGWeaponTableData* pWeaponShopData = pGameInstance->GetWeaponTableData(WeaponIndex);
		if (nullptr == pWeaponShopData)
		{
			ABLOG(Error, TEXT("GetWeaponTableData is Null (WeaponIndex : %d)"), WeaponIndex);
			SetEnable(false);
			return;
		}

		ShowMessageBox(pWeaponShopData->WeaponData.Name, m_pGameMode->GetLastBuyErrorMsg(), nValue);
	}

	OnItemBuyWeapon.Broadcast();
}

void UPG_UserWidgetShopWeaponBox::OnBuyCancel(int32 nValue)
{
	ABCHECK(nullptr != m_pUserWidgetBuyPopup);
	m_pUserWidgetBuyPopup->SetVisibility(ESlateVisibility::Hidden);
}

void UPG_UserWidgetShopWeaponBox::OnMessagePopupBoxClose()
{
	ABCHECK(nullptr != m_pUserWidgetMessagePopup);
	m_pUserWidgetMessagePopup->SetVisibility(ESlateVisibility::Hidden);
}

void UPG_UserWidgetShopWeaponBox::OnRequestUnlockWeapon()
{
	if (false == m_pGameMode.IsValid(true))
	{
		ABLOG(Error, TEXT("m_pGameMode is nullptr"));
		ShowMessageBox(TEXT("Error"), TEXT("Purchase failed."), PG_INVALID_INDEX);
		return;
	}

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	const FPGWeaponTableData* pWeaponShopData = pGameInstance->GetWeaponTableData(WeaponIndex);
	if (nullptr == pWeaponShopData)
	{
		ABLOG(Error, TEXT("GetWeaponTableData is Null (WeaponIndex : %d)"), WeaponIndex);
		SetEnable(false);
		ShowMessageBox(TEXT("Error"), TEXT("Purchase failed."), PG_INVALID_INDEX);
		return;
	}
	
	ShowBuyPopupBox(EBuyType::eBT_Weapon, pWeaponShopData->WeaponData.Name, TEXT("Do you want to buy?"), pWeaponShopData->CostUnlock);
}

void UPG_UserWidgetShopWeaponBox::OnRequestLevelupPower()
{
	if (false == m_pGameMode.IsValid(true))
	{
		ABLOG(Error, TEXT("m_pGameMode is nullptr"));
		ShowMessageBox(TEXT("Error"), TEXT("Purchase failed."), PG_INVALID_INDEX);
		return;
	}

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	const FPGWeaponTableData* pWeaponShopData = pGameInstance->GetWeaponTableData(WeaponIndex);
	if (nullptr == pWeaponShopData)
	{
		ABLOG(Error, TEXT("GetWeaponTableData is Null (WeaponIndex : %d)"), WeaponIndex);
		SetEnable(false);
		ShowMessageBox(TEXT("Error"), TEXT("Purchase failed."), PG_INVALID_INDEX);
		return;
	}

	ShowBuyPopupBox(EBuyType::eBT_Power, FString::Printf(TEXT("%s Power"), *pWeaponShopData->WeaponData.Name), TEXT("Do you want to buy?"), pWeaponShopData->CostUpgradePower);
}

void UPG_UserWidgetShopWeaponBox::OnRequestLevelupAmmo()
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

	const FPGWeaponTableData* pWeaponShopData = pGameInstance->GetWeaponTableData(WeaponIndex);
	if (nullptr == pWeaponShopData)
	{
		ABLOG(Error, TEXT("GetWeaponTableData is Null (WeaponIndex : %d)"), WeaponIndex);
		SetEnable(false);
		ShowMessageBox(TEXT("Error"), TEXT("Purchase failed."), PG_INVALID_INDEX);
		return;
	}

	ShowBuyPopupBox(EBuyType::eBT_Ammo, FString::Printf(TEXT("%s Ammo"), *pWeaponShopData->WeaponData.Name), TEXT("Do you want to buy?"), pWeaponShopData->CostUpgradeAmmo);
}