// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "BlueScriptObject/Weapon/PG_Weapon.h"
#include "BlueScriptObject/Projectile/PG_ProjectileFlame.h"

#include "PG_WeaponFlameGun.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API APG_WeaponFlameGun : public APG_Weapon
{
	GENERATED_BODY()
	
public:
	APG_WeaponFlameGun();

	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

public :
	virtual void Fire() override;
	virtual void StopFire() override;

private :
	void RemoveCurrFireFlame();

private :
	UPROPERTY()
	class APG_Projectile* CurrFireFlame;		// 현재 화염 방사중인 Flame Projectile

	float FlameLength = 0.0f;
};
