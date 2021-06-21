// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "BlueScriptObject/Weapon/PG_Weapon.h"
#include "PG_WeaponShotgun.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API APG_WeaponShotgun : public APG_Weapon
{
	GENERATED_BODY()
	
	enum 
	{
		eMax_ShotgunProjectile = 5,	// ���� �߻�� ���ÿ� �߻�Ǵ� ź�� ����
	};
	
public:
	APG_WeaponShotgun();

	virtual void PostInitializeComponents() override;
	virtual void Destroyed() override;
	
public:
	virtual void Fire() override;
	virtual void StopFire() override;

private:
	UPROPERTY()
	class APG_Projectile* CurrFireShutgun;		// ���� ����ִ� Shutgun Projectile

	UFUNCTION()
	virtual void OnFiringEffectEndParticle(UParticleSystemComponent* ParticleSystemComponent);
};
