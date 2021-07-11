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
	// DivisionSpawnActor : Owner 액터 기준 주위로 원형 모양으로 nDvisionCount만큼 동일한 액터를 Spawn 시킨다.
	// nDvisionCount : 생성할 액터 수
	// vRange : Spawn될 액터는 Owner의 중점으로부터 vRange 만큼 떨어져서 배치된다. (=반지름 값)
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
		FVector vRadius = Owner->GetActorForwardVector() * vRange;		// 액터 스폰 범위값
		FVector vLocation = Owner->GetActorLocation();
		vLocation.Z += 100.0f;	// 생성된 액터가 하늘에서 떨어지는 효과를 내기위한 보정값

		for (int n = 1; n <= nDvisionCount; n++)
		{
			FVector vSpawnPos = vLocation + vRadius.RotateAngleAxis(fDivDgree * n, FVector::UpVector);
			auto ASpawnActor = World->SpawnActor<T>(Owner->GetClass(), vSpawnPos, Owner->GetActorRotation());
			if (nullptr == ASpawnActor)
			{
				// 콜리전으로 인해 생성되지 못한 액터는 Owner 머리 위로 스폰할 수 있게 한다.
				// 추후에 물리엔진을 이용해서 머리 위로 스폰되면 미끄러지면서 퍼지는 물리를 찾아봐야 할 것 같다.
				vLocation.Z += 300.0f;
				ASpawnActor = World->SpawnActor<T>(Owner->GetClass(), vLocation, Owner->GetActorRotation());
			}
			//ABCHECK(nullptr != ASpawnActor, false);
			Owner->OnSpawnActor(ASpawnActor);
		}

		return true;
	}
};
