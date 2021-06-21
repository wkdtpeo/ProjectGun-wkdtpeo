// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <Components/ActorComponent.h>
#include "PG_ComponentDivisionSpawn.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_GUN_API UPG_ComponentDivisionSpawn : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPG_ComponentDivisionSpawn();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//=================================================================================================
	// DivisionSpawnActor : Owner ���� ���� ������ ���� ������� nDvisionCount��ŭ ������ ���͸� Spawn ��Ų��.
	// nDvisionCount : ������ ���� ��
	// vRange : Spawn�� ���ʹ� Owner�� �������κ��� vRange ��ŭ �������� ��ġ�ȴ�. (=������ ��)
	//=================================================================================================
	template<class T>
	bool DivisionSpawnActor(int32 nDvisionCount, FVector vRange)
	{
		ABCHECK(0 < nDvisionCount, false);

		UWorld* World = GetWorld();
		ABCHECK(nullptr != World, false);

		auto Owner = Cast<T>(GetOwner());
		ABCHECK(nullptr != Owner, false);

		float fDivDgree = 360.0f / (float)nDvisionCount;
		FVector vRadius = Owner->GetActorForwardVector() * vRange;		// ���� ���� ������
		FVector vLocation = Owner->GetActorLocation();
		vLocation.Z += 100.0f;	// ������ ���Ͱ� �ϴÿ��� �������� ȿ���� �������� ������

		for (int n = 1; n <= nDvisionCount; n++)
		{
			FVector vSpawnPos = vLocation + vRadius.RotateAngleAxis(fDivDgree * n, FVector::UpVector);
			auto ASpawnActor = World->SpawnActor<T>(Owner->GetClass(), vSpawnPos, Owner->GetActorRotation());
			if (nullptr == ASpawnActor)
			{
				// �ݸ������� ���� �������� ���� ���ʹ� Owner �Ӹ� ���� ������ �� �ְ� �Ѵ�.
				// ���Ŀ� ���������� �̿��ؼ� �Ӹ� ���� �����Ǹ� �̲������鼭 ������ ������ ã�ƺ��� �� �� ����.
				vLocation.Z += 300.0f;
				ASpawnActor = World->SpawnActor<T>(Owner->GetClass(), vLocation, Owner->GetActorRotation());
			}
			//ABCHECK(nullptr != ASpawnActor, false);
			Owner->OnSpawnActor(ASpawnActor);
		}

		return true;
	}
};
