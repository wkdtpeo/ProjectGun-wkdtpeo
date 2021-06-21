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
	// vDashTargetPos : �뽬 �� Ÿ���� ��ġ
	// fRunTime : �뽬 ���� ��ǥ�������� �̵��ð�
	//====================================================================
	bool SetupDashData(FVector vDashTargetPos, float fMoveDistance, float fDashSpeed);

	//====================================================================
	// StartDash() �Լ� �̿��� �ݵ�� SetupDashData() �Լ��� ������ ���� �� ����� ��
	//====================================================================
	void StartDash();
	void StopDash();
	bool IsDash();

public :
	FOnDashEnd OnDashEnd;

private :
	void UpdateDashLocation(float fDeltaTime);

private :
	bool	m_bActiveDash = false;				// �뽬�� ����
	float	m_fDashSpeed = 0.0f;
	FVector m_vDashDir = FVector::ZeroVector;	// �뽬���� (Nomalize)
	FVector m_vGoalPos = FVector::ZeroVector;	// ���� �̵� ��ǥ����
};
