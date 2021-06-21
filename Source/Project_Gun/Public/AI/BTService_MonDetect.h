// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <BehaviorTree/BTService.h>
#include "BTService_MonDetect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UBTService_MonDetect : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_MonDetect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	
};
