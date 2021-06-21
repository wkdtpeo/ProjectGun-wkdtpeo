// Fill out your copyright notice in the Description page of Project Settings.

#include "BlueScriptObject/Projectile/PG_ProjectileRifle.h"
#include "Character/PG_MonChar.h"
#include "Character/PG_MyChar.h"
#include "Define/PG_TableData.h"

// Sets default values
APG_ProjectileRifle::APG_ProjectileRifle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	ProjectileMeshComponent->SetupAttachment(CollisionComponent);
	ProjectileMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

	// �� ������Ʈ�� ����Ͽ� �� ������Ÿ���� �����Ʈ�� ������ŵ�ϴ�.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);

	// ��ü�� �浹�� �߻��Ǵ� �ǰ� ����Ʈ
	BurstingEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BurstingEffect"));
	BurstingEffect->SetupAttachment(CollisionComponent);
	BurstingEffect->SetCollisionProfileName(TEXT("NoCollision"));
	BurstingEffect->bAutoActivate = false;
}

void APG_ProjectileRifle::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ABCHECK(nullptr != ProjectileMovementComponent);
	ProjectileMovementComponent->InitialSpeed = 0.0f;
	ProjectileMovementComponent->MaxSpeed = 0.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = 0.3f;

	// �÷��̾� ������ �̺�Ʈ ����
	ABCHECK(nullptr != CollisionComponent);
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APG_ProjectileRifle::OnTargetBeginOverlap);
	CollisionComponent->OnComponentHit.AddDynamic(this, &APG_ProjectileRifle::OnTargetHit);

	// �Ѿ� �浹 ����Ʈ ����
	ABCHECK(nullptr != BurstingEffect);
	BurstingEffect->SetActive(false, true);
	BurstingEffect->OnSystemFinished.AddDynamic(this, &APG_ProjectileRifle::OnPlayEndParticle);
}

// Called when the game starts or when spawned
void APG_ProjectileRifle::BeginPlay()
{
	Super::BeginPlay();
}

// ������Ÿ���� �ӵ��� �߻� �������� �ʱ�ȭ��Ű�� �Լ��Դϴ�.
void APG_ProjectileRifle::FireInDirection(const FPGWeaponData* pWeaponData, const FVector& ShootDirection, bool bMonsterWeapon)
{
	ABCHECK(nullptr != ProjectileMovementComponent);
	WeaponData = *pWeaponData;

	ProjectileMovementComponent->InitialSpeed = WeaponData.BulletSpeed;
	ProjectileMovementComponent->MaxSpeed = WeaponData.BulletSpeed;
	ProjectileMovementComponent->Velocity = ShootDirection * WeaponData.BulletSpeed;

	{
		FName CollisionName = bMonsterWeapon ? TEXT("Mon_Projectile") : TEXT("Projectile");
		if (CollisionComponent)
			CollisionComponent->SetCollisionProfileName(CollisionName);

		//if (ProjectileMeshComponent)
		//	ProjectileMeshComponent->SetCollisionProfileName(CollisionName);

		//if (BurstingEffect)
		//	BurstingEffect->SetCollisionProfileName(CollisionName);
	}

	vStartPos = GetActorLocation();
}

void APG_ProjectileRifle::OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult)
{
	auto OwnerWeapon = Cast<APG_Weapon>(GetOwner());
	ABCHECK(nullptr != OwnerWeapon);

	// ���� �浹��
	auto AMonCharacter = Cast<APG_MonChar>(OtherActor);
	if (AMonCharacter && IsOwnerMyPlayer())
		AMonCharacter->OnDamage(OwnerWeapon->GetWeaponAttackDamage());

	// �÷��̾� �浹��
	auto AMyCharacter = Cast<APG_MyChar>(OtherActor);
	if (AMyCharacter&& IsOwnerMonster())
		AMyCharacter->OnAttackedByMonster();

	if (BurstingEffect && false == BurstingEffect->IsActive())
		BurstingEffect->SetActive(true, true);

	if (WeaponData.Collide)
	{
		ProjectileMovementComponent->StopMovementImmediately();
		ProjectileMeshComponent->SetVisibility(false);
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void APG_ProjectileRifle::OnTargetHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (BurstingEffect && false == BurstingEffect->IsActive())
		BurstingEffect->SetActive(true, true);
}

void APG_ProjectileRifle::OnPlayEndParticle(UParticleSystemComponent* ParticleSystemComponent)
{
	if (false == WeaponData.Collide)
		return;

	Destroy();
}
