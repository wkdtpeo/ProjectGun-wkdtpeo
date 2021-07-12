// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PG_UserWidgetPlayMissionState.h"
#include "Game/GameState/PG_GameStateStage.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Game/GameMode/PG_GameModeStage.h"

#include <Components/Button.h>
#include <Components/TextBlock.h>

void UPG_UserWidgetPlayMissionState::NativeConstruct()
{
	Super::NativeConstruct();

	PG_WIDGET_LINK(UButton, BtnFlag);
	PG_WIDGET_LINK(UButton, BtnLethal);
	PG_WIDGET_LINK(UButton, BtnTime);

	PG_WIDGET_LINK(UTextBlock, TextCurrFlag);
	PG_WIDGET_LINK(UTextBlock, TextCurrLethal);
	PG_WIDGET_LINK(UTextBlock, TextCurrTime);

	PG_WIDGET_LINK(UTextBlock, TextMaxFlag);
	PG_WIDGET_LINK(UTextBlock, TextMaxLethal);

	auto GameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	if (GameStateStage)
	{
		GameStateStage->UdpateWidgetPlayStage.AddUObject(this, &UPG_UserWidgetPlayMissionState::OnUpdateFlag);
		GameStateStage->UdpateWidgetPlayStage.AddUObject(this, &UPG_UserWidgetPlayMissionState::OnUpdateLethal);
		GameStateStage->UdpateWidgetPlayStage.AddUObject(this, &UPG_UserWidgetPlayMissionState::OnUpdateTime);
	}
}

void UPG_UserWidgetPlayMissionState::UpdateMissionFlag()
{
	auto pGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	ABCHECK(nullptr != pGameStateStage);

	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage);

	const FPGStagePlayData* pStagePlayData = pGameStateStage->GetStagePlayData();
	ABCHECK(nullptr != pStagePlayData);

	SetDataFlag(pGameModeStage->IsClearMission(EStageClearType::eSCT_AmountKill),
		pStagePlayData->NumberOfMonstersKilled,
		pStagePlayData->NumberOfMonsters);
}

void UPG_UserWidgetPlayMissionState::UpdateMissionLethal()
{
	auto pGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	ABCHECK(nullptr != pGameStateStage);

	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage);

	const FPGStagePlayData* pStagePlayData = pGameStateStage->GetStagePlayData();
	ABCHECK(nullptr != pStagePlayData);

	SetDataLethal(pGameModeStage->IsClearMission(EStageClearType::eSCT_Lethal),
		pStagePlayData->NumberOfMonstersMuiltiKilled,
		pStagePlayData->NumberOfMonstersMuiltiKill);
}

void UPG_UserWidgetPlayMissionState::UpdateMissionTime()
{
	auto pGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	ABCHECK(nullptr != pGameStateStage);

	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage);

	const FPGStagePlayData* pStagePlayData = pGameStateStage->GetStagePlayData();
	ABCHECK(nullptr != pStagePlayData);

	SetDataTime(pGameModeStage->IsClearMission(EStageClearType::eSCT_ClearTime),
		pStagePlayData->RemainTime);
}

void UPG_UserWidgetPlayMissionState::SetDataFlag(bool bClear, int32 nCurrValue, int32 nMaxValue)
{
	if (bClear)
		m_pkTextCurrFlag->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.968f, 0.6f)));
	else
		m_pkTextCurrFlag->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f)));

	m_pkBtnFlag->SetIsEnabled(!bClear);

	m_pkTextCurrFlag->SetText(FText::FromString(FString::Printf(TEXT("%d"), nCurrValue)));
	m_pkTextMaxFlag->SetText(FText::FromString(FString::Printf(TEXT("%d"), nMaxValue)));
}

void UPG_UserWidgetPlayMissionState::SetDataLethal(bool bClear, int32 nCurrValue, int32 nMaxValue)
{
	if (bClear)
		m_pkTextCurrLethal->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.968f, 0.6f)));
	else
		m_pkTextCurrLethal->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f)));

	m_pkBtnLethal->SetIsEnabled(!bClear);

	m_pkTextCurrLethal->SetText(FText::FromString(FString::Printf(TEXT("%d"), nCurrValue)));
	m_pkTextMaxLethal->SetText(FText::FromString(FString::Printf(TEXT("%d"), nMaxValue)));
}

void UPG_UserWidgetPlayMissionState::SetDataTime(bool bClear, int32 nCurrTime)
{
	if (bClear)
		m_pkTextCurrTime->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.968f, 0.6f)));
	else
		m_pkTextCurrTime->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f)));

	m_pkBtnTime->SetIsEnabled(!bClear);

	m_pkTextCurrTime->SetText(FText::FromString(FString::Printf(TEXT("%d"), nCurrTime)));
}

void UPG_UserWidgetPlayMissionState::OnUpdateFlag()
{
	UpdateMissionFlag();
}

void UPG_UserWidgetPlayMissionState::OnUpdateLethal()
{
	UpdateMissionLethal();
}

void UPG_UserWidgetPlayMissionState::OnUpdateTime()
{
	UpdateMissionTime();
}