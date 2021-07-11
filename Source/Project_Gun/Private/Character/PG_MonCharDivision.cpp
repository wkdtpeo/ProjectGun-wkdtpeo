// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PG_MonCharDivision.h"
#include "Character/PG_MonChar.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "AI/PG_AICon_Mon.h"
#include "Character/Component/PG_ComponentDivisionSpawn.h"

APG_MonCharDivision::APG_MonCharDivision()
{
	MonsterType = EMonsterType::eMT_Division;
	AIControllerClass = APG_AICon_Mon::StaticClass();
	DivisionSpawnComponent = CreateDefaultSubobject<UPG_ComponentDivisionSpawn>(TEXT("COMPONENTDIVISIONSPAWN"));
	bIsDivisionChild = false;
}

void APG_MonCharDivision::BeginPlay()
{
	Super::BeginPlay();
}

void APG_MonCharDivision::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MonsterDivisionData.MRemainDivisionCount = MonsterDivisionData.MDivisionCount;
	MonsterData = &MonsterDivisionData;
}

int32 APG_MonCharDivision::GetDivisionCountToHPRatio()
{
	if (MonsterDivisionData.MRemainDivisionCount <= 0 || 0.0f >= GetHPRatio())
		return 0;

	float fActiveHPRatio = 1.0f / (MonsterDivisionData.MDivisionCount + 1);
	return (int32)(GetHPRatio() / fActiveHPRatio);
}

bool APG_MonCharDivision::IsPossibleToDivision()
{
	if (0 >= MonsterDivisionData.MRemainDivisionCount || 0 >= MonsterDivisionData.MDivisionCount)
		return false;

	// 분열 대역
	float fPossibleToDivions_HPRatio = (1.0f / (MonsterDivisionData.MDivisionCount + 1)) * MonsterDivisionData.MRemainDivisionCount;
	return (GetHPRatio() < fPossibleToDivions_HPRatio);
}

bool APG_MonCharDivision::IsDivisionChild()
{
	return bIsDivisionChild;
}

void APG_MonCharDivision::SetMonsterDataDivision(FPGMonsterData_Division& MonsterData_Division)
{
	MonsterDivisionData = MonsterData_Division;
}

bool APG_MonCharDivision::OnDamage(int32 nDamage, bool IsUseLetharAttack)
{
	bool bIsDie = Super::OnDamage(nDamage, IsUseLetharAttack);

	if (false == bIsDie && IsPossibleToDivision())
	{
		if (0 < MonsterDivisionData.MRemainDivisionCount)
		{
			FVector vRange(MonsterDivisionData.MDivisionSpawnRange, MonsterDivisionData.MDivisionSpawnRange, 0.0f);
			if (DivisionSpawnComponent)
				DivisionSpawnComponent->DivisionSpawnActor<APG_MonCharDivision>(MonsterDivisionData.MDivisionAmount, vRange);
		}

		MonsterDivisionData.MRemainDivisionCount = GetDivisionCountToHPRatio();
	}

	return bIsDie;
}

void APG_MonCharDivision::OnSpawnActor(APG_MonCharDivision* ASpawnActor)
{
	ABCHECK(nullptr != ASpawnActor);

	FPGMonsterData_Division MakeMonsterData_Division = MonsterDivisionData;
	MakeMonsterData_Division.MRemainDivisionCount = GetDivisionCountToHPRatio();
	ASpawnActor->bIsDivisionChild = true;
	ASpawnActor->SetMonsterDataDivision(MakeMonsterData_Division);
	ASpawnActor->SetHP(MakeMonsterData_Division.MHP);
}