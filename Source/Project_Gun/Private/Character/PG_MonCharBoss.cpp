// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PG_MonCharBoss.h"
#include "Character/PG_MonChar.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "AI/PG_AICon_Mon.h"

APG_MonCharBoss::APG_MonCharBoss()
{
	MonsterType = EMonsterType::eMT_Boss;
	AIControllerClass = APG_AICon_Mon::StaticClass();
}

void APG_MonCharBoss::BeginPlay()
{
	Super::BeginPlay();
}

void APG_MonCharBoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MonsterData = &MonsterBossData;
}