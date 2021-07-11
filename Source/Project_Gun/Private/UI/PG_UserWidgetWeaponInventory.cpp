// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PG_UserWidgetWeaponInventory.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Define/PG_TableData.h"
#include "UI/PG_UserWidgetWeaponBox.h"
#include <Components/Button.h>

void UPG_UserWidgetWeaponInventory::NativeConstruct()
{
	Super::NativeConstruct();

	for (int32 nIndex = 0; nIndex < eMaxInvenIndex; ++nIndex)
	{
		FString BtnWidgetName = FString::Printf(TEXT("WB_WeaponBox_%d"), nIndex);
		auto pBtnWeapon = Cast<UPG_UserWidgetWeaponBox>(GetWidgetFromName(*BtnWidgetName));
		if (nullptr == pBtnWeapon)
		{
			ABLOG(Error, TEXT("Can't find widget %s"), *BtnWidgetName);
			continue;
		}

		pBtnWeapon->OnBtnWeaponClicked_WeaponIndex.AddUObject(this, &UPG_UserWidgetWeaponInventory::OnSelectWeaponIndex);
	}
}

void UPG_UserWidgetWeaponInventory::UpdateWeaponInventory()
{
	auto pGameModeBase = Cast<APG_GameModeBase>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeBase);

	for (int32 nIndex = 0; nIndex < eMaxInvenIndex; ++nIndex)
	{
		FString BtnWidgetName = FString::Printf(TEXT("WB_WeaponBox_%d"), nIndex);
		auto pBtnWeapon = Cast<UPG_UserWidgetWeaponBox>(GetWidgetFromName(*BtnWidgetName));
		if (nullptr == pBtnWeapon)
		{
			ABLOG(Error, TEXT("Can't find widget %s"), *BtnWidgetName);
			continue;
		}

		pBtnWeapon->SetWeaponIndex(pGameModeBase, nIndex);
	}
}

void UPG_UserWidgetWeaponInventory::SetOpenSlotIndex(int32 nSlotIndex)
{
	m_nOpenSlotIndex = nSlotIndex;
}

void UPG_UserWidgetWeaponInventory::OnBtnClose()
{
	OnBtnClickedClose.Broadcast(m_nOpenSlotIndex, PG_INVALID_INDEX);
	m_nOpenSlotIndex = PG_INVALID_INDEX;
}

void UPG_UserWidgetWeaponInventory::OnSelectWeaponIndex(int32 nWeaponIndex)
{
	OnBtnClickedClose.Broadcast(m_nOpenSlotIndex, nWeaponIndex);
	m_nOpenSlotIndex = PG_INVALID_INDEX;
}