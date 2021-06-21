// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Console/Commands/PG_Command.h"
#include "Game/GameInstance/PG_GameInstance.h"

PG_Command::PG_Command()
{
}

PG_Command::~PG_Command()
{
}

void PG_Command::Init(IConsoleVariable* pConsoleVariable)
{
	ABCHECK(nullptr != pConsoleVariable);

	m_pConsoleVariable = pConsoleVariable;
	m_pConsoleVariable->AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateStatic(&OnUpdateConsoleValue));
}

void PG_Command::OnUpdateConsoleValue(IConsoleVariable* pConsoleVariable)
{
	ABCHECK(nullptr != G_PGWorld);

	auto pGameInstance = Cast<UPG_GameInstance>(G_PGWorld->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	auto pConsole = pGameInstance->GetConsole();
	ABCHECK(nullptr != pConsole);

	auto pCommand = pConsole->GetCommand(pConsoleVariable);
	ABCHECK(nullptr != pCommand);

	pCommand->OnChangeCommand();
}