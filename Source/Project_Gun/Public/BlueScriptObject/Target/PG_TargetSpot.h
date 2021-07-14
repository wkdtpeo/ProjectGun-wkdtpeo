// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"

#include <GameFramework/Actor.h>
#include "PG_TargetSpot.generated.h"


UCLASS()
class PROJECT_GUN_API APG_TargetSpot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APG_TargetSpot();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	UFUNCTION(BlueprintImplementableEvent)
	void OnGateOpen();
	UFUNCTION(BlueprintImplementableEvent)
	void OnGateClose();

public :
	UPROPERTY(VisibleAnywhere, Category = "PG TriggerBox")
	UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category = "PG TriggerBox")
	UStaticMeshComponent* Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PG TriggerBox")
	UParticleSystemComponent*	PortalEffect;
	
private :
	UFUNCTION()
	void OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
