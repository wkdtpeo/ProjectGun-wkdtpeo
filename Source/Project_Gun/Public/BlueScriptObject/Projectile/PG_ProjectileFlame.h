// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "BlueScriptObject/Projectile/PG_Projectile.h"
#include "BlueScriptObject/Projectile/PG_Projectile.h"
#include "PG_ProjectileFlame.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API APG_ProjectileFlame : public APG_Projectile
{
	GENERATED_BODY()
	
public :
	APG_ProjectileFlame();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;	// Called when the game starts or when spawned
	virtual void Tick(float DeltaTime) override;	// Called every frame

public:
	virtual void FireInDirection(const struct FPGWeaponData* pWeaponData, const FVector& ShootDirection, bool bMonsterWeapon = false) override;

	void SetScaleTime(float fScaleTime);

private :
	void UpdateCollisionScale(float DeltaTime);

protected:
	virtual void OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult) override;
	virtual void OnPlayEndParticle(UParticleSystemComponent* ParticleSystemComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG ProjectileData", Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent*	CollisionMeshComponent;	// 화염 방사시 충돌 체크를 위한 Static Mesh

	float m_fScaleTime = 0.0f;
};
