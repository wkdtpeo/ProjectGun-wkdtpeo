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
	// ������Ÿ�� �߻縦 �õ��մϴ�.
	FVector		vLocation = Weapon->GetSocketLocation(SocketFire);
	FRotator	rRotation = Weapon->GetSocketRotation(SocketFire);
	UWorld*		World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// �ѱ� ��ġ�� �߻�ü�� ������ŵ�ϴ�.		
		auto Projectile = World->SpawnActor<APG_Projectile>(WeaponData.BProjectile, vLocation, rRotation, SpawnParams);
		if (Projectile)
		{
			// �߻� ������ �˾Ƴ��ϴ�.
			FVector LaunchDirection = rRotation.Vector();

			// ���Ͱ� ����ϴ� ���� ����
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