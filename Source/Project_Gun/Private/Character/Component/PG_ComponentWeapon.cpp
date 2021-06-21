// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Component/PG_ComponentWeapon.h"
#include "Character/PG_Char.h"
#include "BlueScriptObject/Weapon/PG_Weapon.h"
#include "Game/GameInstance/PG_GameInstance.h"

// Sets default values for this component's properties
UPG_ComponentWeapon::UPG_ComponentWeapon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPG_ComponentWeapon::BeginPlay()
{
	Super::BeginPlay();

	EquipWeapon(WeaponTableIndex);
}

// Called every frame
void UPG_ComponentWeapon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
bool UPG_ComponentWeapon::EquipWeapon(int32 nWeaponTableIndex)
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	auto pWeaponTableData = pGameInstance->GetWeaponTableData(WeaponTableIndex);
	ABCHECK(nullptr != pWeaponTableData, false);

	auto Owner = Cast<APG_Char>(GetOwner());
	ABCHECK(nullptr != Owner, false);

	APG_Weapon* NewWeapon = GetWorld()->SpawnActor<APG_Weapon>(pWeaponTableData->BPWeapon, FVector::ZeroVector, FRotator::ZeroRotator);
	ABCHECK(nullptr != NewWeapon, false);
	ABCHECK(NewWeapon->SetWeaponData(nWeaponTableIndex), false)

	if (CurrentWeapon.IsValid(true))
		UnEquipWeapon();

	FName MonsterGunSocket(TEXT("Bip001-L-GunSocket"));
	NewWeapon->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, MonsterGunSocket);
	NewWeapon->SetOwner(Owner);

	CurrentWeapon = NewWeapon;
	
	return true;
}
//
//bool UPG_ComponentWeapon::EquipWeapon(const TSubclassOf<class APG_Weapon>& WeaponType)
//{
//	auto Owner = Cast<APG_Char>(GetOwner());
//	ABCHECK(nullptr != Owner, false);
//
//	APG_Weapon* NewWeapon = GetWorld()->SpawnActor<APG_Weapon>(WeaponType, FVector::ZeroVector, FRotator::ZeroRotator);
//	ABCHECK(nullptr != NewWeapon, false);
//
//	if (CurrentWeapon.IsValid(true))
//		UnEquipWeapon();
//
//	FName MonsterGunSocket(TEXT("Bip001-L-GunSocket"));
//	NewWeapon->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, MonsterGunSocket);
//	NewWeapon->SetOwner(Owner);
//
//	CurrentWeapon = NewWeapon;
//	CurrentWeapon->SetWeaponData(nWeaponTalbeIndex)
//	return true;
//}

void UPG_ComponentWeapon::UnEquipWeapon()
{
	if (CurrentWeapon.IsValid(true))
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}
}

 bool UPG_ComponentWeapon::IsEquipWeapon()
{
	 return CurrentWeapon.IsValid(true);
}

void UPG_ComponentWeapon::Fire()
{
	ABCHECK(CurrentWeapon.Get());
	CurrentWeapon->Fire();
}

void UPG_ComponentWeapon::StopFire()
{
	ABCHECK(CurrentWeapon.IsValid(true));
	CurrentWeapon->StopFire();
}

float UPG_ComponentWeapon::GetAttackRange()
{
	return CurrentWeapon.IsValid(true) ? CurrentWeapon->GetAttackRange() : 0.0f;
}

void UPG_ComponentWeapon::OnDestroy()
{
	UnEquipWeapon();
}