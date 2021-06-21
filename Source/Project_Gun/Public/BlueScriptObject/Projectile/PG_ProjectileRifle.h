// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "BlueScriptObject/Projectile/PG_Projectile.h"
#include <GameFramework/Actor.h>
#include "PG_ProjectileRifle.generated.h"

UCLASS()
class PROJECT_GUN_API APG_ProjectileRifle : public APG_Projectile
{
	GENERATED_BODY()
	
public:	
	APG_ProjectileRifle();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;	// Called when the game starts or when spawned

public:	
	virtual void FireInDirection(const struct FPGWeaponData* pWeaponData, const FVector& ShootDirection, bool bMonsterWeapon = false) override;

protected:
	virtual void OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult) override;
	virtual void OnTargetHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void OnPlayEndParticle(UParticleSystemComponent* ParticleSystemComponent) override;
	
private :
	// ������Ʈ Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG ProjectileData", Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent*	ProjectileMeshComponent;

	// ������Ÿ�� �����Ʈ ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG ProjectileData", Meta = (AllowPrivateAccess = true))
	UProjectileMovementComponent* ProjectileMovementComponent;

	// �ǰݽ� ��Ÿ���� ����Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG ProjectileData", Meta = (AllowPrivateAccess = true))
	UParticleSystemComponent*	BurstingEffect;
};
