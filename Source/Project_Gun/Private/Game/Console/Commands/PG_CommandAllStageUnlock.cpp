// Fill out your copyright notice in the Description page of Project Settings.
#include "Game/Console/Commands/PG_CommandAllStageUnlock.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Game/GameMode/PG_GameModeBase.h"

#include "SaveData/PG_SavePlayerData.h"

PG_CommandAllStageUnlock::PG_CommandAllStageUnlock()
{
}

PG_CommandAllStageUnlock::~PG_CommandAllStageUnlock()
{
}


void PG_CommandAllStageUnlock::UpdateCommand()
{
	if (bOnCommand)
	{
		bOnCommand = false;
		Excute();
	}
}

void PG_CommandAllStageUnlock::Excute()
{
	ABCHECK(nullptr != G_PGWorld);
	ABCHECK(nullptr != m_pConsoleVariable);

	int32 nGetVar = m_pConsoleVariable->GetInt();
	if (1 != nGetVar)
		return;

	auto pGameInstance = Cast<UPG_GameInstance>(G_PGWorld->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	auto pGameModeBase = Cast<APG_GameModeBase>(G_PGWorld->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeBase);

	TArray<const FPGStageData*> kStageDatas;
	if (false == pGameInstance->GetAllTableStageData(kStageDatas))
	{
		ABLOG_S(Error);
		return;
	}

	int nStageID = 1;
	for (auto StageData : kStageDatas)
	{
		if (pGameModeBase->IsFirstPlayStage(nStageID))
		{
			if (false == pGameModeBase->ChangeSelectStageData(nStageID))
			{
				ABLOG(Error, TEXT("ChangeSelectStageData Error : %d"), nStageID++);
				continue;
			}

			if (false == pGameModeBase->SaveStageData())
			{
				ABLOG(Error, TEXT("SaveStageData Error : %d"), nStageID++);
				continue;
			}
		}

		++nStageID;
	}
}

void PG_CommandAllStageUnlock::OnChangeCommand()
{
	ABCHECK(nullptr != m_pConsoleVariable);
	bOnCommand = true;
}