// Fill out your copyright notice in the Description page of Project Settings.

#include "BlueScriptObject/Weapon/PG_WeaponLaserBemGun.h"
#include "BlueScriptObject/Projectile/PG_ProjectileLaserBem.h"
#include "Character/PG_MonChar.h"


APG_WeaponLaserBemGun::APG_WeaponLaserBemGun()
{

}

void APG_WeaponLaserBemGun::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (FiringEffect)
	{
		FName SocketFire(TEXT("Socket_FiringEffect"));
		FiringEffect->AttachToComponent(Weapon, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketFire);
		FiringEffect->SetActive(false, true);
	}
}

void APG_WeaponLaserBemGun::Destroyed()
{
	Super::Destroyed();
}

void APG_WeaponLaserBemGun::Fire()
{
	FName		SocketFire(TEXT("Socket_Fire_0"));
	FVector		vLocation = Weapon->GetSocketLocation(SocketFire);
	FRotator	rRotation = Weapon->GetSocketRotation(SocketFire);

	//if (nullptr != CurrFireLaserBeam)
	//{
	//	CurrFireLaserBeam->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	//	CurrFireLaserBeam->Destroy();
	//	CurrFireLaserBeam->Reset();
	//}

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// 총구 위치에 발사체를 스폰시킵니다.		
		CurrFireLaserBeam = World->SpawnActor<APG_Projectile>(WeaponData.BProjectile, vLocation, rRotation, SpawnParams);
		ABCHECK(nullptr != CurrFireLaserBeam);

		// 발사 방향을 알아냅니다.
		FVector LaunchDirection = rRotation.Vector();

		// 몬스터가 사용하는 무기 여부
		bool bMonsterWeapon = (nullptr != Cast<APG_MonChar>(GetOwner()));

		CurrFireLaserBeam->FireInDirection(GetWeaponData(), LaunchDirection, bMonsterWeapon);
		CurrFireLaserBeam->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	if (FiringEffect && false == FiringEffect->IsActive())
		FiringEffect->SetActive(true, true);
}

void APG_WeaponLaserBemGun::StopFire()
{
	if (FiringEffect && FiringEffect->IsActive())
		FiringEffect->SetActive(false);

	//if (CurrFireLaserBeam)
	//{
	//	CurrFireLaserBeam->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	//	CurrFireLaserBeam->Destroy();
	//	CurrFireLaserBeam->Reset();
	//}
}

