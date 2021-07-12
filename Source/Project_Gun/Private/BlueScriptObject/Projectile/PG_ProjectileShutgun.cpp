// Fill out your copyright notice in the Description page of Project Settings.

#include "BlueScriptObject/Projectile/PG_ProjectileShutgun.h"
#include "BlueScriptObject/Weapon/PG_Weapon.h"
#include "Character/PG_MonChar.h"
#include "Character/PG_MyChar.h"

APG_ProjectileShutgun::APG_ProjectileShutgun()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("COLLISIONMESHCOMPONENT"));
	CollisionMeshComponent->SetupAttachment(CollisionComponent);
	CollisionMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &APG_ProjectileShutgun::OnTargetBeginOverlap);
}

void APG_ProjectileShutgun::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ABCHECK(nullptr != CollisionMeshComponent);
	CollisionMeshComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionMeshComponent->SetGenerateOverlapEvents(true);
}

void APG_ProjectileShutgun::BeginPlay()
{
	Super::BeginPlay();

	ABCHECK(nullptr != CollisionMeshComponent);
	CollisionMeshComponent->SetVisibility(true);
}

void APG_ProjectileShutgun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCollisionScale(DeltaTime);
}

void APG_ProjectileShutgun::UpdateCollisionScale(float DeltaTime)
{
	if (CollisionMeshComponent)
	{
		m_fScaleTime += DeltaTime;

		// 샷건 범위가 완전히 펴질때까지의 목표시간
		float GoalSec = 0.2f;
		if (m_fScaleTime > GoalSec)
			m_fScaleTime = GoalSec;

		float fScale = (m_fScaleTime / GoalSec);
		FVector vScale(fScale, fScale, fScale);// = CollisionMeshComponent->GetComponentScale();
		CollisionMeshComponent->SetRelativeScale3D(vScale);

		if (1.0f <= fScale)
		{
			Destroy();
		}
	}
}

void APG_ProjectileShutgun::FireInDirection(const struct FPGWeaponData* pWeaponData, const FVector& ShootDirection, bool bMonsterWeapon)
{
	ABCHECK(nullptr != CollisionMeshComponent);
	WeaponData = *pWeaponData;

	FName CollisionName = bMonsterWeapon ? TEXT("Mon_Projectile") : TEXT("Projectile");
	if (CollisionMeshComponent)
	{
		CollisionMeshComponent->SetCollisionProfileName(CollisionName);
	}

	vStartPos = GetActorLocation();
}

void APG_ProjectileShutgun::SetScaleTime(float fScaleTime)
{
	m_fScaleTime = fScaleTime;
	UpdateCollisionScale(0.0f);
}

void APG_ProjectileShutgun::OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult)
{
	auto OwnerWeapon = Cast<APG_Weapon>(GetOwner());
	ABCHECK(nullptr != OwnerWeapon);

	// 몬스터 충돌시
	auto AMonCharacter = Cast<APG_MonChar>(OtherActor);
	if (AMonCharacter && IsOwnerMyPlayer())
		AMonCharacter->OnDamage(OwnerWeapon->GetWeaponAttackDamage());

	// 플레이어 충돌시
	auto AMyCharacter = Cast<APG_MyChar>(OtherActor);
	if (AMyCharacter&& IsOwnerMonster())
		AMyCharacter->OnAttackedByMonster();
}

void APG_ProjectileShutgun::OnPlayEndParticle(UParticleSystemComponent* ParticleSystemComponent)
{
	if (false == WeaponData.Collide)
		return;

	Destroy();
}

