// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <Components/ActorComponent.h>
#include "PG_ComponentCharScale.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_GUN_API UPG_ComponentCharScale : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPG_ComponentCharScale();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ������ ������ ������ �����Ѵ�.
	void SetActorScaleRange(FVector MinScale, FVector MaxScale);

	// SetActorScaleRange ���� ������ ������ �������� ������ 1�� ���, �Է��� ������� ������ ũ�⸦ �����Ѵ�. (Ratio : 0.0 ~ 1.0)
	void SetActorScale(float Ratio);

private:
	float m_fMinScale;
	float m_fMaxScale;

	FVector m_vMinScale;
	FVector m_vMaxScale;
};
