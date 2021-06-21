// Fill out your copyright notice in the Description page of Project Settings.

#include "BlueScriptObject/Target/PG_TargetSpot.h"
#include "Character/PG_MyChar.h"
#include "Game/GameState/PG_GameStateStage.h"

// Sets default values
APG_TargetSpot::APG_TargetSpot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	PortalEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PORTALEFFECT"));

	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);

	// 콜리전 설정
	Trigger->SetCollisionProfileName(TEXT("TargetSpot"));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &APG_TargetSpot::OnPlayerOverlap);

	Box->SetCollisionProfileName(TEXT("NoCollision"));

	// 파티클 설정
	PortalEffect->SetupAttachment(RootComponent);
	PortalEffect->bAutoActivate = true;
}

// Called when the game starts or when spawned
void APG_TargetSpot::BeginPlay()
{
	Super::BeginPlay();
	
	APG_GameStateStage* AGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	if (AGameStateStage)
		AGameStateStage->GateOpenDelegate.AddUObject(this, &APG_TargetSpot::OnGateOpen);
}
void APG_TargetSpot::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void APG_TargetSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APG_TargetSpot::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABLOG_S(Warning);

	// 스테이지 클리어 검사
	auto AMyCharacter = Cast<APG_MyChar>(OtherActor);
	if (AMyCharacter && GetWorld())
	{
		APG_GameStateStage* AGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
		if (AGameStateStage)
		{
			AGameStateStage->OnCheckStageClear();
		}
	}
}