// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/Console/Commands/PG_CommandStartStage.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Game/GameMode/PG_GameModeBase.h"

PG_CommandStartStage::PG_CommandStartStage()
{
}

PG_CommandStartStage::~PG_CommandStartStage()
{
}

void PG_CommandStartStage::UpdateCommand()
{
	if (bOnCommand)
	{
		bOnCommand = false;
		Excute();
	}
}

void PG_CommandStartStage::Excute()
{
	ABCHECK(nullptr != G_PGWorld);
	ABCHECK(nullptr != m_pConsoleVariable);

	auto pGameMdoe = Cast<APG_GameModeBase>(G_PGWorld->GetAuthGameMode());
	ABCHECK(nullptr != pGameMdoe);

	//auto pGameInstance = Cast<UPG_GameInstance>(G_PGWorld->GetGameInstance());
	//ABCHECK(nullptr != pGameInstance);

	if (false == pGameMdoe->SetPlayStageID(m_pConsoleVariable->GetInt()))
	{
		ABLOG(Warning, TEXT("There is no corresponding stage information. (StageID ; %d)"), m_pConsoleVariable->GetInt());
	}

	pGameMdoe->StartGame();
}

void PG_CommandStartStage::OnChangeCommand()
{
	ABCHECK(nullptr != m_pConsoleVariable);
	bOnCommand = true;
}