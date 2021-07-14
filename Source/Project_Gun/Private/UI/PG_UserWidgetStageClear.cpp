// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PG_UserWidgetStageClear.h"
#include "Game/GameMode/PG_GameModeStage.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Game/GameState/PG_GameStateStage.h"
#include "Player/PlayerState/PG_MyPlayerState.h"

#include <Components/Overlay.h>
#include <Components/TextBlock.h>
#include <Components/Button.h>

void UPG_UserWidgetStageClear::NativeConstruct()
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

	m_pBtn_ViewAD = Cast<UButton>(GetWidgetFromName(TEXT("Btn_ViewAD")));
	ABCHECK(nullptr != m_pBtn_ViewAD);

	ABCHECK(nullptr != Text_BonusFlag);
	ABCHECK(nullptr != Text_BonusLethal);
	ABCHECK(nullptr != Text_BonusTime);
	ABCHECK(nullptr != Text_BonusStageClear);
	ABCHECK(nullptr != Text_BonusKill);
	ABCHECK(nullptr != Text_BonusStar);
	ABCHECK(nullptr != Text_CaptionBonusAD);
	ABCHECK(nullptr != Text_BonusAD);
	ABCHECK(nullptr != Text_ClearADTimer);
	ABCHECK(nullptr != Text_TotalReward);

	auto pGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	ABCHECK(nullptr != pGameStateStage);
	pGameStateStage->OnViewAD.AddUObject(this, &UPG_UserWidgetStageClear::OnViewResultAD);
}

void UPG_UserWidgetStageClear::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
	
	if (false == m_bWasViewStageClear3X)
	{
		auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
		ABCHECK(nullptr != pGameInstance);

		int32 nRemainTime = pGameInstance->GetADRemainTime(EADType::eAT_StageClear3X);
		SetBtnBonusADTime(nRemainTime);
		SetBtnEnableAD((nRemainTime <= 0));
	}
}

bool UPG_UserWidgetStageClear::IsAbleViewStagePlayAD()
{
	if (m_bWasViewStageClear3X)
	{
		// 스테이지 클리어 후 보상형 광고를 보았으면 스테이지 플레이 광고를 볼 수 없다.
		return false;
	}

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	return pGameInstance->IsAbleADView(EADType::eAT_StagePlayAD);
}

void UPG_UserWidgetStageClear::OnBtnRetry()
{
	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage);

	pGameModeStage->SavePlayerData();
	pGameModeStage->SaveStageData();
	pGameModeStage->StartGame_NextStage();
}

void UPG_UserWidgetStageClear::OnBtnGoToMainLobby()
{
	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage);

	pGameModeStage->SavePlayerData();
	pGameModeStage->SaveStageData();

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/PG/Maps/Lobby/MainLobby"));
}

void UPG_UserWidgetStageClear::OnBtnNextStage()
{
	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage);

	pGameModeStage->SavePlayerData();
	pGameModeStage->SaveStageData();
	pGameModeStage->StartGame_NextStage();
}

void UPG_UserWidgetStageClear::OnBtnViewAD()
{

}

void UPG_UserWidgetStageClear::OnADRewardVideoSucessed()
{
	m_bWasViewStageClear3X = true;

	auto pGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	ABCHECK(nullptr != pGameStateStage);

	// 보상 적용
	pGameStateStage->RequestViewAD();

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	// 타이머 리셋
	pGameInstance->ResetADRemainTime(EADType::eAT_StageClear3X);

	SetBtnEnableAD(false);
}

void UPG_UserWidgetStageClear::OnADRewardVideoClosed()
{

}

void UPG_UserWidgetStageClear::OnADPlayStageVideoSucessed()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	pGameInstance->ResetADRemainTime(EADType::eAT_StagePlayAD);
}

void UPG_UserWidgetStageClear::OnADPlayStageVideoClosed()
{

}

void UPG_UserWidgetStageClear::OnViewResultAD(bool bEnable)
{
	UpdateClearStageLayout();
}

void UPG_UserWidgetStageClear::UpdateClearStageLayout()
{
	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage);

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	auto pTableStageData = pGameInstance->GetTableStageData(pGameModeStage->GetPlayStageID());
	ABCHECK(nullptr != pTableStageData);

	// 몬스터 킬
	bool bIsClearAmount = pGameModeStage->IsClearMission(EStageClearType::eSCT_AmountKill);
	bool bIsWasClearAmount = pGameModeStage->IsWasClearMisiion(EStageClearType::eSCT_AmountKill);
	if (bIsClearAmount || bIsWasClearAmount)
	{
		Overlay_ActiveFlag->SetVisibility(ESlateVisibility::HitTestInvisible);
		Overlay_DeActiveFlag->SetVisibility(ESlateVisibility::Hidden);

		SetBonusFlag(bIsWasClearAmount, pTableStageData->StarReward);
	}
	else
	{
		Overlay_ActiveFlag->SetVisibility(ESlateVisibility::Hidden);
		Overlay_DeActiveFlag->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	// 필살기
	bool bIsClearLethal = pGameModeStage->IsClearMission(EStageClearType::eSCT_Lethal);
	bool bIsWasClearLethal = pGameModeStage->IsWasClearMisiion(EStageClearType::eSCT_Lethal);
	if (bIsClearLethal || bIsWasClearLethal)
	{
		Overlay_ActiveLethal->SetVisibility(ESlateVisibility::HitTestInvisible);
		Overlay_DeActiveLethal->SetVisibility(ESlateVisibility::Hidden);
		SetBonusLethal(bIsWasClearLethal, pTableStageData->StarReward);
	}
	else
	{
		Overlay_ActiveLethal->SetVisibility(ESlateVisibility::Hidden);
		Overlay_DeActiveLethal->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	
	// 시간 
	bool bIsClearTime = pGameModeStage->IsClearMission(EStageClearType::eSCT_ClearTime);
	bool bIsWasClearTime = pGameModeStage->IsWasClearMisiion(EStageClearType::eSCT_ClearTime);
	if (bIsClearTime || bIsWasClearTime)
	{
		Overlay_ActiveTime->SetVisibility(ESlateVisibility::HitTestInvisible);
		Overlay_DeActiveTime->SetVisibility(ESlateVisibility::Hidden);
		SetBonusTime(bIsWasClearTime, pTableStageData->StarReward);
	}
	else
	{
		Overlay_ActiveTime->SetVisibility(ESlateVisibility::Hidden);
		Overlay_DeActiveTime->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	auto pGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	ABCHECK(nullptr != pGameStateStage);
	
	SetBonusStageClear(pGameStateStage->GetClearStagePoint());
	SetBonusMonsterKill(pGameStateStage->GetClearMonsterKillPoint());
	SetBonusStar(pGameStateStage->GetClearMissionPoint());
	SetBonusAD(pGameStateStage->IsViewAD());
	SetTotalReward(pGameStateStage->GetClearTotalPoint());
}

void UPG_UserWidgetStageClear::SetBonusStageClear(int32 nStageClearBonus)
{
	ABCHECK(nullptr != Text_BonusStageClear);
	Text_BonusStageClear->SetText(FText::FromString(FString::Printf(TEXT("+%d"), nStageClearBonus)));
}

void UPG_UserWidgetStageClear::SetBonusMonsterKill(int32 nMonsterKillBonus)
{
	ABCHECK(nullptr != Text_BonusKill);
	Text_BonusKill->SetText(FText::FromString(FString::Printf(TEXT("+%d"), nMonsterKillBonus)));
}

void UPG_UserWidgetStageClear::SetBonusStar(int32 nStarBonus)
{
	ABCHECK(nullptr != Text_BonusStar);
	Text_BonusStar->SetText(FText::FromString(FString::Printf(TEXT("+%d"), nStarBonus)));
}

void UPG_UserWidgetStageClear::SetBonusAD(bool bEnable)
{
	ABCHECK(nullptr != Text_CaptionBonusAD);
	ABCHECK(nullptr != Text_BonusAD);

	if (bEnable)
	{
		Text_CaptionBonusAD->SetColorAndOpacity(FSlateColor(FLinearColor(0.000000f, 0.270706f, 1.000000f)));
		Text_BonusAD->SetColorAndOpacity(FSlateColor(FLinearColor(0.000000f, 0.270706f, 1.000000f)));
	}
	else
	{
		Text_CaptionBonusAD->SetColorAndOpacity(FSlateColor(FLinearColor(0.157986f, 0.157986f, 0.157986f)));
		Text_BonusAD->SetColorAndOpacity(FSlateColor(FLinearColor(0.157986f, 0.157986f, 0.157986f)));
	}
}

void UPG_UserWidgetStageClear::SetBtnBonusADTime(int32 nTime)
{
	ABCHECK(nullptr != Text_ClearADTimer);
	Text_ClearADTimer->SetText(FText::FromString(FString::Printf(TEXT("%d"), nTime)));
}

void UPG_UserWidgetStageClear::SetBtnEnableAD(bool bEnable)
{
	ABCHECK(nullptr != m_pBtn_ViewAD);

	if (m_pBtn_ViewAD->bIsEnabled != bEnable)
		m_pBtn_ViewAD->SetIsEnabled(bEnable);

	ABCHECK(nullptr != Text_ClearADTimer);
	if (bEnable)
		Text_ClearADTimer->SetVisibility(ESlateVisibility::Hidden);
	else
		Text_ClearADTimer->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPG_UserWidgetStageClear::SetTotalReward(int32 nTotalReward)
{
	ABCHECK(nullptr != Text_TotalReward);
	Text_TotalReward->SetText(FText::FromString(FString::Printf(TEXT("%d"), nTotalReward)));
}

void UPG_UserWidgetStageClear::SetBonusFlag(bool bWasClear, int32 nBonus)
{
	ABCHECK(nullptr != Text_BonusFlag);
	Text_BonusFlag->SetText(FText::FromString(FString::Printf(TEXT("+%d"), nBonus)));

	if (bWasClear)
		Text_BonusFlag->SetVisibility(ESlateVisibility::Hidden);
	else
		Text_BonusFlag->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPG_UserWidgetStageClear::SetBonusLethal(bool bWasClear, int32 nBonus)
{
	ABCHECK(nullptr != Text_BonusLethal);
	Text_BonusLethal->SetText(FText::FromString(FString::Printf(TEXT("+%d"), nBonus)));

	if (bWasClear)
		Text_BonusLethal->SetVisibility(ESlateVisibility::Hidden);
	else
		Text_BonusLethal->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPG_UserWidgetStageClear::SetBonusTime(bool bWasClear, int32 nBonus)
{
	ABCHECK(nullptr != Text_BonusTime);
	Text_BonusTime->SetText(FText::FromString(FString::Printf(TEXT("+%d"), nBonus)));

	if (bWasClear)
		Text_BonusTime->SetVisibility(ESlateVisibility::Hidden);
	else
		Text_BonusTime->SetVisibility(ESlateVisibility::HitTestInvisible);
}