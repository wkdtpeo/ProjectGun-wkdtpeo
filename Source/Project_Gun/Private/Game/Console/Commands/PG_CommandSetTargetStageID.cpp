// Fill out your copyright notice in the Description page of Project Settings.
#include "Game/Console/Commands/PG_CommandSetTargetStageID.h"
#include "Game/GameMode/PG_GameModeBase.h"

PG_CommandSetTargetStageID::PG_CommandSetTargetStageID()
{
}

PG_CommandSetTargetStageID::~PG_CommandSetTargetStageID()
{
}

void PG_CommandSetTargetStageID::UpdateCommand()
{
	if (bOnCommand)
	{
		bOnCommand = false;
		Excute();
	}
}

void PG_CommandSetTargetStageID::Excute()
{
	ABCHECK(nullptr != G_PGWorld);
	ABCHECK(nullptr != m_pConsoleVariable);

	auto pGameMdoe = Cast<APG_GameModeBase>(G_PGWorld->GetAuthGameMode());
	ABCHECK(nullptr != pGameMdoe);

	if (false == pGameMdoe->SetPlayStageID(m_pConsoleVariable->GetInt()))
	{
		ABLOG(Warning, TEXT("There is no corresponding stage information. (StageID ; %d)"), m_pConsoleVariable->GetInt());
	}

	ABLOG(Warning, TEXT("You have finished setting up for that stage. (StageID ; %d)"), m_pConsoleVariable->GetInt());
}

void PG_CommandSetTargetStageID::OnChangeCommand()
{
	ABCHECK(nullptr != m_pConsoleVariable);
	bOnCommand = true;
}