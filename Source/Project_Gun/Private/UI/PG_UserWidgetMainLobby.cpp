// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PG_UserWidgetMainLobby.h"
#include "Game/GameMode/PG_GameModeMainLobby.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Define/PG_GamePlayData.h"
#include "SaveData/PG_SavePlayerData.h"
#include "UI/PG_UserWidgetWeaponBox.h"
#include "UI/PG_UserWidgetWeaponInventory.h"
#include <Components/Overlay.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>

void UPG_UserWidgetMainLobby::NativeConstruct()
{
	Super::NativeConstruct();

	// Overlay Stars earned
	Overlay_ActiveFlag = Cast<UOverlay>(GetWidgetFromName(TEXT("Overlay_ActiveFlag")));
	ABCHECK(nullptr != Overlay_ActiveFlag);
	Overlay_DeActiveFlag = Cast<UOverlay>(GetWidgetFromName(TEXT("Overlay_DeActiveFlag")));
	ABCHECK(nullptr != Overlay_DeActiveFlag);
	Overlay_ActiveTime = Cast<UOverlay>(GetWidgetFromName(TEXT("Overlay_ActiveTime")));
	ABCHECK(nullptr != Overlay_ActiveTime);
	Overlay_DeActiveTime = Cast<UOverlay>(GetWidgetFromName(TEXT("Overlay_DeActiveTime")));
	ABCHECK(nullptr != Overlay_DeActiveTime);
	Overlay_ActiveLethal = Cast<UOverlay>(GetWidgetFromName(TEXT("Overlay_ActiveLethal")));
	ABCHECK(nullptr != Overlay_ActiveLethal);
	Overlay_DeActiveLethal = Cast<UOverlay>(GetWidgetFromName(TEXT("Overlay_DeActiveLethal")));
	ABCHECK(nullptr != Overlay_DeActiveLethal);

	ABCHECK(nullptr != Text_FlagValue);
	ABCHECK(nullptr != Text_TimeValue);
	ABCHECK(nullptr != Text_LethalValue);

	for (int32 nIndex = 0; nIndex < PG_MAX_WEAPON_SLOT; ++nIndex)
	{
		FString BtnWidgetName = FString::Printf(TEXT("WB_WeaponBox_%d"), nIndex);
		auto pBtnWeapon = Cast<UPG_UserWidgetWeaponBox>(GetWidgetFromName(*BtnWidgetName));
		ABCHECK(nullptr != pBtnWeapon);

		pBtnWeapon->SetEnable(false);
		pBtnWeapon->SetSlotIndex(nIndex);
		pBtnWeapon->OnBtnWeaponClicked_SlotIndex.AddUObject(this, &UPG_UserWidgetMainLobby::OnBtnClickedWeaponSlot);
	}

	ABCHECK(nullptr != WB_WeaponInventory);
	WB_WeaponInventory->OnBtnClickedClose.AddUObject(this, &UPG_UserWidgetMainLobby::OnCloseWeaponInventory);
}

FString UPG_UserWidgetMainLobby::GetSelectedStageName()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, TEXT("NONE"));

	int32 nSelectedStageID = pGameInstance->GetPlayStageID();
	const FPGStageData* cpStageData = pGameInstance->GetTableStageData(nSelectedStageID);
	if (cpStageData)
		return cpStageData->StageName;

	return TEXT("NONE");
}

void UPG_UserWidgetMainLobby::UpdateClearStageLayout()
{
	auto pGameModeMainLobby = Cast<APG_GameModeMainLobby>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeMainLobby);

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	int32 nSelectedStageID = pGameInstance->GetPlayStageID();
	const FPGStageData* cpStageData = pGameInstance->GetTableStageData(nSelectedStageID);
	ABCHECK(nullptr != cpStageData);

	bool bIsClearAmount = pGameModeMainLobby->IsClearMission(EStageClearType::eSCT_AmountKill);
	Overlay_ActiveFlag->SetIsEnabled(bIsClearAmount);
	Overlay_DeActiveFlag->SetIsEnabled(!bIsClearAmount);
	Text_FlagValue->SetText(FText::FromString(FString::Printf(TEXT("%d"), cpStageData->M_Amount)));

	bool bIsClearTime = pGameModeMainLobby->IsClearMission(EStageClearType::eSCT_ClearTime);
	Overlay_ActiveTime->SetIsEnabled(bIsClearTime);
	Overlay_DeActiveTime->SetIsEnabled(!bIsClearTime);
	Text_TimeValue->SetText(FText::FromString(FString::Printf(TEXT("%d"), cpStageData->Time)));

	bool bIsClearLethal = pGameModeMainLobby->IsClearMission(EStageClearType::eSCT_Lethal);
	Overlay_ActiveLethal->SetIsEnabled(bIsClearLethal);
	Overlay_DeActiveLethal->SetIsEnabled(!bIsClearLethal);
	Text_LethalValue->SetText(FText::FromString(FString::Printf(TEXT("%d"), cpStageData->Multi_Kill)));
}

void UPG_UserWidgetMainLobby::UpdateWeaponSlotLayout()
{
	auto pGameModeMainLobby = Cast<APG_GameModeMainLobby>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeMainLobby);

	auto pSavePlayerData = pGameModeMainLobby->GetSavePlayerData();
	ABCHECK(nullptr != pSavePlayerData);

	ABCHECK(nullptr != WB_WeaponInventory);
	
	for (int32 nIndex = 0; nIndex < PG_MAX_WEAPON_SLOT; ++nIndex)
	{
		FString BtnWidgetName = FString::Printf(TEXT("WB_WeaponBox_%d"), nIndex);
		auto pBtnWeapon = Cast<UPG_UserWidgetWeaponBox>(GetWidgetFromName(*BtnWidgetName));
		if (nullptr == pBtnWeapon)
		{
			ABLOG(Error, TEXT("Can't find UserWidgetWeaponBox (Index : %d)"), nIndex);
			continue;
		}

		pBtnWeapon->SetWeaponIndex(pGameModeMainLobby, pSavePlayerData->m_kEquipWeaponTableIndex[nIndex]);

		if (PG_MAX_WEAPON_SLOT - 1 == nIndex)
			pBtnWeapon->SetEnable(pSavePlayerData->m_bExtendWeaponSlot);
		else
			pBtnWeapon->SetEnable(true);
	}
}

void UPG_UserWidgetMainLobby::AllHideIconOpenSlot()
{
	for (int32 nIndex = 0; nIndex < PG_MAX_WEAPON_SLOT; ++nIndex)
	{
		SetEnableIconOpenSlot(nIndex, false);
	}
}

void UPG_UserWidgetMainLobby::SetEnableIconOpenSlot(int32 nSlotIndex, bool bEnable)
{
	FString kSlotName = FString::Printf(TEXT("OpenSlot_%d"), nSlotIndex);
	auto BtnWeaponSlot = Cast<UOverlay>(GetWidgetFromName(*kSlotName));
	ABCHECK(nullptr != BtnWeaponSlot);

	if (bEnable)
		BtnWeaponSlot->SetVisibility(ESlateVisibility::HitTestInvisible);
	else
		BtnWeaponSlot->SetVisibility(ESlateVisibility::Hidden);
}

bool UPG_UserWidgetMainLobby::OnNextSelectStage()
{
	auto GameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != GameInstance, false);

	return GameInstance->OnNextSelectStage();
}

bool UPG_UserWidgetMainLobby::OnPrevSelectStage()
{
	auto GameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != GameInstance, false);

	return GameInstance->OnPrevSelectStage();
}

void UPG_UserWidgetMainLobby::OnBtnClickedWeaponSlot(int32 nSlotIndex)
{
	ABCHECK(nullptr != WB_WeaponInventory);

	AllHideIconOpenSlot();
	SetEnableIconOpenSlot(nSlotIndex, true);

	// 인벤토리 창을 연다.
	WB_WeaponInventory->SetOpenSlotIndex(nSlotIndex);
	WB_WeaponInventory->SetVisibility(ESlateVisibility::Visible);
	WB_WeaponInventory->UpdateWeaponInventory();
}

void UPG_UserWidgetMainLobby::OnCloseWeaponInventory(int32 nSlotIndex, int32 nWeaponIndex)
{
	// 인벤토리 창이 닫힘
	ABCHECK(nullptr != WB_WeaponInventory);
	WB_WeaponInventory->SetVisibility(ESlateVisibility::Hidden);
	AllHideIconOpenSlot();

	if (PG_INVALID_INDEX != nWeaponIndex)
	{
		auto pGameModeMainLobby = Cast<APG_GameModeMainLobby>(GetWorld()->GetAuthGameMode());
		ABCHECK(nullptr != pGameModeMainLobby);
		pGameModeMainLobby->SetEquipWeapon(nSlotIndex, nWeaponIndex);

		UpdateWeaponSlotLayout();
	}
}

