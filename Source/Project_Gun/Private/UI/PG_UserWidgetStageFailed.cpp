// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PG_UserWidgetStageFailed.h"
#include "Game/GameMode/PG_GameModeStage.h"
#include "Game/GameInstance/PG_GameInstance.h"

#include <Components/Overlay.h>

void UPG_UserWidgetStageFailed::NativeConstruct()
{
	Super::NativeConstruct();
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
}

bool UPG_UserWidgetStageFailed::IsAbleViewStagePlayAD()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	return pGameInstance->IsAbleADView(EADType::eAT_StagePlayAD);
}

void UPG_UserWidgetStageFailed::UpdateClearStageLayout()
{
	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage);

	bool bIsClearAmount = pGameModeStage->IsClearMission(EStageClearType::eSCT_AmountKill) || pGameModeStage->IsWasClearMisiion(EStageClearType::eSCT_AmountKill);
	Overlay_ActiveFlag->SetIsEnabled(bIsClearAmount);
	Overlay_DeActiveFlag->SetIsEnabled(!bIsClearAmount);

	bool bIsClearTime = pGameModeStage->IsClearMission(EStageClearType::eSCT_ClearTime) || pGameModeStage->IsWasClearMisiion(EStageClearType::eSCT_ClearTime);
	Overlay_ActiveTime->SetIsEnabled(bIsClearTime);
	Overlay_DeActiveTime->SetIsEnabled(!bIsClearTime);

	bool bIsClearLethal = pGameModeStage->IsClearMission(EStageClearType::eSCT_Lethal) || pGameModeStage->IsWasClearMisiion(EStageClearType::eSCT_Lethal);
	Overlay_ActiveLethal->SetIsEnabled(bIsClearLethal);
	Overlay_DeActiveLethal->SetIsEnabled(!bIsClearLethal);
}

void UPG_UserWidgetStageFailed::OnBtnRetry()
{
	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage);

	pGameModeStage->ResetStage();
}

void UPG_UserWidgetStageFailed::OnBtnGoToMainLobby()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/PG/Maps/Lobby/MainLobby"));
}

void UPG_UserWidgetStageFailed::OnADPlayStageVideoSucessed()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	pGameInstance->ResetADRemainTime(EADType::eAT_StagePlayAD);
}

void UPG_UserWidgetStageFailed::OnADPlayStageVideoClosed()
{

}
