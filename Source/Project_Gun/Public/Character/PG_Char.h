// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Character/PG_CharDefine.h"

#include <GameFramework/Character.h>
#include "PG_Char.generated.h"

UCLASS()
class PROJECT_GUN_API APG_Char : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APG_Char();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Destroyed() override;
	// 액터가 월드 밖으로 나갔을 때 Call (Kill Z)
	virtual void FellOutOfWorld(const class UDamageType & dmgType) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool AssetLoadCharacter(FString AssetPath);

	// 발사 처리
	UFUNCTION()
	virtual void Fire();

	
protected :
	UFUNCTION()
	virtual void OnAssetLoadCharacterCompleted();

protected :
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "PG UI", Meta = (AllowPrivateAccess = true))
	class UWidgetComponent* HPBarWidget = nullptr;		// Debug HPBar

	// 캐릭터 그림자
	UPROPERTY(VisibleAnywhere, Category = "PG CharacterShadowInfo")
	class UStaticMeshComponent* CharacterShadowComponent;
};
