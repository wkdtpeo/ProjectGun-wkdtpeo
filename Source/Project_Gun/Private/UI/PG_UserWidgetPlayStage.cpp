// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PG_UserWidgetPlayStage.h"
#include "UI/PG_UserWidgetStageClearTarget.h"
#include "UI/PG_UserWidgetWeaponBox.h"
#include "Game/GameState/PG_GameStateStage.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Game/GameMode/PG_GameModeStage.h"
#include "Player/PlayerState/PG_MyPlayerState.h"
#include "Character/PG_MonChar.h"

#include <Components/Overlay.h>

void UPG_UserWidgetPlayStage::NativeConstruct()
{
	Super::NativeConstruct();

	ABCHECK(nullptr != WB_StageClearTarget);
	ABCHECK(nullptr != WB_WeaponBox_0);
	ABCHECK(nullptr != WB_WeaponBox_1);
	ABCHECK(nullptr != WB_WeaponBox_2);
	ABCHECK(nullptr != SelectWeapon_0);
	ABCHECK(nullptr != SelectWeapon_1);
	ABCHECK(nullptr != SelectWeapon_2);

	auto MyPlayerState = Cast<APG_MyPlayerState>(GetOwningPlayerState());
	if (MyPlayerState)
	{
		MyPlayerState->OnUpdateRewardPoint.AddUObject(this, &UPG_UserWidgetPlayStage::OnChangeRewardPoint);
	}

	auto GameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	if (GameStateStage)
	{
		GameStateStage->UdpateWidgetPlayStage.AddUObject(this, &UPG_UserWidgetPlayStage::OnUpdateFlag);
		GameStateStage->UdpateWidgetPlayStage.AddUObject(this, &UPG_UserWidgetPlayStage::OnUpdateLethal);
		GameStateStage->UdpateWidgetPlayStage.AddUObject(this, &UPG_UserWidgetPlayStage::OnUpdateTime);
	}

	WB_WeaponBox_0->SetSlotIndex(0);
	WB_WeaponBox_0->OnBtnWeaponClicked_SlotIndex.AddUObject(this, &UPG_UserWidgetPlayStage::OnChangeEquipWeaponIndex);
	WB_WeaponBox_1->SetSlotIndex(1);
	WB_WeaponBox_1->OnBtnWeaponClicked_SlotIndex.AddUObject(this, &UPG_UserWidgetPlayStage::OnChangeEquipWeaponIndex);
	WB_WeaponBox_2->SetSlotIndex(2);
	WB_WeaponBox_2->OnBtnWeaponClicked_SlotIndex.AddUObject(this, &UPG_UserWidgetPlayStage::OnChangeEquipWeaponIndex);
}

void UPG_UserWidgetPlayStage::InitWeaponBoxData()
{
	auto pMyPlayerState = Cast<APG_MyPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);
	ABCHECK(nullptr != pMyPlayerState);

	auto pMyPlayerData = pMyPlayerState->GetPlayingPlayerData();
	ABCHECK(nullptr != pMyPlayerState);

	auto pGameMode = Cast<APG_GameModeBase>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameMode);

	for (int32 nSlotIndex = 0; nSlotIndex < pMyPlayerData->PlayerWeaponInventory.Num(); ++nSlotIndex)
	{
		int32 nAmmo = pGameMode->GetMaxAmmo(pMyPlayerData->PlayerWeaponInventory[nSlotIndex]);
		if (nSlotIndex == PG_MAX_WEAPON_SLOT - 1)
		{
			// 유료 슬롯
			InitWeaponBoxData(pMyPlayerData->bExtendWeaponSlot, nSlotIndex, pMyPlayerData->PlayerWeaponInventory[nSlotIndex], nAmmo);
		}
		else
		{
			InitWeaponBoxData(true, nSlotIndex, pMyPlayerData->PlayerWeaponInventory[nSlotIndex], nAmmo);
		}
	}

	SetSelectWeapon(pMyPlayerData->EquipWeaponInventoryIndex);

	OnUpdateFlag();
	OnUpdateLethal();
	OnUpdateTime();
	OnChangeRewardPoint();
}

bool UPG_UserWidgetPlayStage::InitWeaponBoxData(bool bEnable, int32 nSlotIndex, int32 nWeaponTableIndex, int32 nAmmo)
{
	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage, false);

	auto pUserWidgetWeaponBox = GetUserWidgetWeaponBox(nSlotIndex);
	ABCHECK(nullptr != pUserWidgetWeaponBox, false);

	pUserWidgetWeaponBox->SetWeaponIndex(pGameModeStage, nWeaponTableIndex);
	pUserWidgetWeaponBox->SetAmmo(nAmmo);
	pUserWidgetWeaponBox->SetEnable(bEnable);

	return true;
}

void UPG_UserWidgetPlayStage::SetSelectWeapon(int32 nSlotIndex)
{
	for (int32 nIndex = 0; nIndex < eMaxWeaponSlotIndex; ++nIndex)
	{
		auto pOverlaySelectWeapon = GetOverlaySelectWeapon(nIndex);
		if (pOverlaySelectWeapon)
		{
			bool bSelect = (nIndex == nSlotIndex);
			if (bSelect)
				pOverlaySelectWeapon->SetVisibility(ESlateVisibility::HitTestInvisible);
			else
				pOverlaySelectWeapon->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UPG_UserWidgetPlayStage::OnUpdateWeaponAmmo(int32 nSlotIndex, int32 nAmmo)
{
	auto pUserWidgetWeaponBox = GetUserWidgetWeaponBox(nSlotIndex);
	ABCHECK(nullptr != pUserWidgetWeaponBox);

	pUserWidgetWeaponBox->SetAmmo(nAmmo);
}

void UPG_UserWidgetPlayStage::OnChangeEquipWeaponIndex(int32 nSlotIndex)
{
	auto pUserWidgetWeaponBox = GetUserWidgetWeaponBox(nSlotIndex);
	ABCHECK(nullptr != pUserWidgetWeaponBox);

	auto pMyPlayerState = Cast<APG_MyPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);
	ABCHECK(nullptr != pMyPlayerState);

	if (pMyPlayerState->ChangeWeapon(nSlotIndex, pUserWidgetWeaponBox->GetWeaponIndex()))
	{
		SetSelectWeapon(nSlotIndex);
	}
}

void UPG_UserWidgetPlayStage::OnUpdateFlag()
{
	ABCHECK(nullptr != WB_StageClearTarget);

	auto pGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	ABCHECK(nullptr != pGameStateStage);

	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage);

	const FPGStagePlayData* pStagePlayData = pGameStateStage->GetStagePlayData();
	ABCHECK(nullptr != pStagePlayData);
	
	WB_StageClearTarget->SetDataFlag(pGameModeStage->IsClearMission(EStageClearType::eSCT_AmountKill), 
		pStagePlayData->NumberOfMonstersKilled, 
		pStagePlayData->NumberOfMonsters);
}

void UPG_UserWidgetPlayStage::OnUpdateLethal()
{
	ABCHECK(nullptr != WB_StageClearTarget);

	auto pGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	ABCHECK(nullptr != pGameStateStage);

	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage);

	const FPGStagePlayData* pStagePlayData = pGameStateStage->GetStagePlayData();
	ABCHECK(nullptr != pStagePlayData);

	WB_StageClearTarget->SetDataLethal(pGameModeStage->IsClearMission(EStageClearType::eSCT_Lethal), 
		pStagePlayData->NumberOfMonstersMuiltiKilled, 
		pStagePlayData->NumberOfMonstersMuiltiKill);
}

void UPG_UserWidgetPlayStage::OnUpdateTime()
{
	ABCHECK(nullptr != WB_StageClearTarget);

	auto pGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	ABCHECK(nullptr != pGameStateStage);

	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage);

	const FPGStagePlayData* pStagePlayData = pGameStateStage->GetStagePlayData();
	ABCHECK(nullptr != pStagePlayData);

	WB_StageClearTarget->SetDataTime(pGameModeStage->IsClearMission(EStageClearType::eSCT_ClearTime),
		pStagePlayData->RemainTime);
}

UPG_UserWidgetWeaponBox* UPG_UserWidgetPlayStage::GetUserWidgetWeaponBox(int32 nSlotIndex)
{
	UPG_UserWidgetWeaponBox* pWeaponBox = nullptr;
	if (0 == nSlotIndex)
		pWeaponBox = WB_WeaponBox_0;
	else if (1 == nSlotIndex)
		pWeaponBox = WB_WeaponBox_1;
	else if (2 == nSlotIndex)
		pWeaponBox = WB_WeaponBox_2;

	return pWeaponBox;
}

UOverlay* UPG_UserWidgetPlayStage::GetOverlaySelectWeapon(int32 nSlotIndex)
{
	UOverlay* pOverlaySelectWeapon = nullptr;
	if (0 == nSlotIndex)
		pOverlaySelectWeapon = SelectWeapon_0;
	else if (1 == nSlotIndex)
		pOverlaySelectWeapon = SelectWeapon_1;
	else if (2 == nSlotIndex)
		pOverlaySelectWeapon = SelectWeapon_2;

	return pOverlaySelectWeapon;
}

