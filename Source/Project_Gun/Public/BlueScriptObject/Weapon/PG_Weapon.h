// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Define/PG_TableData.h"
#include <GameFramework/Actor.h>
#include "PG_Weapon.generated.h"

UCLASS()
class PROJECT_GUN_API APG_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APG_Weapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Destroyed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public :
	bool SetWeaponData(int32 nWeaponTableIndex);

	virtual int32				GetAttackRange();
	class UAnimMontage*			GetMontageFireReaction();
	const struct FPGWeaponData* GetWeaponData();
	int32						GetWeaponAttackDamage();	// 현재 캐릭터 무기 레벨 상태에 따른 무기 데미지를 반환한다.

	bool IsOwnerMonster();	// 몬스터 소유여부
	bool IsOwnerMyPlayer();	// 플레이어 소유여부

	// 발사 처리
	UFUNCTION()
	virtual void Fire();

	UFUNCTION()
	virtual void StopFire();

protected :
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG WeaponInfo", Meta = (AllowPrivateAccess = true))
	int32 WeaponTableIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG WeaponInfo", Meta = (AllowPrivateAccess = true))
	struct FPGWeaponData WeaponData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG WeaponInfo", Meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* Weapon;

	// 발사체가 생성될 때 해당 위치에 생성되는 이펙트(발사 총구 이펙트).
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG WeaponInfo", Meta = (AllowPrivateAccess = true))
	class UParticleSystemComponent*	FiringEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG WeaponInfo", Meta = (AllowPrivateAccess = true))
	class UAudioComponent* FireSound;

	// 무기 발사시 사용되는 애니메이션
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG WeaponInfo", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* MontageFireReaction;
};
