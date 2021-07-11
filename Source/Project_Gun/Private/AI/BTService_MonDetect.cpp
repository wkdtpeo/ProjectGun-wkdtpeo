// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTService_MonDetect.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "AI/PG_AICon_Mon.h"
#include "Character/PG_MyChar.h"
#include "Character/PG_MonChar.h"
#include <DrawDebugHelpers.h>


UBTService_MonDetect::UBTService_MonDetect()
{

}

void UBTService_MonDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	ABCHECK(nullptr != ControllingPawn);

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	ABCHECK(nullptr != World);

	int32 nMDetectRange = OwnerComp.GetBlackboardComponent()->GetValueAsInt(APG_AICon_Mon::DetectRangeKey);
	int32 nMChaseRange = OwnerComp.GetBlackboardComponent()->GetValueAsInt(APG_AICon_Mon::ChaseRangeKey);

	float fRange = (float)(OwnerComp.GetBlackboardComponent()->GetValueAsObject(APG_AICon_Mon::TargetKey) ? nMChaseRange : nMDetectRange);

	// AI 최적화 적용
	// 몬스터 시야거리 외 불필요한 콜리전 체크를 막는다.
	auto DefaultObject = World->GetFirstPlayerController()->GetCharacter();
	if (DefaultObject)
	{
		float fDistance = FMath::Abs<float>(FVector::Distance(DefaultObject->GetActorLocation(), Center));
		if (fDistance > fRange)
		{
			if (ControllingPawn->GetActorEnableCollision())
				ControllingPawn->SetActorEnableCollision(false);

			if (ControllingPawn->IsActorTickEnabled())
				ControllingPawn->SetActorTickEnabled(false);

			return;
		}
	}

	if (false == ControllingPawn->GetActorEnableCollision())
		ControllingPawn->SetActorEnableCollision(true);

	if (false == ControllingPawn->IsActorTickEnabled())
		ControllingPawn->SetActorTickEnabled(true);

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParms(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(fRange),
		CollisionQueryParms
	);

	if (bResult)
	{
		for (auto& OverlapResult : OverlapResults)
		{
			APG_MyChar* MyCharacter = Cast<APG_MyChar>(OverlapResult.GetActor());
			if (MyCharacter && MyCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(APG_AICon_Mon::TargetKey, MyCharacter);
				//DrawDebugSphere(World, Center, fRange, 16, FColor::Green, false, 0.2f);
				//DrawDebugPoint(World, MyCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				//DrawDebugLine(World, ControllingPawn->GetActorLocation(), MyCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
				return;
			}
		}

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(APG_AICon_Mon::TargetKey, nullptr);
	}
	else
	{
		// Can't Find Character
		OwnerComp.GetBlackboardComponent()->ClearValue(APG_AICon_Mon::TargetKey);
	}

	//DrawDebugSphere(World, Center, fRange, 16, FColor::Red, false, 0.2f);
}