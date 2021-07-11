// Fill out your copyright notice in the Description page of Project Settings.
#include "Game/Console/Commands/PG_CommandShowMeTheMoney.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Game/GameMode/PG_GameModeBase.h"

#include "SaveData/PG_SavePlayerData.h"

PG_CommandShowMeTheMoney::PG_CommandShowMeTheMoney()
{
}

PG_CommandShowMeTheMoney::~PG_CommandShowMeTheMoney()
{
}


void PG_CommandShowMeTheMoney::UpdateCommand()
{
	if (bOnCommand)
	{
		bOnCommand = false;
		Excute();
	}
}

void PG_CommandShowMeTheMoney::Excute()
{
	ABCHECK(nullptr != G_PGWorld);
	ABCHECK(nullptr != m_pConsoleVariable);

	int32 nRewardPoint = m_pConsoleVariable->GetInt();

	auto pGameMdoeBase = Cast<APG_GameModeBase>(G_PGWorld->GetAuthGameMode());
	if (nullptr == pGameMdoeBase)
	{
		ABLOG(Warning, TEXT("Can only be used in stage play."));
		return;
	}

	auto pSavePlayerData = pGameMdoeBase->GetSavePlayerData();
	ABCHECK(nullptr != pSavePlayerData);

	pSavePlayerData->m_nRewardPoint = nRewardPoint;
	pGameMdoeBase->SavePlayerData();

	auto pMyPlayerState = Cast<APG_MyPlayerState>(G_PGWorld->GetFirstPlayerController()->PlayerState);
	ABCHECK(nullptr != pMyPlayerState);

	if (false == pMyPlayerState->InitPlayerData())
	{
		ABLOG(Error, TEXT("InitPlayerData Error"));
	}
}

void PG_CommandShowMeTheMoney::OnChangeCommand()
{
	ABCHECK(nullptr != m_pConsoleVariable);
	bOnCommand = true;
}