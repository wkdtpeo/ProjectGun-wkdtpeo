// Fill out your copyright notice in the Description page of Project Settings.

#include "BlueScriptObject/Weapon/PG_WeaponFlameGun.h"
#include "BlueScriptObject/Projectile/PG_ProjectileFlame.h"
#include "Character/PG_MonChar.h"

APG_WeaponFlameGun::APG_WeaponFlameGun()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APG_WeaponFlameGun::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (FiringEffect)
	{
		FName SocketFire(TEXT("Socket_FiringEffect"));
		FiringEffect->AttachToComponent(Weapon, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketFire);
		FiringEffect->SetActive(false, true);
	}
}

void APG_WeaponFlameGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (nullptr != CurrFireFlame)
		FlameLength += DeltaTime;
}

void APG_WeaponFlameGun::Destroyed()
{
	RemoveCurrFireFlame();
	Super::Destroyed();
}

void APG_WeaponFlameGun::Fire()
{
	FName		SocketFire(TEXT("Socket_Fire_0"));
	FVector		vLocation = Weapon->GetSocketLocation(SocketFire);
	FRotator	rRotation = Weapon->GetSocketRotation(SocketFire);

	if (nullptr != CurrFireFlame)
	{
		RemoveCurrFireFlame();
	}

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// 총구 위치에 발사체를 스폰시킵니다.		
		CurrFireFlame = World->SpawnActor<APG_Projectile>(WeaponData.BProjectile, vLocation, rRotation, SpawnParams);
		ABCHECK(nullptr != CurrFireFlame);

		// 발사 방향을 알아냅니다.
		FVector LaunchDirection = rRotation.Vector();

		// 몬스터가 사용하는 무기 여부
		bool bMonsterWeapon = (nullptr != Cast<APG_MonChar>(GetOwner()));

		CurrFireFlame->FireInDirection(GetWeaponData(), LaunchDirection, bMonsterWeapon);
		CurrFireFlame->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		auto ProjectileFlame = Cast<APG_ProjectileFlame>(CurrFireFlame);
		if (ProjectileFlame)
			ProjectileFlame->SetScaleTime(FlameLength);
	}

	if (FiringEffect && false == FiringEffect->IsActive())
		FiringEffect->SetActive(true, true);

	if (FireSound && false == FireSound->IsActive())
		FireSound->Play();
}

void APG_WeaponFlameGun::StopFire()
{
	FlameLength = 0.0f;

	if (FiringEffect && FiringEffect->IsActive())
		FiringEffect->SetActive(false);

	if (FireSound && FireSound->IsActive())
		FireSound->Stop();

	RemoveCurrFireFlame();
}

void APG_WeaponFlameGun::RemoveCurrFireFlame()
{
	if (CurrFireFlame)
	{
		CurrFireFlame->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		CurrFireFlame->Destroy();
	}
}