﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/GameState/PG_GameStateStage.h"
#include "Game/GameMode/PG_GameModeStage.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Character/PG_MonChar.h"
#include "Character/PG_MonCharDivision.h"
#include "Player/PlayerState/PG_MyPlayerState.h"
#include "Player/PlayerController/PG_PlayerController.h"

#include <EngineUtils.h>

APG_GameStateStage::APG_GameStateStage()
{

}

void APG_GameStateStage::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APG_GameStateStage::BeginPlay()
{
	Super::BeginPlay();

	if (false == InitStageData())
	{
		ABLOG(Error, TEXT("Init Stage Data Failed!!"));
	}

	auto pPlayerState = Cast<APG_MyPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);
	ABCHECK(nullptr != pPlayerState);
	pPlayerState->OnKillAddRewardPoint.AddUObject(this, &APG_GameStateStage::OnKillAddRewardPoint);

	// 스테이지 타이머 가동
	GetWorldTimerManager().SetTimer(StageTimerHandle, this, &APG_GameStateStage::OnStageTimerTick, 1.0f, true, 1.0f);
}

bool APG_GameStateStage::InitStageData()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	auto pGameModeBase = Cast<APG_GameModeBase>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeBase, false);

	// Get Stage Data
	int32 PlayStageID = pGameModeBase->GetPlayStageID();
	StageData = pGameInstance->GetTableStageData(PlayStageID);
	ABCHECK(nullptr != StageData, false);

	// Set Kill Z
	GetWorldSettings()->KillZ = -500.0f;

	// 레벨에 설치된 몬스터 수 저장
	StagePlayData.NumberOfMonsters = GetLevelInstallMonsterCount();
	StagePlayData.NumberOfMonstersMuiltiKill = StageData->Multi_Kill;
	StagePlayData.RemainTime = StageData->Time;
	StagePlayData.PlayTime = 0;

	ABLOG(Warning, TEXT("[ Stage Information ] ----------------------------------------"));
	ABLOG(Warning, TEXT("Stage ID : %d"), PlayStageID);
	ABLOG(Warning, TEXT("Stage Name : %s"), *StageData->StageName);
	ABLOG(Warning, TEXT("M_Amount : %d"), StageData->M_Amount);
	ABLOG(Warning, TEXT("Time : %d"), StageData->Time);
	ABLOG(Warning, TEXT("Multi_Kill : %d"), StageData->Multi_Kill);
	ABLOG(Warning, TEXT("StageClearReward : %d"), StageData->StageClearReward);
	ABLOG(Warning, TEXT("StarReward : %d"), StageData->StarReward);
	ABLOG(Warning, TEXT("Number Of Monsters : %d"), StagePlayData.NumberOfMonsters);
	ABLOG(Warning, TEXT("-------------------------------------------------------------"));

	return CheckStageData();
}

void APG_GameStateStage::MonsterDie(APG_MonChar* KillMon, bool IsLethalAttack)
{
	auto pDivisionMonster = Cast<APG_MonCharDivision>(KillMon);
	if (pDivisionMonster && pDivisionMonster->IsDivisionChild())
	{
		// 분화형 몬스터의 자식이 죽었을 때는 몬스터 킬수 카운트에 포함하지 않는다. (본체만 카운트 적용)
		return;
	}

	ABCHECK(nullptr != KillMon);
	StagePlayData.NumberOfMonstersKilled += 1;

	auto pMyPlayerController = Cast<APG_PlayerController>(GetWorld()->GetFirstPlayerController());
	ABCHECK(nullptr != pMyPlayerController);

	auto pMyPlayerState = pMyPlayerController->GetPlayerState<APG_MyPlayerState>();
	ABCHECK(nullptr != pMyPlayerState);

	if (IsLethalAttack)
		pMyPlayerState->MonsterLethalAttackKill(KillMon);		// 필살기로 죽인 몬스터
	else
		pMyPlayerState->MonsterKill(KillMon);					// 일반 공격으로 죽인 몬스터

	UdpateWidgetPlayStage.Broadcast();

	// 스테이지 클리어 조건을 만족하면 다음 스테이지 이동 게이트를 오픈한다.
	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	if (pGameModeStage->IsStageClear())
		GateOpenDelegate.Broadcast();
}

int32 APG_GameStateStage::GetLevelInstallMonsterCount()
{
	int32 nMonsterCount = 0;
	for (TActorIterator<APG_MonChar> It(GetWorld()); It; ++It)
		nMonsterCount++;

	return nMonsterCount;
}

int32 APG_GameStateStage::GetRemainStageTimer()
{
	return StagePlayData.RemainTime;
}

int32 APG_GameStateStage::GetMaxLethalKill()
{
	return StagePlayData.NumberOfMonstersMuiltiKilled;
}


int32 APG_GameStateStage::GetClearTotalPoint()
{
	int nTotalReward = 0;

	auto pGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	ABCHECK(nullptr != pGameStateStage, 0);

	// 스테이지 클리어 포인트
	nTotalReward += GetClearStagePoint();

	// 스테이지 미션 포인트
	nTotalReward += GetClearMissionPoint();

	// 몬스터 처치 포인트
	nTotalReward += GetClearMonsterKillPoint();

	// 광고 보상 포인트
	if (pGameStateStage->IsViewAD())
		nTotalReward = nTotalReward * PG_ADVIEW_REWARD;

	return nTotalReward;
}

int32 APG_GameStateStage::GetClearStagePoint()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, 0);

	auto pGameModeBase = Cast<APG_GameModeBase>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeBase, 0);

	auto pTableStageData = pGameInstance->GetTableStageData(pGameModeBase->GetPlayStageID());
	ABCHECK(nullptr != pTableStageData, 0);

	return pTableStageData->StageClearReward;
}

int32 APG_GameStateStage::GetClearMissionPoint()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, 0);

	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage, 0);

	auto pTableStageData = pGameInstance->GetTableStageData(pGameModeStage->GetPlayStageID());
	ABCHECK(nullptr != pTableStageData, 0);

	bool bIsClearAmount = pGameModeStage->IsClearMission(EStageClearType::eSCT_AmountKill);
	bool bIsWasClearAmount = pGameModeStage->IsWasClearMisiion(EStageClearType::eSCT_AmountKill);
	bool bIsClearLethal = pGameModeStage->IsClearMission(EStageClearType::eSCT_Lethal);
	bool bIsWasClearLethal = pGameModeStage->IsWasClearMisiion(EStageClearType::eSCT_Lethal);
	bool bIsClearTime = pGameModeStage->IsClearMission(EStageClearType::eSCT_ClearTime);
	bool bIsWasClearTime = pGameModeStage->IsWasClearMisiion(EStageClearType::eSCT_ClearTime);

	int32 nClearPoint = 0;
	nClearPoint += (bIsClearAmount && !bIsWasClearAmount) ? pTableStageData->StarReward : 0;
	nClearPoint += (bIsClearLethal && !bIsWasClearLethal) ? pTableStageData->StarReward : 0;
	nClearPoint += (bIsClearTime && !bIsWasClearTime) ? pTableStageData->StarReward : 0;

	return nClearPoint;
}
int32 APG_GameStateStage::GetClearMonsterKillPoint()
{
	auto pGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	ABCHECK(nullptr != pGameStateStage, 0);

	return pGameStateStage->GetStagePlayData()->KillRewardPoint;
}

void APG_GameStateStage::SetMaxLethalKill(int32 a_nMaxLethalKill)
{
	if (a_nMaxLethalKill <= StagePlayData.NumberOfMonstersMuiltiKilled)
		return;

	StagePlayData.NumberOfMonstersMuiltiKilled = a_nMaxLethalKill;

	UdpateWidgetPlayStage.Broadcast();
}

bool APG_GameStateStage::IsViewAD()
{
	return m_bViewAD;
}

void APG_GameStateStage::SetViewAD(bool bEnable)
{
	m_bViewAD = bEnable;
}

bool APG_GameStateStage::RequestViewAD()
{
	if (m_bViewAD)
		return false;

	auto pMyPlayerController = Cast<APG_PlayerController>(GetWorld()->GetFirstPlayerController());
	ABCHECK(nullptr != pMyPlayerController, false);

	auto pPlayerState = Cast<APG_MyPlayerState>(pMyPlayerController->PlayerState);
	ABCHECK(nullptr != pPlayerState, false);

	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeStage, false);

	SetViewAD(true);

	// 광고 보상 적용
	int32 nRewardPoint = pPlayerState->GetOriginalPlayerData()->RewardPoint;
	pPlayerState->SetRewardPoint(nRewardPoint + GetClearTotalPoint());

	// 플레이어 데이터 저장
	pGameModeStage->SetSavePlayerData(*pPlayerState->GetPlayingPlayerData());

	OnViewAD.Broadcast(m_bViewAD);

	return m_bViewAD;
}

void APG_GameStateStage::OnCheckStageClear()
{
	auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
	if (pGameModeStage->IsStageClear())
	{
		GetWorldTimerManager().ClearTimer(StageTimerHandle);
		ABLOG(Warning, TEXT("Stage Clear Succees!"));
		StageClearDelegate.Broadcast();
	}
	else
	{
		ABLOG(Warning, TEXT("Stage Clear Failed!"));
	}
}

void APG_GameStateStage::OnCheckStageFailed()
{
	GetWorldTimerManager().ClearTimer(StageTimerHandle);

	StageFailedDelegate.Broadcast();
}

void APG_GameStateStage::OnKillAddRewardPoint(int32 a_nAddRewardPoint)
{
	StagePlayData.KillRewardPoint += a_nAddRewardPoint;
}

void APG_GameStateStage::OnStageTimerTick()
{
	StagePlayData.PlayTime += 1;

	if (0 < StagePlayData.RemainTime)
	{
		--StagePlayData.RemainTime;
		StageTimerDelegate.Broadcast();
		UdpateWidgetPlayStage.Broadcast();
	}
}

bool APG_GameStateStage::CheckStageData()
{
	ABCHECK(StageData, false);

	if (StagePlayData.NumberOfMonsters < StageData->M_Amount)
	{
		// 설치된 몬스터가 스테이지 테이블의 "M_Amount"값보다 작습니다. (스테이지 클리어를 할 수 없는 조건)
		ABLOG(Error, TEXT("The installed monster is less than the ""M_Amount"" value in the stage table."), false);
	}

	return true;
}