// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerState/PG_MyPlayerState.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Define/PG_GamePlayData.h"
#include "BlueScriptObject/LethalAttack/PG_LethalAttack.h"
#include "SaveData/PG_SavePlayerData.h"
#include "Game/GameMode/PG_GameModeBase.h"
#include "Game/GameMode/PG_GameModeStage.h"
#include "Character/PG_MyChar.h"
#include "Character/PG_MonChar.h"
#include "BlueScriptObject/Weapon/PG_Weapon.h"

APG_MyPlayerState::APG_MyPlayerState()
{
}

void APG_MyPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABCHECK(InitPlayerData());
}

void APG_MyPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

bool APG_MyPlayerState::InitPlayerData()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	auto pGameMode = Cast<APG_GameModeBase>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameMode, false);

	auto pPlayerSaveData = pGameMode->GetSavePlayerData();
	ABCHECK(nullptr != pPlayerSaveData, false);

	OrignalPlayerData.RewardPoint = pPlayerSaveData->m_nRewardPoint;
	OrignalPlayerData.EquipWeaponInventoryIndex = 0;// pPlayerSaveData->m_nEquipWeaponSlotIndex;
	OrignalPlayerData.PlayerWeaponInventory = pPlayerSaveData->m_kEquipWeaponTableIndex;
	OrignalPlayerData.bExtendWeaponSlot = pPlayerSaveData->m_bExtendWeaponSlot;

	for (int32 nIndex = 0; nIndex < PG_MAX_WEAPON_SLOT; ++nIndex)
	{
		OrignalPlayerData.PlayerWeaponAmmo[nIndex] = pGameMode->GetMaxAmmo(pPlayerSaveData->m_kEquipWeaponTableIndex[nIndex]);
	}

	PlayingPlayerData = OrignalPlayerData;

	return true;
}

void APG_MyPlayerState::MonsterKill(APG_MonChar* KillMon)
{
	ABCHECK(nullptr != KillMon);
	int32 nRandIndex = FMath::RandRange(0, 100);
	int32 nDropRate = (int32)(KillMon->GetDropRate() * 100.0f);
	if (nRandIndex <= nDropRate)
	{
		float fDropRate = KillMon->GetDropRate();
		int32 GetRewardPoint = (fDropRate < 1.0f) ? 1 : (int32)fDropRate;
		AddRewardPoint(GetRewardPoint);
		OnKillAddRewardPoint.Broadcast(GetRewardPoint);
		ABLOG(Warning, TEXT("Get Reward Point : %d"), GetRewardPoint);
	}
	
	OnBasicAttackMonsterKill.Broadcast(KillMon);
}

void APG_MyPlayerState::MonsterLethalAttackKill(APG_MonChar* KillMon)
{
	ABCHECK(nullptr != KillMon);
	int32 nRandIndex = FMath::RandRange(0, 100);
	int32 nDropRate = (int32)(KillMon->GetDropRate() * 100.0f);
	if (nRandIndex <= nDropRate)
	{
		float fDropRate = KillMon->GetDropRate();
		int32 GetRewardPoint = (fDropRate < 1.0f) ? 1 : (int32)fDropRate;
		AddRewardPoint(GetRewardPoint);
		OnKillAddRewardPoint.Broadcast(GetRewardPoint);
		ABLOG(Warning, TEXT("Get Reward Point (Lethal Attack) : %d"), GetRewardPoint);
	}
}

void APG_MyPlayerState::AddRewardPoint(int32 a_nAddPoint)
{
	if (0 == a_nAddPoint)
		return;

	PlayingPlayerData.RewardPoint += a_nAddPoint;
	OnUpdateRewardPoint.Broadcast();
}

void APG_MyPlayerState::SetRewardPoint(int32 a_nSetPoint)
{
	PlayingPlayerData.RewardPoint = a_nSetPoint;
	OnUpdateRewardPoint.Broadcast();
}

bool APG_MyPlayerState::UseAmmo()
{
	int32 nEquipTableIndex = PlayingPlayerData.EquipWeaponInventoryIndex;

	if (PG_INFINITY_AMMO == PlayingPlayerData.PlayerWeaponAmmo[nEquipTableIndex])
		return true;

	if (PlayingPlayerData.PlayerWeaponAmmo[nEquipTableIndex] <= 0)
		return false;

	PlayingPlayerData.PlayerWeaponAmmo[nEquipTableIndex] -= 1;

	return true;
}

void APG_MyPlayerState::ChangeDefaultWeapon()
{
	PlayingPlayerData.EquipWeaponInventoryIndex = 0;
	ChangeWeapon(PlayingPlayerData.EquipWeaponInventoryIndex, 0);
}

bool APG_MyPlayerState::ChangeWeapon(int32 nSlotIndex, int32 nWeaponTableIndex)
{
	if (PG_INFINITY_AMMO != PlayingPlayerData.PlayerWeaponAmmo[nSlotIndex] && 0 >= PlayingPlayerData.PlayerWeaponAmmo[nSlotIndex])
	{
		return false;
	}

	PlayingPlayerData.EquipWeaponInventoryIndex = nSlotIndex;
	OnChangeWeapon.Broadcast(nWeaponTableIndex);

	return true;
}

void APG_MyPlayerState::ReadyToUseTheSkill()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Ready To Use The Skill"));
	OnReadyToUseTheLethalAttack.Broadcast();
}

void APG_MyPlayerState::TryUseTheSkill()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Try Use The Skill"));
	OnTryUseTheLethalAttack.Broadcast();
}
