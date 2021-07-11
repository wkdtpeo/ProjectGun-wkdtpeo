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
		eMax_ShotgunProjectile = 5,	// 샷건 발사시 동시에 발사되는 탄약 개수
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
	class APG_Projectile* CurrFireShutgun;		// 현재 쏘고있는 Shutgun Projectile

	UFUNCTION()
	virtual void OnFiringEffectEndParticle(UParticleSystemComponent* ParticleSystemComponent);
};
