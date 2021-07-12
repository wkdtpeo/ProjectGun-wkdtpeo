// Fill out your copyright notice in the Description page of Project Settings.
#include "Game/Console/Commands/PG_CommandPrevStage.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Game/GameMode/PG_GameModeStage.h"

PG_CommandPrevStage::PG_CommandPrevStage()
{
}

PG_CommandPrevStage::~PG_CommandPrevStage()
{
}

void PG_CommandPrevStage::UpdateCommand()
{
	if (bOnCommand)
	{
		bOnCommand = false;
		Excute();
	}
}

void PG_CommandPrevStage::Excute()
{
	ABCHECK(nullptr != G_PGWorld);
	ABCHECK(nullptr != m_pConsoleVariable);
	
	if (1 != m_pConsoleVariable->GetInt())
	{
		ABLOG(Warning, TEXT("To use this command, enter a value of 1."));
		return;
	}

	auto pGameMdoe = Cast<APG_GameModeStage>(G_PGWorld->GetAuthGameMode());
	if (nullptr == pGameMdoe)
	{
		ABLOG(Warning, TEXT("Can only be used in stage play."));
		return;
	}

	pGameMdoe->StartGame_NextStage();
}

void PG_CommandPrevStage::OnChangeCommand()
{
	ABCHECK(nullptr != m_pConsoleVariable);
	bOnCommand = true;
}