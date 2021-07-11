// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PG_UserWidgetScreenStageClear.h"
#include "UI/PG_UserWidgetMessagePopup.h"

#include "Game/GameMode/PG_GameModeStage.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Game/GameState/PG_GameStateStage.h"

#include "Define/PG_GamePlayData.h"

#include "Player/PlayerState/PG_MyPlayerState.h"

#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/Image.h>

void UPG_UserWidgetScreenStageClear::NativeConstruct()
{
	Super::NativeConstruct();

	PG_CONNECT_WIDGET(UTextBlock, TextTitle);
	//===================================================
	// Canvas MissionState
	//===================================================
	PG_CONNECT_WIDGET(UButton, BtnFlag);
	PG_CONNECT_WIDGET(UImage, ImgIconFlagPrice);
	PG_CONNECT_WIDGET(UTextBlock, TextFlagPoint);
	PG_CONNECT_WIDGET(UButton, BtnLethal);
	PG_CONNECT_WIDGET(UImage, ImgIconLethalPrice);
	PG_CONNECT_WIDGET(UTextBlock, TextLethalPoint);
	PG_CONNECT_WIDGET(UButton, BtnTime);
	PG_CONNECT_WIDGET(UImage, ImgIconTimePrice);
	PG_CONNECT_WIDGET(UTextBlock, TextTimePoint);

	//===================================================
	// CanvasClearPointList
	//===================================================
	PG_CONNECT_WIDGET(UTextBlock, TextStageClearPoint);		
	PG_CONNECT_WIDGET(UTextBlock, TextMonsterKillPoint);		
	PG_CONNECT_WIDGET(UTextBlock, TextMissionPoint);			

	//===================================================
	// CanvasViewAD
	//===================================================
	PG_CONNECT_WIDGET(UButton, BtnADView);
	PG_CONNECT_WIDGET(UTextBlock, TextClearADTimer);

	//===================================================
	// CanvasTotalPoint
	//===================================================
	PG_CONNECT_WIDGET(UTextBlock, TextTotalPoint);

	//===================================================
	// CanvasBottomMenu
	//===================================================
	PG_CONNECT_WIDGET(UButton, BtnGoHome);
	PG_CONNECT_WIDGET(UButton, BtnRetry);
	PG_CONNECT_WIDGET(UButton, BtnNext);

	auto pGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	ABCHECK(nullptr != pGameStateStage);
	pGameStateStage->OnViewAD.AddUObject(this, &UPG_UserWidgetScreenStageClear::OnViewResultAD);

	{
		m_pUserWidgetMessagePopup = CreateWidget<UPG_UserWidgetMessagePopup>(GetWorld(), LoadClass<UPG_UserWidgetMessagePopup>(NULL, TEXT("WidgetBlueprint'/Game/PG/Blueprints/WidgetBP/WB_MessagePopup.WB_MessagePopup_C'")));
		ABCHECK(nullptr != m_pUserWidgetMessagePopup);

		m_pUserWidgetMessagePopup->OnBtnClickedOK.AddUObject(this, &UPG_UserWidgetScreenStageClear::OnMessagePopupBoxClose);
		m_pUserWidgetMessagePopup->SetVisibility(ESlateVisibility::Hidden);
		m_pUserWidgetMessagePopup->AddToViewport(999);
	}
}

void UPG_UserWidgetScreenStageClear::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
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

void UPG_UserWidgetScreenStageClear::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if (ESlateVisibility::Hidden != InVisibility)
	{
		if (PopupPlaySound)
			PlaySound(PopupPlaySound);
	}
}

bool UPG_UserWidgetScreenStageClear::IsAbleViewStagePlayAD()
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

void UPG_UserWidgetScreenStageClear::UpdateClearStageLayout()
{
	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage);

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	auto pTableStageData = pGameInstance->GetTableStageData(pGameModeStage->GetPlayStageID());
	ABCHECK(nullptr != pTableStageData);

	m_pkTextTitle->SetText(FText::FromString(pTableStageData->StageName));

	// 몬스터 킬
	bool bIsClearAmount = pGameModeStage->IsClearMission(EStageClearType::eSCT_AmountKill);
	bool bIsWasClearAmount = pGameModeStage->IsWasClearMisiion(EStageClearType::eSCT_AmountKill);
	if (bIsClearAmount || bIsWasClearAmount)
	{
		m_pkBtnFlag->SetIsEnabled(false);
		SetBonusFlag(bIsWasClearAmount, pTableStageData->StarReward);
	}
	else
	{
		m_pkBtnFlag->SetIsEnabled(true);
	}

	// 필살기
	bool bIsClearLethal = pGameModeStage->IsClearMission(EStageClearType::eSCT_Lethal);
	bool bIsWasClearLethal = pGameModeStage->IsWasClearMisiion(EStageClearType::eSCT_Lethal);
	if (bIsClearLethal || bIsWasClearLethal)
	{
		m_pkBtnLethal->SetIsEnabled(false);
		SetBonusLethal(bIsWasClearLethal, pTableStageData->StarReward);
	}
	else
	{
		m_pkBtnLethal->SetIsEnabled(true);
	}

	// 시간 
	bool bIsClearTime = pGameModeStage->IsClearMission(EStageClearType::eSCT_ClearTime);
	bool bIsWasClearTime = pGameModeStage->IsWasClearMisiion(EStageClearType::eSCT_ClearTime);
	if (bIsClearTime || bIsWasClearTime)
	{
		m_pkBtnTime->SetIsEnabled(false);
		SetBonusTime(bIsWasClearTime, pTableStageData->StarReward);
	}
	else
	{
		m_pkBtnTime->SetIsEnabled(true);
	}

	auto pGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	ABCHECK(nullptr != pGameStateStage);

	SetBonusStageClear(pGameModeStage->GetClearStagePoint());
	SetBonusMonsterKill(pGameModeStage->GetClearMonsterKillPoint());
	SetBonusMissionPoint(pGameModeStage->GetClearMissionPoint());
	//SetBonusAD(pGameStateStage->IsViewAD());
	SetTotalReward(pGameModeStage->GetClearTotalPoint());
}

void UPG_UserWidgetScreenStageClear::ShowMessageBox(FString kTitle, FString kDescription)
{
	ABCHECK(nullptr != m_pUserWidgetMessagePopup);
	m_pUserWidgetMessagePopup->SetTitle(kTitle);
	m_pUserWidgetMessagePopup->SetDescription(kDescription);
	m_pUserWidgetMessagePopup->SetVisibility(ESlateVisibility::Visible);
}

void UPG_UserWidgetScreenStageClear::SetBonusFlag(bool bWasClear, int32 nBonus)
{
	ABCHECK(nullptr != m_pkImgIconFlagPrice);
	ABCHECK(nullptr != m_pkTextFlagPoint);

	m_pkTextFlagPoint->SetText(FText::FromString(FString::Printf(TEXT("%d"), nBonus)));

	if (bWasClear)
	{
		m_pkImgIconFlagPrice->SetVisibility(ESlateVisibility::Hidden);
		m_pkTextFlagPoint->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		m_pkImgIconFlagPrice->SetVisibility(ESlateVisibility::HitTestInvisible);
		m_pkTextFlagPoint->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UPG_UserWidgetScreenStageClear::SetBonusLethal(bool bWasClear, int32 nBonus)
{
	ABCHECK(nullptr != m_pkImgIconLethalPrice);
	ABCHECK(nullptr != m_pkTextLethalPoint);

	m_pkTextLethalPoint->SetText(FText::FromString(FString::Printf(TEXT("%d"), nBonus)));

	if (bWasClear)
	{
		m_pkImgIconLethalPrice->SetVisibility(ESlateVisibility::Hidden);
		m_pkTextLethalPoint->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		m_pkImgIconLethalPrice->SetVisibility(ESlateVisibility::HitTestInvisible);
		m_pkTextLethalPoint->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UPG_UserWidgetScreenStageClear::SetBonusTime(bool bWasClear, int32 nBonus)
{
	ABCHECK(nullptr != m_pkImgIconTimePrice);
	ABCHECK(nullptr != m_pkTextTimePoint);

	m_pkTextTimePoint->SetText(FText::FromString(FString::Printf(TEXT("%d"), nBonus)));

	if (bWasClear)
	{
		m_pkImgIconTimePrice->SetVisibility(ESlateVisibility::Hidden);
		m_pkTextTimePoint->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		m_pkImgIconTimePrice->SetVisibility(ESlateVisibility::HitTestInvisible);
		m_pkTextTimePoint->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UPG_UserWidgetScreenStageClear::SetBonusStageClear(int32 nStageClearBonus)
{
	ABCHECK(nullptr != m_pkTextStageClearPoint);
	m_pkTextStageClearPoint->SetText(FText::FromString(FString::Printf(TEXT("%d"), nStageClearBonus)));
}

void UPG_UserWidgetScreenStageClear::SetBonusMonsterKill(int32 nMonsterKillBonus)
{
	ABCHECK(nullptr != m_pkTextMonsterKillPoint);
	m_pkTextMonsterKillPoint->SetText(FText::FromString(FString::Printf(TEXT("%d"), nMonsterKillBonus)));
}

void UPG_UserWidgetScreenStageClear::SetBonusMissionPoint(int32 nStarBonus)
{
	ABCHECK(nullptr != m_pkTextMissionPoint);
	m_pkTextMissionPoint->SetText(FText::FromString(FString::Printf(TEXT("%d"), nStarBonus)));
}

void UPG_UserWidgetScreenStageClear::SetBtnBonusADTime(int32 nTime)
{
	ABCHECK(nullptr != m_pkTextClearADTimer);
	m_pkTextClearADTimer->SetText(FText::FromString(FString::Printf(TEXT("%d"), nTime)));
}

void UPG_UserWidgetScreenStageClear::SetBtnEnableAD(bool bEnable)
{
	ABCHECK(nullptr != m_pkBtnADView);

	if (m_pkBtnADView->bIsEnabled != bEnable)
		m_pkBtnADView->SetIsEnabled(bEnable);

	ABCHECK(nullptr != m_pkTextClearADTimer);
	if (bEnable)
		m_pkTextClearADTimer->SetVisibility(ESlateVisibility::Hidden);
	else
		m_pkTextClearADTimer->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPG_UserWidgetScreenStageClear::SetTotalReward(int32 nTotalReward)
{
	ABCHECK(nullptr != m_pkTextTotalPoint);
	m_pkTextTotalPoint->SetText(FText::FromString(FString::Printf(TEXT("%d"), nTotalReward)));
}

void UPG_UserWidgetScreenStageClear::OnBtnADview()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	pGameInstance->ResetADRemainTime(EADType::eAT_StageClear3X);
}

void UPG_UserWidgetScreenStageClear::OnBtnGoHome()
{
	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage);

	pGameModeStage->SavePlayerData();
	pGameModeStage->SaveStageData();

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/PG/Maps/Lobby/MainLobby"));
}

void UPG_UserWidgetScreenStageClear::OnBtnRetry()
{
	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage);

	pGameModeStage->SavePlayerData();
	pGameModeStage->SaveStageData();
	pGameModeStage->ResetStage();
}

void UPG_UserWidgetScreenStageClear::OnBtnNext()
{
	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage);

	pGameModeStage->SavePlayerData();
	pGameModeStage->SaveStageData();
	pGameModeStage->NextStage();
}

void UPG_UserWidgetScreenStageClear::OnADRewardVideoSucessed()
{
	m_bWasViewStageClear3X = true;

	auto pGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	ABCHECK(nullptr != pGameStateStage);

	// 보상 적용
	pGameStateStage->RequestViewAD();

	SetBtnEnableAD(false);
}

void UPG_UserWidgetScreenStageClear::OnADRewardVideoError()
{
	ShowMessageBox(TEXT("Error"), TEXT("Failed to get the reward.\nPlease try again later."));
}

void UPG_UserWidgetScreenStageClear::OnADPlayStageVideoSucessed()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	pGameInstance->ResetADRemainTime(EADType::eAT_StagePlayAD);
}

void UPG_UserWidgetScreenStageClear::OnADPlayStageVideoClosed()
{

}

void UPG_UserWidgetScreenStageClear::OnViewResultAD(bool bEnable)
{
	UpdateClearStageLayout();
}

void UPG_UserWidgetScreenStageClear::OnMessagePopupBoxClose()
{
	ABCHECK(nullptr != m_pUserWidgetMessagePopup);
	m_pUserWidgetMessagePopup->SetVisibility(ESlateVisibility::Hidden);
}