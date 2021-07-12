// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PG_UserWidgetScreenShop.h"
#include "UI/PG_UserWidgetSkillSlot.h"
#include "UI/PG_UserWidgetShopWeaponSlot.h"
#include "UI/PG_UserWidgetMainLobby_Renewal.h"
#include "UI/PG_UserWidgetMessagePopup.h"

#include "Game/GameInstance/PG_GameInstance.h"
#include "Game/GameMode/PG_GameModeMainLobby.h"

#include "Player/PlayerState/PG_MyPlayerState.h"

#include "SaveData/PG_SavePlayerData.h"

#include <Components/Button.h>
#include <Components/TextBlock.h>

void UPG_UserWidgetScreenShop::NativeConstruct()
{
	Super::NativeConstruct();

	PG_WIDGET_LINK(UButton, BtnGotoLobby);
	PG_WIDGET_LINK(UButton, BtnADView);
	PG_WIDGET_LINK(UTextBlock, TextADRemainTime);
	PG_WIDGET_LINK_TArray(UPG_UserWidgetSkillSlot, WB_SkillSlot, eMaxLethalShopItemSlot);
	PG_WIDGET_LINK_TArray(UPG_UserWidgetShopWeaponSlot, WB_ShopWeaponSlot, eMaxWeaponShopItemSlot);

	for (auto ShopWeaponSlot : m_pkWB_ShopWeaponSlot)
	{
		if (ShopWeaponSlot)
			ShopWeaponSlot->OnItemBuyWeapon.AddUObject(this, &UPG_UserWidgetScreenShop::UpdateRewardPoint);
	}

	{
		m_pUserWidgetMessagePopup = CreateWidget<UPG_UserWidgetMessagePopup>(GetWorld(), LoadClass<UPG_UserWidgetMessagePopup>(NULL, TEXT("WidgetBlueprint'/Game/PG/Blueprints/WidgetBP/WB_MessagePopup.WB_MessagePopup_C'")));
		ABCHECK(nullptr != m_pUserWidgetMessagePopup);

		m_pUserWidgetMessagePopup->OnBtnClickedOK.AddUObject(this, &UPG_UserWidgetScreenShop::OnMessagePopupBoxClose);
		m_pUserWidgetMessagePopup->SetVisibility(ESlateVisibility::Hidden);
		m_pUserWidgetMessagePopup->AddToViewport(999);
	}
}

void UPG_UserWidgetScreenShop::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	int32 nRemainTime = pGameInstance->GetADRemainTime(EADType::eAT_WeaponShop);
	SetADRemainTime(nRemainTime);
	SetBtnEnableAD((nRemainTime <= 0));
}

void UPG_UserWidgetScreenShop::Init()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	auto pGameModeMainLobby = Cast<APG_GameModeMainLobby>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeMainLobby);

	TArray<const FPGWeaponTableData*> kWeaponTableDatas;
	ABCHECK(pGameInstance->GetAllWeaponTableData(kWeaponTableDatas));

	int nSlotIndex = 0;
	int nWeaponIndex = 0;
	for (auto WeaponTableData : kWeaponTableDatas)
	{
		if (nSlotIndex >= m_pkWB_ShopWeaponSlot.Num())
			break;

		if (false == WeaponTableData->ServiceOpen)
		{
			nWeaponIndex++;
			continue;
		}

		if (m_pkWB_ShopWeaponSlot[nSlotIndex])
		{
			m_pkWB_ShopWeaponSlot[nSlotIndex]->SetWeaponIndex(pGameModeMainLobby, nWeaponIndex, nSlotIndex);
		}

		nSlotIndex++;
		nWeaponIndex++;
	}

	UpdateRewardPoint();
	UpdateLethalShopLayout();
	UpdateWeaponShopLayout();
}

void UPG_UserWidgetScreenShop::UpdateLethalShopLayout()
{
	int nLethalIndex = 0;
	for (auto ShopSkillSlot : m_pkWB_SkillSlot)
	{
		if (ShopSkillSlot)
		{
			// 현재 스킬은 한개만 구현되어 있으므로 임시로 첫번째 스킬 슬롯만 보여주게 한다.
			ShopSkillSlot->SetEnable((0 == nLethalIndex));
		}

		nLethalIndex++;
	}
}

void UPG_UserWidgetScreenShop::UpdateWeaponShopLayout()
{
	for (auto ShopWeaponSlot : m_pkWB_ShopWeaponSlot)
	{
		ShopWeaponSlot->UpdateLayout();
	}
}

void UPG_UserWidgetScreenShop::SetBtnEnableAD(bool bEnable)
{
	if (m_pkBtnADView->bIsEnabled != bEnable)
		m_pkBtnADView->SetIsEnabled(bEnable);

	if (bEnable)
		m_pkTextADRemainTime->SetVisibility(ESlateVisibility::Hidden);
	else
		m_pkTextADRemainTime->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPG_UserWidgetScreenShop::SetADRemainTime(int32 nTime)
{
	m_pkTextADRemainTime->SetText(FText::FromString(FString::Printf(TEXT("%d"), nTime)));
}

void UPG_UserWidgetScreenShop::ShowMessageBox(FString kTitle, FString kDescription)
{
	ABCHECK(nullptr != m_pUserWidgetMessagePopup);
	m_pUserWidgetMessagePopup->SetTitle(kTitle);
	m_pUserWidgetMessagePopup->SetDescription(kDescription);
	m_pUserWidgetMessagePopup->SetVisibility(ESlateVisibility::Visible);
}

void UPG_UserWidgetScreenShop::OnBtnGotoLobby()
{
	auto GameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != GameInstance);

	if (nullptr == m_pWBMainLobby)
	{
		m_pWBMainLobby = CreateWidget<UPG_UserWidgetMainLobby_Renewal>(this, WB_MainLobbyClass);
		ABCHECK(nullptr != m_pWBMainLobby);
	}

	GetWorld()->GetGameViewport()->RemoveAllViewportWidgets();
	m_pWBMainLobby->AddToViewport();
	m_pWBMainLobby->UpdateLayoutStage();
	m_pWBMainLobby->UpdateLayoutMission();
	m_pWBMainLobby->UpdateLayoutWeaponSlot();
	m_pWBMainLobby->UpdateRewardPoint();
}

void UPG_UserWidgetScreenShop::OnBtnADView()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	pGameInstance->ResetADRemainTime(EADType::eAT_WeaponShop);
}

void UPG_UserWidgetScreenShop::OnADRewardVideoSucessed()
{
	auto pGameMode = Cast<APG_GameModeBase>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameMode);

	int32 nRewardPoint = pGameMode->GetSavePlayerData()->m_nRewardPoint + PG_ADVIEW_SHOP_REWARD;
	pGameMode->GetSavePlayerData()->m_nRewardPoint = nRewardPoint;
	pGameMode->SavePlayerData();

	auto pMyPlayerState = Cast<APG_MyPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);
	ABCHECK(nullptr != pMyPlayerState);
	pMyPlayerState->SetRewardPoint(nRewardPoint);

	UpdateWeaponShopLayout();
	UpdateRewardPoint();
	ShowMessageBox(TEXT("Get Reward Point!"), TEXT("+200 Point"));
}

void UPG_UserWidgetScreenShop::OnADRewardVideoError()
{
	UpdateRewardPoint();
	ShowMessageBox(TEXT("Error"), TEXT("Failed to get the reward.\nPlease try again later."));
}

void UPG_UserWidgetScreenShop::OnMessagePopupBoxClose()
{
	ABCHECK(nullptr != m_pUserWidgetMessagePopup);
	m_pUserWidgetMessagePopup->SetVisibility(ESlateVisibility::Hidden);
}