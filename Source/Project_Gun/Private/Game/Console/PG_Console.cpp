// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/Console/PG_Console.h"
#include <Core\Public\HAL\IConsoleManager.h>

PG_Console::PG_Console()
{
}

PG_Console::~PG_Console()
{
    ReleaseCommands();
}

void PG_Console::CreateCommands()
{
#define CREATE_CONSOLEVARIABLE(CommandName, DefaultValue, Help, Flags)    \
    IConsoleManager::Get().RegisterConsoleVariable(CommandName, \
    DefaultValue,  \
    Help, \
    Flags)\

#define ADD_COMMAND(index, CommandName, ClassName, DefaultValue, Help, Flags) \
    m_kCommands[index] = new ClassName();   \
    m_kCommands[index]->Init(CREATE_CONSOLEVARIABLE(CommandName, DefaultValue, Help, Flags))   \

    //====================================================================================================================
    // Create Commands
    //====================================================================================================================
    ADD_COMMAND(eCC_SET_TARGET_STAGE_ID, TEXT("PG.SetTargetStageID"), PG_CommandSetTargetStageID,
        1,
        TEXT("Changes to the stage ID value at the level you are currently playing.\n")
        TEXT("(After the change, clear the stage, or start with the changed stage value when restarting.).\n"),
        ECVF_Scalability | ECVF_RenderThreadSafe);

    ADD_COMMAND(eCC_RESTART_STAGE, TEXT("PG.RestartStage"), PG_CommandRestartStage,
        1,
        TEXT("Restart the current stage.\n"),
        ECVF_Scalability | ECVF_RenderThreadSafe);

    ADD_COMMAND(eCC_NEXT_STAGE, TEXT("PG.NextStage"), PG_CommandNextStage,
        1,
        TEXT("Start the next stage.\n"),
        ECVF_Scalability | ECVF_RenderThreadSafe);

    ADD_COMMAND(eCC_PREV_STAGE, TEXT("PG.PrevStage"), PG_CommandPrevStage,
        1,
        TEXT("Start the previous stage.\n"),
        ECVF_Scalability | ECVF_RenderThreadSafe);

    ADD_COMMAND(eCC_START_STAGE, TEXT("PG.StartStage"), PG_CommandStartStage,
        1,
        TEXT("Start the input stage.\n"),
        ECVF_Scalability | ECVF_RenderThreadSafe);

    ADD_COMMAND(eCC_SHOW_ME_THE_MONEY, TEXT("PG.ShowMeTheMoney"), PG_CommandShowMeTheMoney,
        1,
        TEXT("Set Reward Point.\n"),
        ECVF_Scalability | ECVF_RenderThreadSafe);

    ADD_COMMAND(eCC_ALL_STAGE_UNLOCK, TEXT("PG.AllStageUnlock"), PG_CommandAllStageUnlock,
        1,
        TEXT("1 : All Stage Unlock..\n"),
        ECVF_Scalability | ECVF_RenderThreadSafe);   

    //====================================================================================================================

#undef ADD_COMMAND
#undef CREATE_CONSOLEVARIABLE
}

bool PG_Console::InitConsoleCommands()
{
    m_kCommands.Init(nullptr, eCC_Max);

    CreateCommands();

	return true;
}

void PG_Console::ReleaseCommands()
{
    for (auto command : m_kCommands)
    {
        if (command)
        {
            IConsoleManager::Get().UnregisterConsoleObject(command->GetConsoleVariable());
            delete command;
            command = nullptr;
        }
    }

    m_kCommands.Reset();
}

PG_Command* PG_Console::GetCommand(class IConsoleVariable* pConsoleVariable)
{
    for (auto command : m_kCommands)
    {
        if (nullptr == command)
            continue;

        if (command->GetConsoleVariable() == pConsoleVariable)
            return command;
    }

    return nullptr;
}

void PG_Console::UpdateCommands()
{
    for (auto command : m_kCommands)
    {
        if (nullptr == command)
            continue;

        command->UpdateCommand();
    }
}
