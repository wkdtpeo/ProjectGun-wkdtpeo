// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Character/PG_MonChar.h"
#include "PG_MonCharDivisionSkillType.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API APG_MonCharDivisionSkillType : public APG_MonChar
{
	GENERATED_BODY()
	
public :
	APG_MonCharDivisionSkillType();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(VisibleInstanceOnly, Category = "PG MonsterComponent")
	class UPG_ComponentDivisionSpawn* CharDivisionComponent;

	UPROPERTY(EditAnywhere, Category = "PG MonsterInfo", Meta = (AllowPrivateAccess = true))
	struct FPGMonsterData_DivisionSkillType MonsterDivisionSkillTypeData;
	
};
