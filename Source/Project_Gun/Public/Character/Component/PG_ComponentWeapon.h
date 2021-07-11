// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <Components/ActorComponent.h>
#include "PG_ComponentWeapon.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_GUN_API UPG_ComponentWeapon : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPG_ComponentWeapon();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//bool EquipWeapon(const TSubclassOf<class APG_Weapon>& WeaponType);
	bool EquipWeapon(int32 nWeaponTableIndex);
	void UnEquipWeapon();
	bool IsEquipWeapon();

	void Fire();
	void StopFire();

	float GetAttackRange();

public :
	void OnDestroy();
	
protected :
	UPROPERTY(EditAnywhere, Category = "PG Weapon")
	int32 WeaponTableIndex;

	UPROPERTY(VisibleAnywhere, Category = "PG WeaponType")
	TWeakObjectPtr<class APG_Weapon> CurrentWeapon = nullptr;
};
