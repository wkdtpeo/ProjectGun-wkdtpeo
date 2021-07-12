// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/PG_AICon_Mon.h"
#include "Character/PG_MonChar.h"

#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardData.h>
#include <BehaviorTree/BlackboardComponent.h>

const FName APG_AICon_Mon::HomePosKey(TEXT("HomePos"));
const FName APG_AICon_Mon::TargetKey(TEXT("Target"));
const FName APG_AICon_Mon::DetectRangeKey(TEXT("MDetectRange"));
const FName APG_AICon_Mon::ChaseRangeKey(TEXT("MChaseRange"));

APG_AICon_Mon::APG_AICon_Mon()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/PG/AI/BB_Mon.BB_Mon"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/PG/AI/BT_MonNormal.BT_MonNormal"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void APG_AICon_Mon::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

bool APG_AICon_Mon::RunAI()
{
	ABCHECK(UseBlackboard(BBAsset, Blackboard), false);
	ABCHECK(SetBlackboardValue(), false);

	if (!RunBehaviorTree(BTAsset))
	{
		ABLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		return false;
	}

	return true;
}

void APG_AICon_Mon::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
		//StopMovement();
	}
}

bool APG_AICon_Mon::SetBlackboardValue()
{
	ABCHECK(nullptr != Blackboard, false);

	auto MonsterCharacter = Cast<APG_MonChar>(GetPawn());
	if (nullptr == MonsterCharacter)
		return false;

	Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
	Blackboard->SetValueAsInt(DetectRangeKey, MonsterCharacter->GetDetectRange());
	Blackboard->SetValueAsInt(ChaseRangeKey, MonsterCharacter->GetChaseRange());

	return true;
}
