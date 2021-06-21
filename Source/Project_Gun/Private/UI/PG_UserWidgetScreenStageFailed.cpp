// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PG_UserWidgetScreenStageFailed.h"

#include "Game/GameMode/PG_GameModeStage.h"
#include "Game/GameInstance/PG_GameInstance.h"

#include <Components/TextBlock.h>
#include <Components/Button.h>

void UPG_UserWidgetScreenStageFailed::NativeConstruct()
{
	Super::NativeConstruct();

	// Popup Title
	PG_CONNECT_WIDGET(UTextBlock, TextTitle);

	// Mission State
	PG_CONNECT_WIDGET(UButton, BtnFlag);
	PG_CONNECT_WIDGET(UButton, BtnLethal);
	PG_CONNECT_WIDGET(UButton, BtnTime);

	// Menu Button
	PG_CONNECT_WIDGET(UButton, BtnHome);
	PG_CONNECT_WIDGET(UButton, BtnRetry);
}

void UPG_UserWidgetScreenStageFailed::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if (ESlateVisibility::Hidden != InVisibility)
	{
		if (PopupPlaySound)
			PlaySound(PopupPlaySound);
	}
}

bool UPG_UserWidgetScreenStageFailed::IsAbleViewStagePlayAD()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	return pGameInstance->IsAbleADView(EADType::eAT_StagePlayAD);
}

void UPG_UserWidgetScreenStageFailed::UpdateLayout()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage);

	int32 nSelectedStageID = pGameInstance->GetPlayStageID();
	const FPGStageData* cpStageData = pGameInstance->GetTableStageData(nSelectedStageID);
	ABCHECK(nullptr != cpStageData);

	SetPopupTitle(cpStageData->StageName);

	bool bIsClearAmount = pGameModeStage->IsClearMission(EStageClearType::eSCT_AmountKill) || pGameModeStage->IsWasClearMisiion(EStageClearType::eSCT_AmountKill);
	m_pkBtnFlag->SetIsEnabled(bIsClearAmount);
	bool bIsClearTime = pGameModeStage->IsClearMission(EStageClearType::eSCT_ClearTime) || pGameModeStage->IsWasClearMisiion(EStageClearType::eSCT_ClearTime);
	m_pkBtnTime->SetIsEnabled(bIsClearTime);
	bool bIsClearLethal = pGameModeStage->IsClearMission(EStageClearType::eSCT_Lethal) || pGameModeStage->IsWasClearMisiion(EStageClearType::eSCT_Lethal);
	m_pkBtnLethal->SetIsEnabled(bIsClearLethal);
}

void UPG_UserWidgetScreenStageFailed::SetPopupTitle(FString kTitle)
{
	ABCHECK(nullptr != m_pkTextTitle);
	m_pkTextTitle->SetText(FText::FromString(kTitle));
}

void UPG_UserWidgetScreenStageFailed::OnBtnHome()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/PG/Maps/Lobby/MainLobby"));
}

void UPG_UserWidgetScreenStageFailed::OnBtnRetry()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	pGameInstance->ResetStage();
}

void UPG_UserWidgetScreenStageFailed::OnADPlayStageVideoSucessed()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	pGameInstance->ResetADRemainTime(EADType::eAT_StagePlayAD);
}

void UPG_UserWidgetScreenStageFailed::OnADPlayStageVideoClosed()
{

}