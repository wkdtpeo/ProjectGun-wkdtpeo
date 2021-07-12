// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PG_UserWidgetScreenPlayStage.h"
#include "UI/PG_UserWidgetPlayMissionState.h"
#include "UI/PG_UserWidgetPlayStageWeaponBag.h"

#include "Game/GameState/PG_GameStateStage.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Game/GameMode/PG_GameModeStage.h"

#include "Player/PlayerState/PG_MyPlayerState.h"

#include "Character/PG_MonChar.h"

void UPG_UserWidgetScreenPlayStage::NativeConstruct()
{
	Super::NativeConstruct();

	PG_WIDGET_LINK(UPG_UserWidgetPlayMissionState, WB_MissionState);
	PG_WIDGET_LINK(UPG_UserWidgetPlayStageWeaponBag, WB_PlayStageWeaponBag);

	auto MyPlayerState = Cast<APG_MyPlayerState>(GetOwningPlayerState());
	if (MyPlayerState)
	{
		MyPlayerState->OnUpdateRewardPoint.AddUObject(this, &UPG_UserWidgetScreenPlayStage::OnChangeRewardPoint);
	}
}

void UPG_UserWidgetScreenPlayStage::Init()
{
	InitPlayStageWeaponBag();
	InitPlayMissionState();

	OnChangeRewardPoint();
}

void UPG_UserWidgetScreenPlayStage::InitPlayStageWeaponBag()
{
	ABCHECK(nullptr != m_pkWB_PlayStageWeaponBag);

	auto pMyPlayerState = Cast<APG_MyPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);
	ABCHECK(nullptr != pMyPlayerState);

	auto pMyPlayerData = pMyPlayerState->GetPlayingPlayerData();
	ABCHECK(nullptr != pMyPlayerState);

	auto pGameMode = Cast<APG_GameModeBase>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameMode);

	int nSlotIndex = 0;
	for (auto WeaponIndex : pMyPlayerData->PlayerWeaponInventory)
	{
		int nAmmo = pGameMode->GetMaxAmmo(WeaponIndex);

		m_pkWB_PlayStageWeaponBag->SetSlotWaponIndex(nSlotIndex, WeaponIndex);
		m_pkWB_PlayStageWeaponBag->SetSlotWeaponAmmo(nSlotIndex, nAmmo);

		// 유료 슬롯
		if (nSlotIndex == PG_MAX_WEAPON_SLOT - 1)
			m_pkWB_PlayStageWeaponBag->SetEnableSlot(nSlotIndex, pMyPlayerData->bExtendWeaponSlot);
		else
			m_pkWB_PlayStageWeaponBag->SetEnableSlot(nSlotIndex, true);

		nSlotIndex++;
	}

	UpdateWeaponSlotLightBox();
}

void UPG_UserWidgetScreenPlayStage::InitPlayMissionState()
{
	ABCHECK(nullptr != m_pkWB_MissionState);

	m_pkWB_MissionState->UpdateMissionFlag();
	m_pkWB_MissionState->UpdateMissionLethal();
	m_pkWB_MissionState->UpdateMissionTime();
}

void UPG_UserWidgetScreenPlayStage::UpdateWeaponSlotLightBox()
{
	ABCHECK(nullptr != m_pkWB_PlayStageWeaponBag);

	auto pMyPlayerState = Cast<APG_MyPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);
	ABCHECK(nullptr != pMyPlayerState);

	int32 nSlotIndex = pMyPlayerState->GetPlayingPlayerData()->EquipWeaponInventoryIndex;
	m_pkWB_PlayStageWeaponBag->SetEnableLightBox(nSlotIndex);
}

void UPG_UserWidgetScreenPlayStage::OnFire()
{
	ABCHECK(nullptr != m_pkWB_PlayStageWeaponBag);

	auto pMyPlayerState = Cast<APG_MyPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);
	ABCHECK(nullptr != pMyPlayerState);

	int32 nSlotIndex = pMyPlayerState->GetPlayingPlayerData()->EquipWeaponInventoryIndex;
	int32 nAmmo = pMyPlayerState->GetPlayingPlayerData()->PlayerWeaponAmmo[nSlotIndex];

	m_pkWB_PlayStageWeaponBag->SetSlotWeaponAmmo(nSlotIndex, nAmmo);

}

void UPG_UserWidgetScreenPlayStage::OnChangeWeapon()
{
	UpdateWeaponSlotLightBox();
}