// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <Animation/AnimInstance.h>
#include "PG_AnimIns_MyChar.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnFireDelegate);

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_AnimIns_MyChar : public UAnimInstance
{
	GENERATED_BODY()
	
public :
	UPG_AnimIns_MyChar();
	virtual void NativeUpdateAnimation(float DeletaSeconds) override;

	bool IsVaildTargetMonster();	// ���� Ÿ���� �Ǿ��ִ��� ����
	bool IsAimingComplete();		// ���ؿϷ� ����

	void SetRotation(float fYaw);
	void SetMontageFireReaction(UAnimMontage* AnimMontage);

public :
	UFUNCTION()
	void OnTargetMonster(class APG_MonChar* pTarget);

	

public :
	FOnFireDelegate OnFire;

	UFUNCTION()
	void AnimNotify_Fire();

private :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PG Pawn", Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PG Pawn", Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeedRatio;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PG Pawn", Meta = (AllowPrivateAccess = true))
	float fAimYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PG Pawn", Meta = (AllowPrivateAccess = true))
	float fAimPitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PG Pawn", Meta = (AllowPrivateAccess = true))
	class APG_MonChar* TargetMonster;

	// ���� �߻� ��Ÿ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PG Animation", Meta = (AllowPrivateAccess = true))
	UAnimMontage* MontageFireReaction;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	//bool IsDead;
};
