// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/GameMode/PG_GameModeStage.h"
#include "Player/PlayerController/PG_PlayerController.h"
#include "Character/PG_MyChar.h"
#include "Game/GameState/PG_GameStateStage.h"
#include "Player/PlayerState/PG_MyPlayerState.h"
#include "SaveData/PG_SaveStageData.h"
#include "Define/PG_GamePlayData.h"
#include "Game/GameInstance/PG_GameInstance.h"

APG_GameModeStage::APG_GameModeStage()
{
	DefaultPawnClass = APG_MyChar::StaticClass();
	PlayerControllerClass = APG_PlayerController::StaticClass();
	GameStateClass = APG_GameStateStage::StaticClass();
	PlayerStateClass = APG_MyPlayerState::StaticClass();
}

void APG_GameModeStage::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APG_GameModeStage::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void APG_GameModeStage::BeginPlay()
{
	Super::BeginPlay();
}


bool APG_GameModeStage::IsClearMission(enum EStageClearType a_ekStageClearType)
{
	auto pGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	ABCHECK(nullptr != pGameStateStage, false);

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	const FPGStageData*	cpStageData = pGameInstance->GetTableStageData(m_pSelectSaveStageData->m_nStageID);
	ABCHECK(nullptr != cpStageData, false);

	switch (a_ekStageClearType)
	{
	case EStageClearType::eSCT_AmountKill:
		return (pGameStateStage->GetStagePlayData()->NumberOfMonstersKilled >= cpStageData->M_Amount);
	case EStageClearType::eSCT_ClearTime:
		return (IsClearMission(EStageClearType::eSCT_Lethal) && (pGameStateStage->GetRemainStageTimer() > 0));
	case EStageClearType::eSCT_Lethal:
		return (pGameStateStage->GetMaxLethalKill() >= cpStageData->Multi_Kill);
	}

	ABLOG(Error, TEXT("Can't find MisiionType"));
	return false;
}

bool APG_GameModeStage::IsWasClearMisiion(enum EStageClearType a_ekStageClearType)
{
	ABCHECK(nullptr != m_pSelectSaveStageData, false);

	switch (a_ekStageClearType)
	{
	case EStageClearType::eSCT_AmountKill:
		return m_pSelectSaveStageData->m_bWasClearAmount;
	case EStageClearType::eSCT_ClearTime:
		return m_pSelectSaveStageData->m_bWasClearTime;
	case EStageClearType::eSCT_Lethal:
		return m_pSelectSaveStageData->m_bWasClearLethal;
	}

	ABLOG(Error, TEXT("Can't find MisiionType"));
	return false;
}

bool APG_GameModeStage::IsStageClear()
{
	// 모든 몬스터를 죽여야 스테이지 클리어 가능하다.
	// 필살기 킬수나 제한시간은 부수적인 스테이지 클리어 조건이기 때문에 검사하지 않는다.
	return IsClearMission(EStageClearType::eSCT_AmountKill);
}

int32 APG_GameModeStage::GetPlayStageID()
{
	return m_pSelectSaveStageData->m_nStageID;
}

int32 APG_GameModeStage::GetClearTotalPoint()
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

int32 APG_GameModeStage::GetClearStagePoint()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, 0);

	auto pTableStageData = pGameInstance->GetTableStageData(GetPlayStageID());
	ABCHECK(nullptr != pTableStageData, 0);

	return pTableStageData->StageClearReward;
}

int32 APG_GameModeStage::GetClearMissionPoint()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, 0);

	auto pTableStageData = pGameInstance->GetTableStageData(GetPlayStageID());
	ABCHECK(nullptr != pTableStageData, 0);

	bool bIsClearAmount = IsClearMission(EStageClearType::eSCT_AmountKill);
	bool bIsWasClearAmount = IsWasClearMisiion(EStageClearType::eSCT_AmountKill);
	bool bIsClearLethal = IsClearMission(EStageClearType::eSCT_Lethal);
	bool bIsWasClearLethal = IsWasClearMisiion(EStageClearType::eSCT_Lethal);
	bool bIsClearTime = IsClearMission(EStageClearType::eSCT_ClearTime);
	bool bIsWasClearTime = IsWasClearMisiion(EStageClearType::eSCT_ClearTime);

	int32 nClearPoint = 0;
	nClearPoint += (bIsClearAmount && !bIsWasClearAmount) ? pTableStageData->StarReward : 0;
	nClearPoint += (bIsClearLethal && !bIsWasClearLethal) ? pTableStageData->StarReward : 0;
	nClearPoint += (bIsClearTime && !bIsWasClearTime) ? pTableStageData->StarReward : 0;

	return nClearPoint;
}
int32 APG_GameModeStage::GetClearMonsterKillPoint()
{
	auto pGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	ABCHECK(nullptr != pGameStateStage, 0);

	return pGameStateStage->GetStagePlayData()->KillRewardPoint;
}

bool APG_GameModeStage::SavePlayerData()
{
	auto pPlayerState = Cast<APG_MyPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);
	ABCHECK(nullptr != pPlayerState, false);

	ABCHECK(SetSavePlayerData(*pPlayerState->GetPlayingPlayerData()), false);

	return APG_GameModeBase::SavePlayerData();
}

bool APG_GameModeStage::SaveStageData()
{
	auto pGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	ABCHECK(nullptr != pGameStateStage, false);

	ABCHECK(SetSaveStageData(*pGameStateStage->GetStagePlayData()), false);

	return APG_GameModeBase::SaveStageData();
}

bool APG_GameModeStage::SetSavePlayerData(const struct FPGPlayerData& a_rkPlayerData)
{
	ABCHECK(nullptr != m_pSavePlayerData, false);
	m_pSavePlayerData->m_nRewardPoint = a_rkPlayerData.RewardPoint;
	//m_pSavePlayerData->m_nEquipWeaponSlotIndex = a_rkPlayerData.EquipWeaponInventoryIndex;
	//m_pSavePlayerData->m_kEquipWeaponClass = a_rkPlayerData.EquipWeaponClass;

	return true;
}

bool APG_GameModeStage::SetSaveStageData(const FPGStagePlayData& a_rkStagePlayData)
{
	ABCHECK(nullptr != m_pSelectSaveStageData, false);
	m_pSelectSaveStageData->m_kStagePlayData = a_rkStagePlayData;

	if (false == m_pSelectSaveStageData->m_bWasClearAmount)
		m_pSelectSaveStageData->m_bWasClearAmount = IsClearMission(EStageClearType::eSCT_AmountKill);

	if (false == m_pSelectSaveStageData->m_bWasClearTime)
		m_pSelectSaveStageData->m_bWasClearTime = IsClearMission(EStageClearType::eSCT_ClearTime);

	if (false == m_pSelectSaveStageData->m_bWasClearLethal)
		m_pSelectSaveStageData->m_bWasClearLethal = IsClearMission(EStageClearType::eSCT_Lethal);

	return true;
}