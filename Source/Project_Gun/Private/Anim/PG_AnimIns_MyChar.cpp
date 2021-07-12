// Fill out your copyright notice in the Description page of Project Settings.

#include "Anim/PG_AnimIns_MyChar.h"
#include "Character/PG_MyChar.h"
#include "Character/PG_MonChar.h"

#include <DrawDebugHelpers.h>

UPG_AnimIns_MyChar::UPG_AnimIns_MyChar()
{
	CurrentPawnSpeed = 0.0f;
	CurrentPawnSpeedRatio = 0.0f;
	fAimYaw = 0.0f;
	fAimPitch = 0.0f;
	TargetMonster = nullptr;
}

void UPG_AnimIns_MyChar::NativeUpdateAnimation(float DeletaSeconds)
{
	Super::NativeUpdateAnimation(DeletaSeconds);

	auto Pawn = Cast<APG_MyChar>(TryGetPawnOwner());
	if (Pawn)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();

		if (CurrentPawnSpeed > 0.0f)
			CurrentPawnSpeedRatio = CurrentPawnSpeed / Pawn->GetMaxSpeed();
		else
			CurrentPawnSpeedRatio = 0.0f;
	}
}

void UPG_AnimIns_MyChar::SetRotation(float fYaw)
{
	fAimYaw = fYaw;
	fAimPitch = fYaw;
}

void UPG_AnimIns_MyChar::SetMontageFireReaction(UAnimMontage* AnimMontage)
{
	ABCHECK(nullptr != AnimMontage);
	MontageFireReaction = AnimMontage;
}

bool UPG_AnimIns_MyChar::IsVaildTargetMonster()
{
	return TargetMonster ? true : false;
}

bool UPG_AnimIns_MyChar::IsAimingComplete()
{
	ABCHECK(nullptr != MontageFireReaction, false);

	FAnimMontageInstance* pAnimMontageInstance = GetActiveInstanceForMontage(MontageFireReaction);
	if (pAnimMontageInstance && pAnimMontageInstance->bPlaying)
	{
		float fBlendTime = pAnimMontageInstance->GetWeight();
		//FString DebugMsg = FString::Printf(TEXT("BlendTime : %f"), fBlendTime);
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, DebugMsg);

		return (0.9f <= fBlendTime);
	}

	return false;
}

void UPG_AnimIns_MyChar::OnTargetMonster(APG_MonChar* pTarget)
{
	TargetMonster = pTarget;
}

void UPG_AnimIns_MyChar::AnimNotify_Fire()
{
	//ABLOG(Warning, TEXT("Fire!"));
	OnFire.Broadcast();
}

