// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PG_UserWidgetPlayStageWeaponBag.h"
#include "UI/PG_UserWidgetWeaponBox.h"

#include "Player/PlayerState/PG_MyPlayerState.h"

#include "Define/PG_GamePlayData.h"

void UPG_UserWidgetPlayStageWeaponBag::NativeConstruct()
{
	Super::NativeConstruct();

	PG_CONNECT_WIDGET_TArray(UPG_UserWidgetWeaponBox, WB_WeaponSlot, eMaxWeaponSlot);

	for (auto WeaponSlot : m_pkWB_WeaponSlot)
	{
		if (WeaponSlot)
			WeaponSlot->OnBtnWeaponClicked_SlotIndex.AddUObject(this, &UPG_UserWidgetPlayStageWeaponBag::OnBtnWeaponSlotClicked);
	}
}

bool UPG_UserWidgetPlayStageWeaponBag::SetSlotWaponIndex(int32 nSlotIndex, int32 nWeaponIndex)
{
	ABCHECK(nSlotIndex < m_pkWB_WeaponSlot.Num(), false);
	ABCHECK(nullptr != m_pkWB_WeaponSlot[nSlotIndex], false);

	auto pGameModeBase = Cast<APG_GameModeBase>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeBase, false);

	m_pkWB_WeaponSlot[nSlotIndex]->SetSlotIndex(nSlotIndex);
	m_pkWB_WeaponSlot[nSlotIndex]->SetWeaponIndex(pGameModeBase, nWeaponIndex);

	return true;
}

bool UPG_UserWidgetPlayStageWeaponBag::SetSlotWeaponAmmo(int32 nSlotIndex, int32 nAmmo)
{
	ABCHECK(nSlotIndex < m_pkWB_WeaponSlot.Num(), false);
	ABCHECK(nullptr != m_pkWB_WeaponSlot[nSlotIndex], false);

	m_pkWB_WeaponSlot[nSlotIndex]->SetAmmo(nAmmo);

	return true;
}

bool UPG_UserWidgetPlayStageWeaponBag::SetEnableSlot(int32 nSlotIndex, bool bEnable)
{
	ABCHECK(nSlotIndex < m_pkWB_WeaponSlot.Num(), false);
	ABCHECK(nullptr != m_pkWB_WeaponSlot[nSlotIndex], false);

	m_pkWB_WeaponSlot[nSlotIndex]->SetEnable(bEnable);

	return true;
}

void UPG_UserWidgetPlayStageWeaponBag::SetEnableLightBox(int32 nSlotIndex)
{
	int nIndex = 0;
	for (auto WeaponSlot : m_pkWB_WeaponSlot)
	{
		if (WeaponSlot)
		{
			bool bEnable = (nIndex == nSlotIndex);
			WeaponSlot->SetEnableLightBox(bEnable);
		}

		nIndex++;
	}
}

void UPG_UserWidgetPlayStageWeaponBag::OnBtnWeaponSlotClicked(int32 nSlotIndex)
{
	ABCHECK(nSlotIndex < m_pkWB_WeaponSlot.Num());

	auto pMyPlayerState = Cast<APG_MyPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);
	ABCHECK(nullptr != pMyPlayerState);

	// 무기 변경
	ABCHECK(pMyPlayerState->ChangeWeapon(nSlotIndex, m_pkWB_WeaponSlot[nSlotIndex]->GetWeaponIndex()));

	SetEnableLightBox(nSlotIndex);
}