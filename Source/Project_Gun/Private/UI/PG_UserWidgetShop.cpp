// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PG_UserWidgetShop.h"
#include "UI/PG_UserWidgetShopWeaponBox.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Game/GameMode/PG_GameModeMainLobby.h"
#include "Player/PlayerState/PG_MyPlayerState.h"
#include "SaveData/PG_SavePlayerData.h"

#include <Components/Button.h>
#include <Components/TextBlock.h>

void UPG_UserWidgetShop::NativeConstruct()
{
	Super::NativeConstruct();

	//ABCHECK(nullptr != Text_ADRemainTime);
	PG_WIDGET_LINK(UTextBlock, Text_ADRemainTime);
	
	m_pBtn_ADView = Cast<UButton>(GetWidgetFromName(TEXT("Btn_ADView")));
	ABCHECK(nullptr != m_pBtn_ADView);

	for (int32 nIndex = 0; nIndex < eMaxWeaponShopItemSlot; ++nIndex)
	{
		FString kWeaponShopBoxWidgetName = FString::Printf(TEXT("WB_ShopWeaponBox_%d"), nIndex);
		auto pWeaponShopBox = Cast<UPG_UserWidgetShopWeaponBox>(GetWidgetFromName(*kWeaponShopBoxWidgetName));
		if (nullptr == pWeaponShopBox)
		{
			ABLOG(Error, TEXT("Can't find UserWidgetShopWeaponBox (Index : %d)"), nIndex);
			continue;
		}

		pWeaponShopBox->OnItemBuyWeapon.AddUObject(this, &UPG_UserWidgetShop::UpdateWeaponShopLayout);
	}
}

void UPG_UserWidgetShop::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	int32 nRemainTime = pGameInstance->GetADRemainTime(EADType::eAT_WeaponShop);
	SetADRemainTime(nRemainTime);
	SetBtnEnableAD((nRemainTime <= 0));

}

void UPG_UserWidgetShop::UpdateLethalShopLayout()
{

}

void UPG_UserWidgetShop::UpdateWeaponShopLayout()
{
	UpdateRewardPoint();

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	auto pGameModeMainLobby = Cast<APG_GameModeMainLobby>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeMainLobby);

	for (int32 nIndex = 0; nIndex < eMaxWeaponShopItemSlot; ++nIndex)
	{
		FString kWeaponShopBoxWidgetName = FString::Printf(TEXT("WB_ShopWeaponBox_%d"), nIndex);
		auto pWeaponShopBox = Cast<UPG_UserWidgetShopWeaponBox>(GetWidgetFromName(*kWeaponShopBoxWidgetName));
		if (nullptr == pWeaponShopBox)
		{
			ABLOG(Error, TEXT("Can't find UserWidgetShopWeaponBox (Index : %d)"), nIndex);
			continue;
		}

		pWeaponShopBox->SetWeaponIndex(pGameModeMainLobby, nIndex);
		pWeaponShopBox->SetSlotIndex(nIndex);
	}
}

void UPG_UserWidgetShop::SetBtnEnableAD(bool bEnable)
{
	ABCHECK(nullptr != m_pBtn_ADView);

	if (m_pBtn_ADView->bIsEnabled != bEnable)
		m_pBtn_ADView->SetIsEnabled(bEnable);

	if (bEnable)
		m_pkText_ADRemainTime->SetVisibility(ESlateVisibility::Hidden);
	else
		m_pkText_ADRemainTime->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPG_UserWidgetShop::SetADRemainTime(int32 nTime)
{
	m_pkText_ADRemainTime->SetText(FText::FromString(FString::Printf(TEXT("%d"), nTime)));
}

void UPG_UserWidgetShop::OnADRewardVideoSucessed()
{
	auto pGameMode = Cast<APG_GameModeBase>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameMode);

	int32 nRewardPoint = pGameMode->GetSavePlayerData()->m_nRewardPoint + PG_ADVIEW_SHOP_REWARD;
	pGameMode->GetSavePlayerData()->m_nRewardPoint = nRewardPoint;
	pGameMode->SavePlayerData();

	auto pMyPlayerState = Cast<APG_MyPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);
	ABCHECK(nullptr != pMyPlayerState);
	pMyPlayerState->SetRewardPoint(nRewardPoint);

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	pGameInstance->ResetADRemainTime(EADType::eAT_WeaponShop);

	UpdateWeaponShopLayout();
}

void UPG_UserWidgetShop::OnADRewardVideoClosed()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	pGameInstance->ResetADRemainTime(EADType::eAT_WeaponShop);
}