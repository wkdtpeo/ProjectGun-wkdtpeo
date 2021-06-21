// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <GameFramework/Actor.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "PG_Projectile.generated.h"

UCLASS()
class PROJECT_GUN_API APG_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	APG_Projectile();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;	// Called when the game starts or when spawned

public:	
	virtual void Tick(float DeltaTime) override;	// Called every frame

	// �߻�ü�� �ӵ��� �߻� �������� �ʱ�ȭ��ŵ�ϴ�.
	virtual void FireInDirection(const struct FPGWeaponData* pWeaponData, const FVector& ShootDirection, bool bMonsterWeapon = false);

protected:
	bool IsEndBulletRange();	// �߻�ü�� �����ϴ� �ִ� ���� �˻�
	bool IsOwnerMonster();
	bool IsOwnerMyPlayer();
	bool IsAttackNullity(FVector vTargetPos);		// ���� ��ȿ���� �˻�

protected :
	UFUNCTION()
	virtual void OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnTargetHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnPlayEndParticle(UParticleSystemComponent* ParticleSystemComponent);

protected :
	// ��ü �ݸ��� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG ProjectileData", Meta = (AllowPrivateAccess = true))
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG ProjectileData", Meta = (AllowPrivateAccess = true))
	bool IsMonsterFire;

	UPROPERTY(VisibleAnywhere)
	struct FPGWeaponData  WeaponData;

	FVector vStartPos = FVector::ZeroVector;
};
