// Fill out your copyright notice in the Description page of Project Settings.

#include "BlueScriptObject/Weapon/PG_WeaponShotgun.h"
#include "BlueScriptObject/Projectile/PG_ProjectileShutgun.h"
#include "Character/PG_MonChar.h"


APG_WeaponShotgun::APG_WeaponShotgun()
{

}

void APG_WeaponShotgun::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (FiringEffect)
	{
		FName SocketFire(TEXT("Socket_FiringEffect"));
		FiringEffect->AttachToComponent(Weapon, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketFire);
		FiringEffect->SetActive(false, true);
		FiringEffect->bAutoActivate = false;
	}
}

void APG_WeaponShotgun::Destroyed()
{
	Super::Destroyed();
}

void APG_WeaponShotgun::Fire()
{
	for (int32 nIndex = 0; nIndex < eMax_ShotgunProjectile; ++nIndex)
	{
		FName		SocketFire(*FString::Printf(TEXT("Socket_Fire_%d"), nIndex));
		FVector		vLocation = Weapon->GetSocketLocation(SocketFire);
		FRotator	rRotation = Weapon->GetSocketRotation(SocketFire);

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// 총구 위치에 발사체를 스폰시킵니다.		
			CurrFireShutgun = World->SpawnActor<APG_Projectile>(WeaponData.BProjectile, vLocation, rRotation, SpawnParams);
			ABCHECK(nullptr != CurrFireShutgun);

			// 발사 방향을 알아냅니다.
			FVector LaunchDirection = rRotation.Vector();

			// 몬스터가 사용하는 무기 여부
			bool bMonsterWeapon = (nullptr != Cast<APG_MonChar>(GetOwner()));

			CurrFireShutgun->FireInDirection(GetWeaponData(), LaunchDirection, bMonsterWeapon);
			//CurrFireShutgun->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}

	auto pFiringEffect = NewObject<UParticleSystemComponent>(Weapon, UParticleSystemComponent::StaticClass());
	if (pFiringEffect)
	{
		FName SocketFire(TEXT("Socket_FiringEffect"));
		FVector		vLocation = Weapon->GetSocketLocation(SocketFire);
		FRotator	rRotation = Weapon->GetSocketRotation(SocketFire);

		pFiringEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FiringEffect->Template, vLocation, rRotation, false);
		pFiringEffect->bAutoActivate = false;
		pFiringEffect->bAutoDestroy = true;
		pFiringEffect->Activate(true);
		pFiringEffect->OnSystemFinished.AddDynamic(this, &APG_WeaponShotgun::OnFiringEffectEndParticle);
	}

	if (FireSound)
		FireSound->Play();
}

void APG_WeaponShotgun::StopFire()
{
	//if (FiringEffect && FiringEffect->IsActive())
	//	FiringEffect->SetActive(false);
}

void APG_WeaponShotgun::OnFiringEffectEndParticle(UParticleSystemComponent* ParticleSystemComponent)
{
	//if (ParticleSystemComponent)
	//	ParticleSystemComponent->DestroyComponent();
}