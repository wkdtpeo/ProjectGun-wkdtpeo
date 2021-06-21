// Fill out your copyright notice in the Description page of Project Settings.

#include "BlueScriptObject/Weapon/PG_WeaponRifle.h"
#include "BlueScriptObject/Projectile/PG_Projectile.h"
#include "Character/PG_MonChar.h"
#include <Engine/StaticMeshSocket.h>

APG_WeaponRifle::APG_WeaponRifle()
{
}

void APG_WeaponRifle::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (FiringEffect)
	{
		FName SocketFire(TEXT("Socket_FiringEffect"));
		FiringEffect->AttachToComponent(Weapon, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketFire);
		FiringEffect->SetActive(false, true);
	}
}

void APG_WeaponRifle::Fire()
{
	FName SocketFire(TEXT("Socket_Fire_0"));
	// 프로젝타일 발사를 시도합니다.
	FVector		vLocation = Weapon->GetSocketLocation(SocketFire);
	FRotator	rRotation = Weapon->GetSocketRotation(SocketFire);
	UWorld*		World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// 총구 위치에 발사체를 스폰시킵니다.		
		auto Projectile = World->SpawnActor<APG_Projectile>(WeaponData.BProjectile, vLocation, rRotation, SpawnParams);
		if (Projectile)
		{
			// 발사 방향을 알아냅니다.
			FVector LaunchDirection = rRotation.Vector();

			// 몬스터가 사용하는 무기 여부
			bool bMonsterWeapon = (nullptr != Cast<APG_MonChar>(GetOwner()));

			Projectile->FireInDirection(GetWeaponData(), LaunchDirection, bMonsterWeapon);
		}
	}

	if (FiringEffect && false == FiringEffect->IsActive())
		FiringEffect->SetActive(true, true);

	if (FireSound)
		FireSound->Play();

}

void APG_WeaponRifle::StopFire()
{
	if (FiringEffect && FiringEffect->IsActive())
		FiringEffect->SetActive(false);
}