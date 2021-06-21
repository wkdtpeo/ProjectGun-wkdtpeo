// Fill out your copyright notice in the Description page of Project Settings.

#include "BlueScriptObject/Projectile/PG_Projectile.h"
#include "BlueScriptObject/Weapon/PG_Weapon.h"
#include "Character/PG_MonChar.h"
#include "Character/PG_MyChar.h"
#include "Define/PG_TableData.h"

// Sets default values
APG_Projectile::APG_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 구체를 단순 콜리전 표현으로 사용합니다.
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	// 구체의 콜리전 반경을 설정합니다.
	CollisionComponent->InitSphereRadius(50.0f);
	// 루트 컴포넌트를 콜리전 컴포넌트로 설정합니다.
	CollisionComponent->SetCollisionProfileName(TEXT("NoCollision"));

	RootComponent = CollisionComponent;

	vStartPos = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void APG_Projectile::BeginPlay()
{
	Super::BeginPlay();
}

void APG_Projectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void APG_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsEndBulletRange())
		Destroy();
}

// 프로젝타일의 속도를 발사 방향으로 초기화시키는 함수입니다.
void APG_Projectile::FireInDirection(const FPGWeaponData* pWeaponData, const FVector& ShootDirection, bool bMonsterWeapon)
{

}

bool APG_Projectile::IsEndBulletRange()
{
	if (0 == WeaponData.BulletRange)
		return false;	// 무한

	float fBulletRange = FMath::Abs<float>(FVector::Distance(vStartPos, GetActorLocation()));
	return (fBulletRange >= (float)WeaponData.BulletRange);
}

bool APG_Projectile::IsOwnerMonster()
{
	auto OwnerWeapon = Cast<APG_Weapon>(GetOwner());
	ABCHECK(nullptr != OwnerWeapon, false);

	return OwnerWeapon->IsOwnerMonster();
}
bool APG_Projectile::IsOwnerMyPlayer()
{
	auto OwnerWeapon = Cast<APG_Weapon>(GetOwner());
	ABCHECK(nullptr != OwnerWeapon, false);

	return OwnerWeapon->IsOwnerMyPlayer();
}

bool APG_Projectile::IsAttackNullity(FVector vTargetPos)
{
	FCollisionQueryParams Params(NAME_None, false, this);

	if (GetWorld()->LineTraceTestByProfile(
		GetActorLocation(),
		vTargetPos,
		TEXT("AttackNullity"),
		Params)
		)
	{
//#ifdef ENABLE_DRAW_DEBUG
//		DrawDebugPoint(GetWorld(), vTargetPos, 10.0f, FColor::Black, false, 1.02f);
//		DrawDebugLine(GetWorld(), GetActorLocation(), vTargetPos, FColor::Black, false, 1.02f);
//#endif
		return true;
	}

	return false;
}

void APG_Projectile::OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult)
{

}

void APG_Projectile::OnTargetHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}

void APG_Projectile::OnPlayEndParticle(UParticleSystemComponent* ParticleSystemComponent)
{

}
