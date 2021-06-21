// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PG_MonCharNormal.h"
#include "Character/PG_MonChar.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "AI/PG_AICon_Mon.h"

APG_MonCharNormal::APG_MonCharNormal()
{
	MonsterType = EMonsterType::eMT_Normal;
	AIControllerClass = APG_AICon_Mon::StaticClass();
}

void APG_MonCharNormal::BeginPlay()
{
	Super::BeginPlay();

}

void APG_MonCharNormal::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MonsterData = &MonsterNormalData;
}

