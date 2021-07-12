// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PG_UserWidgetMainLobby_Renewal.h"
#include "UI/PG_UserWidgetWeaponBox.h"
#include "UI/PG_UserWidgetWeaponInventory.h"
#include "UI/PG_UserWidgetScreenShop.h"
#include "Game/GameMode/PG_GameModeMainLobby.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "SaveData/PG_SavePlayerData.h"

#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/CanvasPanel.h>
#include <Components/ScaleBox.h>


void UPG_UserWidgetMainLobby_Renewal::NativeConstruct()
{

	//===========================================
	// Canvas TopMenu
	//===========================================
	//PG_WIDGET_LINK(UUserWidget, WB_RewardPoint);
	PG_WIDGET_LINK(UButton, BtnOption);

	//===========================================
	// Canvas StageSelect
	//===========================================
	PG_WIDGET_LINK(UButton, BtnSelectStagePrev);
	PG_WIDGET_LINK(UButton, BtnSelectStageNext);
	PG_WIDGET_LINK(UTextBlock, TextSelectedStage);
	PG_WIDGET_LINK(UCanvasPanel, CanvasSelectStageLock);

	//===========================================
	// Canvas Mission
	//===========================================
	PG_WIDGET_LINK(UButton, BtnFlag);
	PG_WIDGET_LINK(UButton, BtnLethal);
	PG_WIDGET_LINK(UButton, BtnTime);
	PG_WIDGET_LINK(UTextBlock, TextMainLobbyFlag);
	PG_WIDGET_LINK(UTextBlock, TextMainLobbyLethal);
	PG_WIDGET_LINK(UTextBlock, TextMainLobbyTime);

	//===========================================
	// Canvas MenuButtons
	//===========================================
	PG_WIDGET_LINK(UButton, BtnGameStart);
	PG_WIDGET_LINK(UButton, BtnShop);
	PG_WIDGET_LINK(UCanvasPanel, CanvasGameStart);

	//===========================================
	// Canvas WeaponBox (무기 슬롯)
	//===========================================
	PG_WIDGET_LINK_TArray(UPG_UserWidgetWeaponBox, WB_WeaponBox, eMaxWeaponSlot);

	int nIndex = 0;
	for (auto Box : m_pkWB_WeaponBox)
	{
		if (Box)
		{
			Box->SetEnable(false);
			Box->SetSlotIndex(nIndex++);
			Box->OnBtnWeaponClicked_SlotIndex.AddUObject(this, &UPG_UserWidgetMainLobby_Renewal::OnOpenWeaponInventory);
		}
	}

	//===========================================
	// Canvas ADBanner
	//===========================================

	//===========================================
	// Canvas WeaponInventory (무기 인벤토리)
	//===========================================
	PG_WIDGET_LINK(UScaleBox, ScaleWeaponInventory);
	PG_WIDGET_LINK(UPG_UserWidgetWeaponInventory, WB_WeaponInventory);
	m_pkWB_WeaponInventory->OnBtnClickedClose.AddUObject(this, &UPG_UserWidgetMainLobby_Renewal::OnCloseWeaponInventory);

	Super::NativeConstruct();

}

void UPG_UserWidgetMainLobby_Renewal::UpdateLayoutStage()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	auto pGameModeMainLobby = Cast<APG_GameModeMainLobby>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeMainLobby);

	int32 nSelectedStageID = pGameModeMainLobby->GetPlayStageID();
	const FPGStageData* cpStageData = pGameInstance->GetTableStageData(nSelectedStageID);
	ABCHECK(nullptr != cpStageData);

	const FPGStageData* cpNextStageData = pGameInstance->GetTableStageData(nSelectedStageID + 1);
	const FPGStageData* cpPrevStageData = pGameInstance->GetTableStageData(nSelectedStageID - 1);

	m_pkBtnSelectStageNext->SetIsEnabled((nullptr != cpNextStageData));
	m_pkBtnSelectStagePrev->SetIsEnabled((nullptr != cpPrevStageData));

	bool bIsFirstPlayCurrStage = pGameModeMainLobby->IsFirstPlayStage(nSelectedStageID);
	bool bIsFirstPlayNextStage = pGameModeMainLobby->IsFirstPlayStage(nSelectedStageID + 1);
	bool bIsFirstPlayPrevStage = pGameModeMainLobby->IsFirstPlayStage(nSelectedStageID - 1);

	// 처음 플레이 하는 경우
	if (bIsFirstPlayCurrStage)
	{
		if (1 == nSelectedStageID || false == bIsFirstPlayPrevStage)
		{
			m_pkCanvasSelectStageLock->SetVisibility(ESlateVisibility::Hidden);
			m_pkCanvasGameStart->SetIsEnabled(true);
		}
		// 다음 스테이지 진행 안했음
		else if (bIsFirstPlayNextStage)
		{
			m_pkCanvasSelectStageLock->SetVisibility(ESlateVisibility::HitTestInvisible);
			m_pkCanvasGameStart->SetIsEnabled(false);
		}
	}
	else
	{
		m_pkCanvasSelectStageLock->SetVisibility(ESlateVisibility::Hidden);
		m_pkCanvasGameStart->SetIsEnabled(true);
	}
}

void UPG_UserWidgetMainLobby_Renewal::UpdateLayoutMission()
{
	//m_pkWB_RewardPoint->UpdateRewardPoint();

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	auto pGameModeMainLobby = Cast<APG_GameModeMainLobby>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeMainLobby);

	auto pSavePlayerData = pGameModeMainLobby->GetSavePlayerData();
	ABCHECK(nullptr != pSavePlayerData);

	int32 nSelectedStageID = pGameModeMainLobby->GetPlayStageID();
	const FPGStageData* cpStageData = pGameInstance->GetTableStageData(nSelectedStageID);
	ABCHECK(nullptr != cpStageData);

	bool bIsClearAmount = pGameModeMainLobby->IsClearMission(EStageClearType::eSCT_AmountKill);
	bool bIsClearTime = pGameModeMainLobby->IsClearMission(EStageClearType::eSCT_ClearTime);
	bool bIsClearLethal = pGameModeMainLobby->IsClearMission(EStageClearType::eSCT_Lethal);

	m_pkBtnFlag->SetIsEnabled(!bIsClearAmount);
	m_pkBtnLethal->SetIsEnabled(!bIsClearLethal);
	m_pkBtnTime->SetIsEnabled(!bIsClearTime);

	m_pkTextMainLobbyFlag->SetText(FText::FromString(FString::Printf(TEXT("%d"), cpStageData->M_Amount)));
	m_pkTextMainLobbyLethal->SetText(FText::FromString(FString::Printf(TEXT("%d"), cpStageData->Multi_Kill)));
	m_pkTextMainLobbyTime->SetText(FText::FromString(FString::Printf(TEXT("%d"), cpStageData->Time)));

	m_pkTextSelectedStage->SetText(FText::FromString(cpStageData->StageName));
}

void UPG_UserWidgetMainLobby_Renewal::UpdateLayoutWeaponSlot()
{
	auto pGameModeMainLobby = Cast<APG_GameModeMainLobby>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeMainLobby);

	auto pSavePlayerData = pGameModeMainLobby->GetSavePlayerData();
	ABCHECK(nullptr != pSavePlayerData);

	int nIndex = 0;
	for (auto WeaponSlot : m_pkWB_WeaponBox)
	{
		ABCHECK(nullptr != WeaponSlot);
		WeaponSlot->SetWeaponIndex(pGameModeMainLobby, pSavePlayerData->m_kEquipWeaponTableIndex[nIndex]);

		if (PG_MAX_WEAPON_SLOT - 1 == nIndex)
			WeaponSlot->SetEnable(pSavePlayerData->m_bExtendWeaponSlot);
		else
			WeaponSlot->SetEnable(true);

		nIndex++;
	}
}

void UPG_UserWidgetMainLobby_Renewal::OnBtnOption()
{

}

void UPG_UserWidgetMainLobby_Renewal::OnBtnSelectStageNext()
{
	auto pGameModeMainLobby = Cast<APG_GameModeMainLobby>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeMainLobby);

	ABCHECK(pGameModeMainLobby->SetNextStageID());
	UpdateLayoutStage();
	UpdateLayoutMission();
}

void UPG_UserWidgetMainLobby_Renewal::OnBtnSelectStagePrev()
{
	auto pGameModeMainLobby = Cast<APG_GameModeMainLobby>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeMainLobby);
	ABCHECK(pGameModeMainLobby->SetPrevStageID());
	UpdateLayoutStage();
	UpdateLayoutMission();
}

void UPG_UserWidgetMainLobby_Renewal::OnBtnGameStart()
{
	auto pGameModeBase = Cast<APG_GameModeBase>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeBase);

	if (pGameModeBase->StartGame())
	{
		GetWorld()->GetGameViewport()->RemoveAllViewportWidgets();
	}
}

void UPG_UserWidgetMainLobby_Renewal::OnBtnShop()
{
	auto GameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != GameInstance);

	if (nullptr == m_pWBShop)
	{
		m_pWBShop = CreateWidget<UPG_UserWidgetScreenShop>(this, WB_ShopClass);
		ABCHECK(nullptr != m_pWBShop);
	}

	GetWorld()->GetGameViewport()->RemoveAllViewportWidgets();
	m_pWBShop->AddToViewport();
	m_pWBShop->Init();
}

void UPG_UserWidgetMainLobby_Renewal::OnOpenWeaponInventory(int32 nSlotIndex)
{
	ABCHECK(nullptr != m_pkWB_WeaponInventory);

	// 첫번째 인벤토리는 기본 라이플 무기를 사용해야 하므로 수정 못하도록 한다. 
	if (0 == nSlotIndex)
		return;

	// 인벤토리 창을 연다.
	m_pkScaleWeaponInventory->SetVisibility(ESlateVisibility::Visible);
	m_pkWB_WeaponInventory->SetOpenSlotIndex(nSlotIndex);
	m_pkWB_WeaponInventory->SetVisibility(ESlateVisibility::Visible);
	m_pkWB_WeaponInventory->UpdateWeaponInventory();

	m_pkWB_WeaponBox[nSlotIndex]->SetEnableLightBox(true);
}

void UPG_UserWidgetMainLobby_Renewal::OnCloseWeaponInventory(int32 nSlotIndex, int32 nWeaponIndex)
{
	ABCHECK(nullptr != m_pkWB_WeaponInventory);

	// 인벤토리 창이 닫힘
	m_pkScaleWeaponInventory->SetVisibility(ESlateVisibility::Hidden);
	m_pkWB_WeaponBox[nSlotIndex]->SetEnableLightBox(false);
	if (PG_INVALID_INDEX != nWeaponIndex)
	{
		auto pGameModeMainLobby = Cast<APG_GameModeMainLobby>(GetWorld()->GetAuthGameMode());
		ABCHECK(nullptr != pGameModeMainLobby);
		pGameModeMainLobby->SetEquipWeapon(nSlotIndex, nWeaponIndex);

		UpdateLayoutWeaponSlot();
	}
}

void UPG_UserWidgetMainLobby_Renewal::OnDoNotChangeCloseWeaponInventory()
{
	ABCHECK(nullptr != m_pkWB_WeaponInventory);

	// 인벤토리 창이 닫힘
	m_pkScaleWeaponInventory->SetVisibility(ESlateVisibility::Hidden);
	m_pkWB_WeaponInventory->SetVisibility(ESlateVisibility::Hidden);

	for (auto WeaponBox : m_pkWB_WeaponBox)
	{
		if (WeaponBox)
			WeaponBox->SetEnableLightBox(false);
	}
}