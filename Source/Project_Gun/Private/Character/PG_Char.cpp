// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PG_Char.h"
#include "Game/GameInstance/PG_GameInstance.h"

// Sets default values
APG_Char::APG_Char()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	{
		CharacterShadowComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SHADOWCOMPONENT"));

		static ConstructorHelpers::FObjectFinder<UStaticMesh>
			DEFAULT_SHADOW(TEXT("StaticMesh'/Game/PG/Meshs/Player/ShadowMesh.ShadowMesh'"));
		if (DEFAULT_SHADOW.Succeeded())
		{
			CharacterShadowComponent->SetStaticMesh(DEFAULT_SHADOW.Object);
			CharacterShadowComponent->SetupAttachment(GetMesh());
			CharacterShadowComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

// Called when the game starts or when spawned
void APG_Char::BeginPlay()
{
	Super::BeginPlay();
}

void APG_Char::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APG_Char::Destroyed()
{
	Super::Destroyed();
}

void APG_Char::FellOutOfWorld(const class UDamageType& dmgType)
{
	Super::FellOutOfWorld(dmgType);
}

// Called every frame
void APG_Char::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APG_Char::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool APG_Char::AssetLoadCharacter(FString AssetPath)
{
	auto PGGameInstance = Cast<UPG_GameInstance>(GetGameInstance());
	ABCHECK(nullptr != PGGameInstance, false);

	FSoftObjectPath CharacterAssetToLoad = AssetPath;
	AssetStreamingHandle = PGGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &APG_Char::OnAssetLoadCharacterCompleted));

	return true;
}

void APG_Char::Fire()
{

}

void APG_Char::OnAssetLoadCharacterCompleted()
{
	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());
	AssetStreamingHandle.Reset();

	ABCHECK(nullptr != AssetLoaded);
	ABCHECK(nullptr != GetMesh());

	GetMesh()->SetSkeletalMesh(AssetLoaded);
}