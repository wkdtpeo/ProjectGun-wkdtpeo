﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Character/PG_MonChar.h"

#include "PG_MonCharNormal.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API APG_MonCharNormal : public APG_MonChar
{
	GENERATED_BODY()
	
public :
	APG_MonCharNormal();

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PG MonsterInfo", Meta = (AllowPrivateAccess = true))
	struct FPGMonsterData_Normal MonsterNormalData;
};
