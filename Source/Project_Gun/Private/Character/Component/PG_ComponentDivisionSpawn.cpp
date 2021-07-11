// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Component/PG_ComponentDivisionSpawn.h"


// Sets default values for this component's properties
UPG_ComponentDivisionSpawn::UPG_ComponentDivisionSpawn()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPG_ComponentDivisionSpawn::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPG_ComponentDivisionSpawn::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

