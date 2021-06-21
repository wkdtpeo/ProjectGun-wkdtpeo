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
	bool	IsPossibleToDivision();				// ���� �������� �˻�
	bool	IsDivisionChild();					// �ڽ� ���� Ȯ��

	int32	GetDivisionCountToHPRatio();		// ���� ������ ���� ü�� ����, ���� �� �� �ִ� Ƚ���� ��ȯ�Ѵ�.
	const FPGMonsterData_Division* GetMonsterDataDivision() { return &MonsterDivisionData; }

	void	SetMonsterDataDivision(FPGMonsterData_Division& MonsterData_Division);

public:
	virtual bool OnDamage(int32 nDamage, bool IsUseLetharAttack = false) override;

	// �����Ǹ� ������ ���� ���Ͱ� ȣ��ȴ�.
	void OnSpawnActor(APG_MonCharDivision* ASpawnActor);

protected :
	UPROPERTY(VisibleAnywhere, Category = "PG MonsterComponent")
	class UPG_ComponentDivisionSpawn* DivisionSpawnComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PG MonsterInfo", Meta = (AllowPrivateAccess = true))
	struct FPGMonsterData_Division MonsterDivisionData;
	
	UPROPERTY()
	bool bIsDivisionChild;	// ��ȭ�� ������ �ڽ� ����
};
