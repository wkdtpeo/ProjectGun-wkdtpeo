﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/GameMode/PG_GameModeMainLobby.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "SaveData/PG_SaveStageData.h"
#include "SaveData/PG_SavePlayerData.h"
#include "Player/PlayerState/PG_MyPlayerState.h"

APG_GameModeMainLobby::APG_GameModeMainLobby()
{
	PlayerStateClass = APG_MyPlayerState::StaticClass();
}

void APG_GameModeMainLobby::BeginPlay()
{
	Super::BeginPlay();
}

bool APG_GameModeMainLobby::IsClearMission(enum EStageClearType a_ekStageClearType)
{
	ABCHECK(nullptr != m_pSelectSaveStageData, false);

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	const FPGStageData*	cpStageData = pGameInstance->GetTableStageData(m_pSelectSaveStageData->m_nStageID);
	ABCHECK(nullptr != cpStageData, false);

	switch (a_ekStageClearType)
	{
	case EStageClearType::eSCT_AmountKill:
		return m_pSelectSaveStageData->m_bWasClearAmount;
	case EStageClearType::eSCT_ClearTime:
		return m_pSelectSaveStageData->m_bWasClearTime;
	case EStageClearType::eSCT_Lethal:
		return m_pSelectSaveStageData->m_bWasClearLethal;
	}

	return false;
}

void APG_GameModeMainLobby::SaveWeaponEquipSlot(int32 nSlotIndex, int32 nWeaponIndex)
{
	ABCHECK(nullptr != m_pSavePlayerData);
	m_pSavePlayerData->m_kEquipWeaponTableIndex[nSlotIndex] = nWeaponIndex;

	ABCHECK(SavePlayerData());
}