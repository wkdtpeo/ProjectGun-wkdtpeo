// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "BlueScriptObject/Weapon/PG_Weapon.h"
#include "PG_WeaponLaserBemGun.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API APG_WeaponLaserBemGun : public APG_Weapon
{
	GENERATED_BODY()
	
public:
	APG_WeaponLaserBemGun();

	virtual void PostInitializeComponents() override;
	virtual void Destroyed() override;

public:
	virtual void Fire() override;
	virtual void StopFire() override;

private:
	UPROPERTY()
	class APG_Projectile* CurrFireLaserBeam;		// 현재 쏘고있는 Shutgun Projectile
	
};
