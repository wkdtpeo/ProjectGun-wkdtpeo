// Fill out your copyright notice in the Description page of Project Settings.

#include "BlueScriptObject/Projectile/PG_ProjectileFlame.h"
#include "Character/PG_MonChar.h"
#include "Character/PG_MyChar.h"
#include "Define/PG_TableData.h"

APG_ProjectileFlame::APG_ProjectileFlame()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("COLLISIONMESHCOMPONENT"));
	CollisionMeshComponent->SetupAttachment(CollisionComponent);
	CollisionMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &APG_ProjectileFlame::OnTargetBeginOverlap);
}

void APG_ProjectileFlame::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ABCHECK(nullptr != CollisionMeshComponent);
	CollisionMeshComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionMeshComponent->SetGenerateOverlapEvents(true);
}

void APG_ProjectileFlame::BeginPlay()
{
	Super::BeginPlay();

	ABCHECK(nullptr != CollisionMeshComponent);
	CollisionMeshComponent->SetVisibility(true);
	CollisionMeshComponent->SetRelativeScale3D(FVector(0.0f, 0.0f, 0.0f));
}

void APG_ProjectileFlame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCollisionScale(DeltaTime);
}

void APG_ProjectileFlame::UpdateCollisionScale(float DeltaTime)
{
	if (CollisionMeshComponent)
	{
		m_fScaleTime += DeltaTime;

		// ȭ������ ������ ������ ������������ ��ǥ�ð�
		float GoalSec = 1.0f;
		if (m_fScaleTime > GoalSec)
			m_fScaleTime = GoalSec;

		float fScale = (m_fScaleTime / GoalSec);
		FVector vScale(fScale, fScale, fScale);// = CollisionMeshComponent->GetComponentScale();
		CollisionMeshComponent->SetRelativeScale3D(vScale);
	}
}

void APG_ProjectileFlame::FireInDirection(const struct FPGWeaponData* pWeaponData, const FVector& ShootDirection, bool bMonsterWeapon)
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

void APG_ProjectileFlame::SetScaleTime(float fScaleTime)
{
	m_fScaleTime = fScaleTime;
	UpdateCollisionScale(0.0f);
}

void APG_ProjectileFlame::OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult)
{
	auto OwnerWeapon = Cast<APG_Weapon>(GetOwner());
	ABCHECK(nullptr != OwnerWeapon);

	// ���� �浹��
	auto AMonCharacter = Cast<APG_MonChar>(OtherActor);
	if (AMonCharacter && IsOwnerMyPlayer())
	{
		if (false == IsAttackNullity(AMonCharacter->GetActorLocation()))
		{
			FCollisionQueryParams Params(NAME_None, false, AMonCharacter);
			if (GetWorld()->LineTraceTestByProfile(
				GetActorLocation(),
				AMonCharacter->GetActorLocation(),
				TEXT("Monster"),
				Params)
				)
			{
				// ȭ�����⿡ �¾����� �� �տ� ���Ͱ� �������� ���� ��ȿȭ ó�� �ϵ��� �Ѵ�.
				return;
			}

			AMonCharacter->OnDamage(OwnerWeapon->GetWeaponAttackDamage());
		}
		
		return;
	}
	
	// �÷��̾� �浹��
	auto AMyCharacter = Cast<APG_MyChar>(OtherActor);
	if (AMyCharacter && IsOwnerMonster())
	{
		if (false == IsAttackNullity(AMyCharacter->GetActorLocation()))
		{
			AMyCharacter->OnAttackedByMonster();
		}
		return;
	}
}

void APG_ProjectileFlame::OnPlayEndParticle(UParticleSystemComponent* ParticleSystemComponent)
{
	if (false == WeaponData.Collide)
		return;

	Destroy();
}