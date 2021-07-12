// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerController/PG_PlayerController.h"
#include "Player/PlayerState/PG_MyPlayerState.h"
#include "Character/PG_MonChar.h"
#include "Game/GameState/PG_GameStateStage.h"
#include "Game/GameMode/PG_GameModeStage.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "UI/PG_UserWidgetScreenPlayStage.h"
#include "UI/PG_UserWidgetScreenStageFailed.h"
#include "UI/PG_UserWidgetScreenStageClear.h"
#include "UI/PG_VirtualJoystick.h"

#include <GameFramework/TouchInterface.h>
#include <GameFramework/InputSettings.h>
#include <Widgets/Input/SVirtualJoystick.h>

APG_PlayerController::APG_PlayerController()
{
}

void APG_PlayerController::CreateTouchInterface()
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);

	// do we want to show virtual joysticks?
	if (LocalPlayer && LocalPlayer->ViewportClient && SPG_VirtualJoystick::ShouldDisplayTouchInterface())
	{
		// in case we already had one, remove it
		if (VirtualJoystick.IsValid())
		{
			Cast<ULocalPlayer>(Player)->ViewportClient->RemoveViewportWidgetContent(VirtualJoystick.ToSharedRef());
		}

		if (CurrentTouchInterface == nullptr)
		{
			// load what the game wants to show at startup
			FSoftObjectPath DefaultTouchInterfaceName = GetDefault<UInputSettings>()->DefaultTouchInterface;

			if (DefaultTouchInterfaceName.IsValid())
			{
				// activate this interface if we have it
				CurrentTouchInterface = LoadObject<UTouchInterface>(NULL, *DefaultTouchInterfaceName.ToString());
			}
		}

		if (CurrentTouchInterface)
		{
			// create the joystick 
			TSharedPtr<SPG_VirtualJoystick> CustomJoystick = SNew(SPG_VirtualJoystick);
			CustomJoystick->BindPlayerController(this);

			VirtualJoystick = CustomJoystick;

			// add it to the player's viewport
			LocalPlayer->ViewportClient->AddViewportWidgetContent(CustomJoystick.ToSharedRef());

			ActivateTouchInterface(CurrentTouchInterface);
		}
	}
}

void APG_PlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APG_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void APG_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	//UserWidgetPlayStage = CreateWidget<UPG_UserWidgetPlayStage>(this, LoadClass<UPG_UserWidgetPlayStage>(NULL, TEXT("WidgetBlueprint'/Game/PG/Blueprints/WidgetBP/WB_PlayStage.WB_PlayStage_C'")));
	//ABCHECK(nullptr != UserWidgetPlayStage);
	UserWidgetScreenPlayStage = CreateWidget<UPG_UserWidgetScreenPlayStage>(this, LoadClass<UPG_UserWidgetScreenPlayStage>(NULL, TEXT("WidgetBlueprint'/Game/PG/Blueprints/WidgetBP/Screen/WB_ScreenPlayStage.WB_ScreenPlayStage_C'")));
	ABCHECK(nullptr != UserWidgetScreenPlayStage);
	WidgetStageClear = CreateWidget<UPG_UserWidgetScreenStageClear>(this, LoadClass<UPG_UserWidgetScreenStageClear>(NULL, TEXT("WidgetBlueprint'/Game/PG/Blueprints/WidgetBP/Screen/WB_ScreenStageClear.WB_ScreenStageClear_C'")));
	ABCHECK(nullptr != WidgetStageClear);
	UserWidgetScreenStageFailed = CreateWidget<UPG_UserWidgetScreenStageFailed>(this, LoadClass<UPG_UserWidgetScreenStageFailed>(NULL, TEXT("WidgetBlueprint'/Game/PG/Blueprints/WidgetBP/Screen/WB_ScreenStageFailed.WB_ScreenStageFailed_C'")));
	ABCHECK(nullptr != UserWidgetScreenStageFailed);

	auto GameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	if (GameStateStage)
	{
		GameStateStage->StageClearDelegate.AddUObject(this, &APG_PlayerController::OnStateClear);
		GameStateStage->StageFailedDelegate.AddUObject(this, &APG_PlayerController::OnStateFailed);
	}

	UserWidgetScreenPlayStage->AddToViewport();
	UserWidgetScreenPlayStage->Init();
}

void APG_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void APG_PlayerController::MonsterKill(class APG_MonChar* KillMon)
{
	ABCHECK(nullptr != KillMon);

	auto MyPlayerState = GetPlayerState<APG_MyPlayerState>();
	ABCHECK(nullptr != MyPlayerState);
	MyPlayerState->MonsterKill(KillMon);
}

void APG_PlayerController::MonsterLethalAttackKill(class APG_MonChar* KillMon)
{
	ABCHECK(nullptr != KillMon);

	auto MyPlayerState = GetPlayerState<APG_MyPlayerState>();
	ABCHECK(nullptr != MyPlayerState);

	MyPlayerState->MonsterLethalAttackKill(KillMon);
}

void APG_PlayerController::SetLockJoystickInput(bool a_bLock)
{
	if (a_bLock)
		ActivateTouchInterface(nullptr);
	else
		CreateTouchInterface();
}

void APG_PlayerController::OnStateClear()
{
	SetLockJoystickInput(true);
	//SetPause(true);

	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage);

	auto pPlayerState = GetPlayerState<APG_MyPlayerState>();
	ABCHECK(nullptr != pPlayerState);

	// 스테이지 클리어 보상 획득
	int32 nRewardPoint = pPlayerState->GetOriginalPlayerData()->RewardPoint;
	pPlayerState->SetRewardPoint(nRewardPoint + pGameModeStage->GetClearTotalPoint());

	ABCHECK(nullptr != WidgetStageClear);
	WidgetStageClear->AddToViewport();
	WidgetStageClear->SetVisibility(ESlateVisibility::Visible);
	WidgetStageClear->UpdateClearStageLayout();
}

void APG_PlayerController::OnStateFailed()
{
	SetLockJoystickInput(true);
	SetPause(true);

	auto GameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != GameInstance);

	//GameInstance->ResetStage();
	ABCHECK(nullptr != UserWidgetScreenStageFailed);
	UserWidgetScreenStageFailed->AddToViewport();
	UserWidgetScreenStageFailed->SetVisibility(ESlateVisibility::Visible);
	UserWidgetScreenStageFailed->UpdateLayout();
}

void APG_PlayerController::OnFire()
{
	auto pPlayerState = GetPlayerState<APG_MyPlayerState>();
	ABCHECK(nullptr != pPlayerState);
	ABCHECK(nullptr != UserWidgetScreenPlayStage)

	if (false == pPlayerState->UseAmmo())
	{
		// 기본 무기로 바꾼다.
		pPlayerState->ChangeDefaultWeapon();
		UserWidgetScreenPlayStage->OnChangeWeapon();
	}

	UserWidgetScreenPlayStage->OnFire();
}

void APG_PlayerController::OnTryLethalAttack()
{
	auto pPlayerState = GetPlayerState<APG_MyPlayerState>();
	ABCHECK(nullptr != pPlayerState);
	pPlayerState->TryUseTheSkill();
}