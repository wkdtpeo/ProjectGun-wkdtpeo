// Fill out your copyright notice in the Description page of Project Settings.

#include "BlueScriptObject/Weapon/PG_Weapon.h"
#include "BlueScriptObject/Projectile/PG_Projectile.h"
#include "Character/PG_MonChar.h"
#include "Character/PG_MyChar.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Game/GameMode/PG_GameModeBase.h"

// Sets default values
APG_Weapon::APG_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 캐릭터에 무기 부착
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	FiringEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FIRINGEFFECT"));
	FiringEffect->bAutoActivate = false;
	FireSound = CreateDefaultSubobject<UAudioComponent>(TEXT("FIRESOUND"));
	FireSound->bAutoActivate = false;

	RootComponent = Weapon;
	WeaponTableIndex = PG_INVALID_INDEX;
}

// Called when the game starts or when spawned
void APG_Weapon::BeginPlay()
{
	Super::BeginPlay();
}

void APG_Weapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APG_Weapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void APG_Weapon::Destroyed()
{
	Super::Destroyed();
}

// Called every frame
void APG_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
bool APG_Weapon::SetWeaponData(int32 nWeaponTableIndex)
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	auto pWeaponTableData = pGameInstance->GetWeaponTableData(nWeaponTableIndex);
	ABCHECK(nullptr != pWeaponTableData, false);

	WeaponTableIndex = nWeaponTableIndex;
	WeaponData = pWeaponTableData->WeaponData;
	MontageFireReaction = pWeaponTableData->MontageFireReaction;

	return true;
}

int32 APG_Weapon::GetAttackRange()
{
	return WeaponData.AttackRange;
}

UAnimMontage* APG_Weapon::GetMontageFireReaction()
{
	return MontageFireReaction;
}

//void APG_Weapon::SetWeaponData(const FPGWeaponData* pWeaponData)
//{
//	WeaponData = *pWeaponData;
//}

const FPGWeaponData* APG_Weapon::GetWeaponData()
{
	return &WeaponData;
}

int32 APG_Weapon::GetWeaponAttackDamage()
{
	auto pGameInstance = Cast<APG_GameModeBase>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameInstance, 0);

	return pGameInstance->GetMaxPower(WeaponTableIndex);
}

bool APG_Weapon::IsOwnerMonster()
{
	return (nullptr != Cast<APG_MonChar>(GetOwner()));
}

bool APG_Weapon::IsOwnerMyPlayer()
{
	return (nullptr != Cast<APG_MyChar>(GetOwner()));
}

void APG_Weapon::Fire()
{
}

void APG_Weapon::StopFire()
{

}