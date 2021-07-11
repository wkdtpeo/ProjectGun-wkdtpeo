// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <Components/ActorComponent.h>
#include "PG_ComponentDash.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDashEnd);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_GUN_API UPG_ComponentDash : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPG_ComponentDash();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//====================================================================
	// vDashTargetPos : 대쉬 할 타겟의 위치
	// fRunTime : 대쉬 최종 목표지점까지 이동시간
	//====================================================================
	bool SetupDashData(FVector vDashTargetPos, float fMoveDistance, float fDashSpeed);

	//====================================================================
	// StartDash() 함수 이용전 반드시 SetupDashData() 함수로 데이터 설정 후 사용할 것
	//====================================================================
	void StartDash();
	void StopDash();
	bool IsDash();

public :
	FOnDashEnd OnDashEnd;

private :
	void UpdateDashLocation(float fDeltaTime);

private :
	bool	m_bActiveDash = false;				// 대쉬중 여부
	float	m_fDashSpeed = 0.0f;
	FVector m_vDashDir = FVector::ZeroVector;	// 대쉬방향 (Nomalize)
	FVector m_vGoalPos = FVector::ZeroVector;	// 최종 이동 목표지점
};
