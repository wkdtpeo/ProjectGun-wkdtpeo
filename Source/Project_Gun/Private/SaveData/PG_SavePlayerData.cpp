// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveData/PG_SavePlayerData.h"



UPG_SavePlayerData::UPG_SavePlayerData()
{
	m_nLastPlayStageID = 0;
	m_nRewardPoint = 0;
	m_bExtendWeaponSlot = false;
	//m_nEquipWeaponSlotIndex = 0;
	//m_kEquipWeaponClass = LoadClass<APG_Weapon>(NULL, TEXT("Blueprint'/Game/PG/Blueprints/Weapon/BP_Weapon_Rifle.BP_Weapon_Rifle_C'"));

	m_kEquipWeaponTableIndex.Init(0, PG_MAX_WEAPON_SLOT);

	// 기본 무기 Unlock
	FPGWeaponOwnData kDefalutWeaponOwnData;
	kDefalutWeaponOwnData.IsUnlock = true;
	WeaponOwnData.Add(0, kDefalutWeaponOwnData);
}


FString UPG_SavePlayerData::GetSlotName()
{
	return TEXT("PlayerSaveData");
}

int32 UPG_SavePlayerData::GetPlayerIndex()
{
	return 0;
}