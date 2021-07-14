// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PG_MonCharDivisionSkillType.h"
#include "Character/PG_MonChar.h"
#include "Character/Component/PG_ComponentDivisionSpawn.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "AI/PG_AICon_Mon.h"

APG_MonCharDivisionSkillType::APG_MonCharDivisionSkillType()
{
	AIControllerClass = APG_AICon_Mon::StaticClass();
}

void APG_MonCharDivisionSkillType::BeginPlay()
{
	Super::BeginPlay();
}

void APG_MonCharDivisionSkillType::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MonsterData = &MonsterDivisionSkillTypeData;
}
