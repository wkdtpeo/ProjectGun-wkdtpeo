// Fill out your copyright notice in the Description page of Project Settings.

#include "BlueScriptObject/LethalAttack/PG_LethalAttack.h"
#include "Player/PlayerState/PG_MyPlayerState.h"
#include "Player/PlayerController/PG_PlayerController.h"
#include "Character/PG_MonChar.h"
#include "Character/PG_MyChar.h"
#include "Game/GameState/PG_GameStateStage.h"

#include <Components/AudioComponent.h>
//#include <DrawDebugHelpers.h>

// Sets default values
APG_LethalAttack::APG_LethalAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->SetCollisionProfileName(TEXT("CharacterSkill"));

	RootComponent = CollisionComponent;

	LShowingEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LShowingEffect"));
	LShowingEffect->SetupAttachment(CollisionComponent);
	LShowingEffect->SetCollisionProfileName(TEXT("CharacterSkill"));
	LShowingEffect->bAutoActivate = false;

	LEffectVisual = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LEffectVisual"));
	LEffectVisual->SetupAttachment(CollisionComponent);
	LEffectVisual->SetCollisionProfileName(TEXT("CharacterSkill"));
	LEffectVisual->OnSystemFinished.AddDynamic(this, &APG_LethalAttack::OnEndEffectVisual);
	LEffectVisual->bAutoActivate = false;

	LethalSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("LethalSoundComponent"));
	LethalSoundComponent->bAutoActivate = false;

	LethalPoint = 0.0f;
}

void APG_LethalAttack::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (LShowingEffect)
		LShowingEffect->SetVisibility(false);

	if (LEffectVisual)
		LEffectVisual->SetVisibility(false);
}

// Called when the game starts or when spawned
void APG_LethalAttack::BeginPlay()
{
	Super::BeginPlay();

	OnInitLethal();
}

// Called every frame
void APG_LethalAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

//#ifdef ENABLE_DRAW_DEBUG
//	float fAttackRange = GetAttackRange();
//
//	FVector TraceVec = GetActorForwardVector() * fAttackRange;
//	FVector Center = GetActorLocation();
//	float HalfHeight = fAttackRange;
//	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
//	FColor DrawColor = LethalPoint < LethalAttackData.LMinPoint ? FColor::Purple : FColor::Emerald;
//	float DebugLifeTime = 0.03f;
//
//	DrawDebugCapsule(GetWorld(),
//		Center,
//		HalfHeight,
//		fAttackRange,
//		CapsuleRot,
//		DrawColor,
//		false,
//		DebugLifeTime);
//#endif
}


void APG_LethalAttack::BindPlayerState(class APG_MyPlayerState* APlayerState)
{
	ABCHECK(nullptr != APlayerState);

	HandleOnBasicAttackMonsterKill = APlayerState->OnDefaultAttackMonsterKill.AddUObject(this, &APG_LethalAttack::OnAddLethalPoint);
	HandleOnReadyToUseTheLethalAttack = APlayerState->OnReadyToUseTheLethalAttack.AddUObject(this, &APG_LethalAttack::OnReadyToUseTheLethalAttack);
	HandleOnUseTheLethalAttackSuccess = APlayerState->OnTryUseTheLethalAttack.AddUObject(this, &APG_LethalAttack::OnTryUseTheLethalAttack);
	HandleOnUseTheLethalAttackFail = APlayerState->OnUseTheLethalAttackFail.AddUObject(this, &APG_LethalAttack::OnUseTheLethalAttackFail);
}

void APG_LethalAttack::UnBindPlayerState(class APG_MyPlayerState* APlayerState)
{
	ABCHECK(nullptr != APlayerState);

	APlayerState->OnDefaultAttackMonsterKill.Remove(HandleOnBasicAttackMonsterKill);
	APlayerState->OnReadyToUseTheLethalAttack.Remove(HandleOnReadyToUseTheLethalAttack);
	APlayerState->OnTryUseTheLethalAttack.Remove(HandleOnUseTheLethalAttackSuccess);
	APlayerState->OnUseTheLethalAttackFail.Remove(HandleOnUseTheLethalAttackFail);
}

void APG_LethalAttack::SetLethaAttackData(const FPGLethalAttackData* pLethalAttackData)
{
	LethalAttackData = *pLethalAttackData;
}

bool APG_LethalAttack::IsReadyLethal()
{
	return (LethalPoint >= LethalAttackData.LMinPoint);
}

float APG_LethalAttack::GetAttackRange()
{
	// 필살기의 타격 범위는 필살기가 가지고 있는 "필살기 최대 범위(LMaxRange)"를 필살기의 "필살기 최대 킬(LMaxPoint)"로 나눈 뒤 
	// 플레이어의 "필살기 현재 수치(LethalPoint)"를 곱해준다.
	// Ex) StrongWindAttack 필살기(LmaxRange: 300, LMaxPoint: 100)를 사용할 때, 
	// 플레이어의 "필살기 현재 수치(LethalPoint)"가 60이면 해당 필살기가 가동될때 범위는 180이다. 
	return (float)(LethalAttackData.LMaxRange / LethalAttackData.LMaxPoint) * (float)LethalPoint;
}

float APG_LethalAttack::GetRatioAttackRange()
{
	return GetAttackRange() / (float)LethalAttackData.LMaxRange;
}

bool APG_LethalAttack::FindingDamagedMonsters(OUT TArray<class APG_MonChar*>& GetMonsters)
{
	float fAttackRange = GetAttackRange();

	TArray<FHitResult> HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepMultiByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(fAttackRange),
		Params);

//#ifdef ENABLE_DRAW_DEBUG
//	FVector TraceVec = GetActorForwardVector() * fAttackRange;
//	FVector Center = GetActorLocation();
//	float HalfHeight = fAttackRange;
//	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
//	FColor DrawColor = FColor::Orange;
//	float DebugLifeTime = 1.0f;
//
//	DrawDebugCapsule(GetWorld(),
//		Center,
//		HalfHeight,
//		fAttackRange,
//		CapsuleRot,
//		DrawColor,
//		false,
//		DebugLifeTime);
//#endif

	if (bResult)
	{
		for (auto Monster : HitResult)
		{
			auto AMonster = Cast<class APG_MonChar>(Monster.GetActor());
			if (AMonster)
				GetMonsters.Add(AMonster);
		}
	}

	return bResult;
}


void APG_LethalAttack::OnReadyToUseTheLethalAttack()
{
	if (LShowingEffect && false == LShowingEffect->IsVisible())
	{
		float fScale = GetAttackRange();
		LShowingEffect->SetRelativeScale3D(FVector(fScale, fScale, fScale));
		LShowingEffect->SetActive(true, true);
		LShowingEffect->SetVisibility(true);
	}
}

void APG_LethalAttack::OnUseTheLethalAttackFail()
{

}

void APG_LethalAttack::OnTryUseTheLethalAttack()
{
	if (LethalPoint < LethalAttackData.LMinPoint)
		return;

	if (LShowingEffect && LShowingEffect->IsVisible())
	{
		LShowingEffect->SetVisibility(false);
	}

	if (LEffectVisual && false == LEffectVisual->IsVisible())
	{
		float fScale = GetAttackRange() / 100.0f;
		LEffectVisual->SetRelativeScale3D(FVector(fScale, fScale, fScale));
		LEffectVisual->SetActive(true, true);
		LEffectVisual->SetVisibility(true);
	}

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	TArray<class APG_MonChar*> DamagedMonsters;
	FindingDamagedMonsters(DamagedMonsters);
	for (auto AMonster : DamagedMonsters)
	{
		if (AMonster && false == AMonster->IsActorBeingDestroyed())
		{
			ABLOG(Warning, TEXT("DamgeMonster : %s"), *AMonster->GetName());
			if (AMonster->OnDamage(LethalAttackData.LPower, true))
			{
				MaxLethalKill += 1;
			}
		}
	}

	auto AMyChar = Cast<APG_MyChar>(GetOwner());
	if (AMyChar)
		AMyChar->UnEquipLethalAttack();

	auto pGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	if (pGameStateStage)
		pGameStateStage->SetMaxLethalKill(MaxLethalKill);

	if (LethalSoundComponent)
		LethalSoundComponent->Play();
}

void APG_LethalAttack::OnAddLethalPoint(class APG_MonChar* AMonster)
{
	if (LethalPoint > LethalAttackData.LMaxPoint)
		return;

	LethalPoint++;

	if (LShowingEffect && LShowingEffect->IsVisible())
	{
		float fScale = GetAttackRange() / 100.0f;
		LShowingEffect->SetRelativeScale3D(FVector(fScale, fScale, fScale));
	}

	if (false == m_bReadyLethal && IsReadyLethal())
	{
		m_bReadyLethal = true;
		OnReadyLethal();
	}
}

void APG_LethalAttack::OnEndEffectVisual(UParticleSystemComponent* ParticleSystemComponent)
{
	Destroy();
}

void APG_LethalAttack::OnBegineOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto AMonCharacter = Cast<APG_MonChar>(OtherActor);
	if (AMonCharacter)
	{
		ABLOG(Warning, TEXT("Lethal Damage : %s"), *AMonCharacter->GetName());
		AMonCharacter->OnDamage(LethalAttackData.LPower, true);
	}
}

void APG_LethalAttack::OnParticleCollide(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity, FVector Direction, FVector Normal, FName BoneName, UPhysicalMaterial* PhysMat)
{

}

void APG_LethalAttack::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	auto AMonCharacter = Cast<APG_MonChar>(OtherActor);
	if (AMonCharacter)
	{
		ABLOG(Warning, TEXT("Lethal Damage : %s"), *AMonCharacter->GetName());
		AMonCharacter->OnDamage(LethalAttackData.LPower, true);
	}
}