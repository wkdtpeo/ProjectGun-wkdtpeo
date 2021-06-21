// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Component/PG_ComponentCharScale.h"

// Sets default values for this component's properties
UPG_ComponentCharScale::UPG_ComponentCharScale()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPG_ComponentCharScale::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UPG_ComponentCharScale::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPG_ComponentCharScale::SetActorScaleRange(FVector MinScale, FVector MaxScale)
{
	m_vMinScale = MinScale;
	m_vMaxScale = MaxScale;
}

void UPG_ComponentCharScale::SetActorScale(float Ratio)
{
	ABCHECK(0 <= Ratio && Ratio <= 1.0f);

	FVector vNormal = m_vMaxScale - m_vMinScale;
	FVector vScale = m_vMinScale + (vNormal * FMath::Clamp<float>(Ratio, 0.0f, 1.0f));

	auto OwnerActor = GetOwner();
	if (OwnerActor)
		OwnerActor->SetActorRelativeScale3D(vScale);
}