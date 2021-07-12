// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Character/PG_MonChar.h"

#include "PG_MonCharDivision.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_GUN_API APG_MonCharDivision : public APG_MonChar
{
	GENERATED_BODY()

public:
	APG_MonCharDivision();

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	bool	IsPossibleToDivision();				// 분할 가능한지 검사
	bool	IsDivisionChild();					// 자식 여부 확인

	int32	GetDivisionCountToHPRatio();		// 현재 몬스터의 남은 체력 기준, 분할 할 수 있는 횟수를 반환한다.
	const FPGMonsterData_Division* GetMonsterDataDivision() { return &MonsterDivisionData; }

	void	SetMonsterDataDivision(FPGMonsterData_Division& MonsterData_Division);

public:
	virtual bool OnDamage(int32 nDamage, bool IsUseLetharAttack = false) override;

	// 스폰되면 생성된 몬스터 액터가 호출된다.
	void OnSpawnActor(APG_MonCharDivision* ASpawnActor);

protected :
	UPROPERTY(VisibleAnywhere, Category = "PG MonsterComponent")
	class UPG_ComponentDivisionSpawn* DivisionSpawnComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PG MonsterInfo", Meta = (AllowPrivateAccess = true))
	struct FPGMonsterData_Division MonsterDivisionData;
	
	UPROPERTY()
	bool bIsDivisionChild;	// 분화형 몬스터의 자식 여부
};
