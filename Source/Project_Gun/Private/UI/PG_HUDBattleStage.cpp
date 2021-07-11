// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PG_HUDBattleStage.h"
#include "Game/GameState/PG_GameStateStage.h"
#include "Player/PlayerState/PG_MyPlayerState.h"
#include <Components/TextBlock.h>
#include <Components/Button.h>



void UPG_HUDBattleStage::NativeConstruct()
{
	Super::NativeConstruct();

	StageTimer = Cast<UTextBlock>(GetWidgetFromName(TEXT("StageTimer")));
	ABCHECK(nullptr != StageTimer);

	RewardPoint = Cast<UTextBlock>(GetWidgetFromName(TEXT("RewardPoint")));
	ABCHECK(nullptr != RewardPoint);

	LethalAttack = Cast<UTextBlock>(GetWidgetFromName(TEXT("LethalAttack")));
	ABCHECK(nullptr != LethalAttack);

	Btn_Rifle = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Rifle")));
	ABCHECK(nullptr != Btn_Rifle);

	Btn_Flame = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Flame")));
	ABCHECK(nullptr != Btn_Flame);

	Btn_Shutgun = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Shutgun")));
	ABCHECK(nullptr != Btn_Shutgun);

	Btn_Laser = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Laser")));
	ABCHECK(nullptr != Btn_Laser);

	Btn_Grandle = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Grandle")));
	ABCHECK(nullptr != Btn_Grandle);
}

void UPG_HUDBattleStage::BindPlayerState(APG_MyPlayerState* APlayerState)
{
	ABCHECK(nullptr != APlayerState);

	APlayerState->OnTryUseTheLethalAttack.AddUObject(this, &UPG_HUDBattleStage::UpdateLethalAttack);
	APlayerState->OnUseTheLethalAttackFail.AddUObject(this, &UPG_HUDBattleStage::UpdateLethalAttack);
	//APlayerState->OnAddRewardPoint.AddUObject(this, &UPG_HUDBattleStage::UpdateRewardPoint);

	auto GameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	if (GameStateStage)
		GameStateStage->StageTimerDelegate.AddUObject(this, &UPG_HUDBattleStage::UpdateStageTimer);


	Btn_Rifle->OnPressed.AddDynamic(this, &UPG_HUDBattleStage::OnBtnRifle);
	Btn_Flame->OnPressed.AddDynamic(this, &UPG_HUDBattleStage::OnBtnFlame);
	Btn_Shutgun->OnPressed.AddDynamic(this, &UPG_HUDBattleStage::OnBtnShutgun);
	Btn_Laser->OnPressed.AddDynamic(this, &UPG_HUDBattleStage::OnBtnLaser);
	Btn_Grandle->OnPressed.AddDynamic(this, &UPG_HUDBattleStage::OnBtnGrandle);
}

void UPG_HUDBattleStage::UpdateStageTimer()
{
	ABCHECK(nullptr != StageTimer);

	auto GameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	ABCHECK(nullptr != GameStateStage);

	FString TextTimer = FString::Printf(TEXT("Time : %d"), GameStateStage->GetRemainStageTimer());
	StageTimer->SetText(FText::FromString(TextTimer));
}

void UPG_HUDBattleStage::UpdateRewardPoint()
{
	ABCHECK(nullptr != RewardPoint);

	auto MyPlayerState = Cast<APG_MyPlayerState>(GetOwningPlayerState());
	if (MyPlayerState)
	{
		FString TextRewardPoint = FString::Printf(TEXT("RP : %d"), MyPlayerState->GetRewardPoint());
		RewardPoint->SetText(FText::FromString(TextRewardPoint));
	}
}

void UPG_HUDBattleStage::UpdateLethalAttack()
{
	ABCHECK(nullptr != LethalAttack);

	auto pGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	if (pGameStateStage)
	{
		FString TextLethalAttack = FString::Printf(TEXT("Lethal Kill : %d"), pGameStateStage->GetMaxLethalKill());
		LethalAttack->SetText(FText::FromString(TextLethalAttack));
	}
}

void UPG_HUDBattleStage::OnBtnRifle()
{
	//auto MyPlayerState = Cast<APG_MyPlayerState>(GetOwningPlayerState());
	//if (MyPlayerState)
	//	MyPlayerState->ChangeWeapon(LoadClass<APG_Weapon>(NULL, TEXT("Blueprint'/Game/PG/Blueprints/Weapon/BP_Weapon_Rifle.BP_Weapon_Rifle_C'")));
}

void UPG_HUDBattleStage::OnBtnFlame()
{
	//auto MyPlayerState = Cast<APG_MyPlayerState>(GetOwningPlayerState());
	//if (MyPlayerState)
	//	MyPlayerState->ChangeWeapon(LoadClass<APG_Weapon>(NULL, TEXT("Blueprint'/Game/PG/Blueprints/Weapon/BP_Weapon_FlameGun.BP_Weapon_FlameGun_C'")));
}

void UPG_HUDBattleStage::OnBtnShutgun()
{
	//auto MyPlayerState = Cast<APG_MyPlayerState>(GetOwningPlayerState());
	//if (MyPlayerState)
	//	MyPlayerState->ChangeWeapon(LoadClass<APG_Weapon>(NULL, TEXT("Blueprint'/Game/PG/Blueprints/Weapon/BP_Weapon_ShotGun.BP_Weapon_ShotGun_C'")));
}

void UPG_HUDBattleStage::OnBtnLaser()
{
	//auto MyPlayerState = Cast<APG_MyPlayerState>(GetOwningPlayerState());
	//if (MyPlayerState)
	//	MyPlayerState->ChangeWeapon(LoadClass<APG_Weapon>(NULL, TEXT("Blueprint'/Game/PG/Blueprints/Weapon/BP_Weapon_LaserBemGun.BP_Weapon_LaserBemGun_C'")));
}

void UPG_HUDBattleStage::OnBtnGrandle()
{
	//auto MyPlayerState = Cast<APG_MyPlayerState>(GetOwningPlayerState());
	//if (MyPlayerState)
	//	MyPlayerState->ChangeWeapon(LoadClass<APG_Weapon>(NULL, TEXT("Blueprint'/Game/PG/Blueprints/Weapon/BP_Weapon_GraenadeLauncher.BP_Weapon_GraenadeLauncher_C'")));
}