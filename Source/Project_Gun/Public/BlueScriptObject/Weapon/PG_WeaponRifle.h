// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "BlueScriptObject/Weapon/PG_Weapon.h"
#include "PG_WeaponRifle.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API APG_WeaponRifle : public APG_Weapon
{
	GENERATED_BODY()
	
public :
	APG_WeaponRifle();
	
	virtual void PostInitializeComponents() override;

public :
	virtual void Fire() override;
	virtual void StopFire() override;
};
