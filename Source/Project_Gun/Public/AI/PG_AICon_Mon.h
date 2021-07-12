// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"

#include <AIController.h>
#include "PG_AICon_Mon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API APG_AICon_Mon : public AAIController
{
	GENERATED_BODY()
	
public:
	APG_AICon_Mon();
	virtual void OnPossess(APawn* InPawn) override;
	//virtual void UnPossess() override;

	static const FName HomePosKey;
	static const FName TargetKey;		// 캐릭터 타겟
	static const FName DetectRangeKey;	// 몬스터가 캐릭터 인식하는 거리
	static const FName ChaseRangeKey;	// 몬스터가 캐릭터 쫓아오는 최대 거리

	virtual bool RunAI();
	virtual void StopAI();

private :
	bool SetBlackboardValue();

protected:
	UPROPERTY()
	class UBehaviorTree* BTAsset = nullptr;

	UPROPERTY()
	class UBlackboardData* BBAsset = nullptr;
};
