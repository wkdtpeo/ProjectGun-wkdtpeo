// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/GameInstance/PG_GameInstance.h"
#include "Game/GameMode/PG_GameModeBase.h"
#include "Game/GameMode/PG_GameModeMainLobby.h"

UPG_GameInstance::UPG_GameInstance()
{
	LoadTable();
}

void UPG_GameInstance::Init()
{
	Super::Init();

	//PalyStageID = 0;

	m_kADData.Init(this);
	LoadConsoleCommand();
}

void UPG_GameInstance::Shutdown()
{
	m_kConsole.ReleaseCommands();
	Super::Shutdown();
}

FPGCharacterMovementData* UPG_GameInstance::GetTableCharacterMovementData()
{
	return m_pCharacterMovementTable ? m_pCharacterMovementTable->FindRow<FPGCharacterMovementData>(*FString::FromInt(0), TEXT("")) : nullptr;
}

FPGCharacterCameraData* UPG_GameInstance::GetTableCharacterCameraData()
{
	return m_pCharacterCameraTable ? m_pCharacterCameraTable->FindRow<FPGCharacterCameraData>(*FString::FromInt(0), TEXT("")) : nullptr;
}

const FPGWeaponTableData* UPG_GameInstance::GetWeaponTableData(int32 nIndex)
{
	return m_pWeaponTable ? m_pWeaponTable->FindRow<FPGWeaponTableData>(*FString::FromInt(nIndex), TEXT("")) : nullptr;
}

const bool UPG_GameInstance::GetAllWeaponTableData(TArray<const FPGWeaponTableData*>& a_rkWeaponTableDatas)
{
	ABCHECK(nullptr != m_pWeaponTable, false);

	m_pWeaponTable->GetAllRows(TEXT("WeaponShopTable"), a_rkWeaponTableDatas);

	return (0 < a_rkWeaponTableDatas.Num());
}

const FPGStageData*	UPG_GameInstance::GetTableStageData(int32 nID)
{
	return m_pStageTable ? m_pStageTable->FindRow<FPGStageData>(*FString::FromInt(nID), TEXT("")) : nullptr;
}

const bool UPG_GameInstance::GetAllTableStageData(OUT TArray<const FPGStageData*>& a_rkStageDatas)
{
	ABCHECK(nullptr != m_pStageTable, false);

	m_pStageTable->GetAllRows(TEXT("StageTable"), a_rkStageDatas);

	return (0 < a_rkStageDatas.Num());
}

const FPGLethalAttackData*	UPG_GameInstance::GetTableLethalAttackData(int32 nID)
{
	return m_pLethalAttackTable ? m_pLethalAttackTable->FindRow<FPGLethalAttackData>(*FString::FromInt(nID), TEXT("")) : nullptr;
}

const FPGADTableData* UPG_GameInstance::GetADTableData(EADType eADType)
{
	return m_pADTable ? m_pADTable->FindRow<FPGADTableData>(*FString::FromInt((int32)eADType), TEXT("")) : nullptr;
}

int32 UPG_GameInstance::GetADRemainTime(EADType eADType)
{
	return m_kADData.GetRemainReViewableTime(eADType);
}

bool UPG_GameInstance::IsAbleADView(EADType eADType)
{
	return m_kADData.IsAbleViewAD(eADType);
}

void UPG_GameInstance::ResetADRemainTime(EADType eADType)
{
	m_kADData.Reset(eADType);
}

bool UPG_GameInstance::LoadTable()
{
#define LOAD_TABLE(Path, Out)	\
{	\
	FString TablePath = Path;	\
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Table(*TablePath);	\
	if (DT_Table.Succeeded())	\
	{	\
		Out = DT_Table.Object;	\
		ABCHECK(Out->GetRowMap().Num() > 0, false);	\
	}	\
	else	\
	{	\
		ABLOG(Error, TEXT("Can't Find Table : %s"), *TablePath);	return false; \
	}	\
}	
	LOAD_TABLE(TEXT("/Game/PG/Tables/Character/CharacterMovementTable.CharacterMovementTable"), m_pCharacterMovementTable);
	LOAD_TABLE(TEXT("/Game/PG/Tables/Character/CharacterCameraTable.CharacterCameraTable"), m_pCharacterCameraTable);
	LOAD_TABLE(TEXT("/Game/PG/Tables/Weapon/WeaponTable.WeaponTable"), m_pWeaponTable);
	LOAD_TABLE(TEXT("/Game/PG/Tables/Stage/StageData.StageData"), m_pStageTable);
	LOAD_TABLE(TEXT("/Game/PG/Tables/LethalAttack/LethalAttackTable.LethalAttackTable"), m_pLethalAttackTable);
	LOAD_TABLE(TEXT("/Game/PG/Tables/AD/ADTable.ADTable"), m_pADTable);
	
#undef LOAD_TABLE

	return true;
}

bool UPG_GameInstance::LoadConsoleCommand()
{
	ABCHECK(m_kConsole.InitConsoleCommands(), false);
	return true;
}

FString UPG_GameInstance::GetStageName(int32 StageID)
{
	return FString::Printf(TEXT("/Game/PG/Maps/Stage/Stage%d"), StageID);
}

//void UPG_GameInstance::StartGame_NextStage()
//{
//	PalyStageID++;
//	//StartGame();
//
//	auto pGameModeBase = Cast<APG_GameModeBase>(GetWorld()->GetAuthGameMode());
//	if (pGameModeBase)
//	{
//		pGameModeBase->StartGame();
//	}
//}
//
//void UPG_GameInstance::RestartGame()
//{
//	FString RestartStageName = GetStageName(PalyStageID);
//	//FString DebugMsg = FString::Printf(TEXT("Stage Restart : %s"), *RestartStageName);
//	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, DebugMsg);
//
//	UGameplayStatics::OpenLevel(GetWorld(), *RestartStageName);
//}
//
//bool UPG_GameInstance::OnNextSelectStage()
//{
//	int32 nNextStage = PalyStageID + 1;
//
//	if (GetTableStageData(nNextStage))
//	{
//		int32 nBackupPlayStageID = PalyStageID;
//		PalyStageID = nNextStage;
//
//		auto pGameModeBase = Cast<APG_GameModeBase>(GetWorld()->GetAuthGameMode());
//		ABCHECK(nullptr != pGameModeBase, false);
//
//		if (false == pGameModeBase->ChangeSelectStageData(PalyStageID))
//		{
//			PalyStageID = nBackupPlayStageID;
//			return false;
//		}
//
//		return true;
//	}
//
//	return false;
//}
//
//bool UPG_GameInstance::OnPrevSelectStage()
//{
//	int32 nPrevStage = PalyStageID - 1;
//
//	if (GetTableStageData(nPrevStage))
//	{
//		int32 nBackupPlayStageID = PalyStageID;
//		PalyStageID = nPrevStage;
//
//		auto pGameModeBase = Cast<APG_GameModeBase>(GetWorld()->GetAuthGameMode());
//		ABCHECK(nullptr != pGameModeBase, false);
//
//		if (false == pGameModeBase->ChangeSelectStageData(PalyStageID))
//		{
//			PalyStageID = nBackupPlayStageID;
//			return false;
//		}
//
//		return true;
//	}
//
//	return false;
//}
//
//bool UPG_GameInstance::SetPlayStageID(int32 nPlayStageID)
//{
//	ABCHECK(GetTableStageData(nPlayStageID), false);
//
//	PalyStageID = nPlayStageID;
//	return true;
//}

//bool UPG_GameInstance::StartGame()
//{
//	ABCHECK(GetTableStageData(PalyStageID), false);
//
//	auto pGameModeBase = Cast<APG_GameModeBase>(GetWorld()->GetAuthGameMode());
//	ABCHECK(nullptr != pGameModeBase, false);
//
//	auto pSavePlayerData = pGameModeBase->GetSavePlayerData();
//	ABCHECK(nullptr != pSavePlayerData, false);
//
//	pSavePlayerData->m_nLastPlayStageID = PalyStageID;
//	pGameModeBase->SavePlayerData();
//
//	FString NextStageName = GetStageName(PalyStageID);
//	//FString DebugMsg = FString::Printf(TEXT("Play Stage : %s"), *NextStageName);
//	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, DebugMsg);
//
//	UGameplayStatics::OpenLevel(GetWorld(), *NextStageName);
//
//	return true;
//}

void UPG_GameInstance::FromGameModeTick(float DeltaTime)
{
	m_kConsole.UpdateCommands();
	m_kADData.Update(DeltaTime);
}
