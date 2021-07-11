// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Component/PG_ComponentDash.h"
#include "Character/PG_Char.h"

// Sets default values for this component's properties
UPG_ComponentDash::UPG_ComponentDash()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPG_ComponentDash::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPG_ComponentDash::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateDashLocation(DeltaTime);
}

bool UPG_ComponentDash::SetupDashData(FVector vDashTargetPos, float fMoveDistance, float fDashSpeed)
{
	auto AOwner = Cast<APG_Char>(GetOwner());
	ABCHECK(nullptr != AOwner, false);
	ABCHECK(false == vDashTargetPos.IsZero(), false);

	m_fDashSpeed = fDashSpeed;
	// 대쉬 방향 계산
	m_vDashDir = vDashTargetPos - AOwner->GetActorLocation();
	m_vDashDir.Normalize();

	m_vGoalPos = AOwner->GetActorLocation() + (m_vDashDir * fMoveDistance);

	// 타겟 방향으로 몬스터 회전
	AOwner->SetActorRotation(m_vDashDir.Rotation());

	return true;
}

void UPG_ComponentDash::StartDash()
{
	m_bActiveDash = true;
}

void UPG_ComponentDash::StopDash()
{
	m_bActiveDash = false;
	m_fDashSpeed = 0.0f;
	m_vDashDir = FVector::ZeroVector;
	m_vGoalPos = FVector::ZeroVector;	// 최종 이동 목표지점
	OnDashEnd.Broadcast();
}

bool UPG_ComponentDash::IsDash()
{
	return m_bActiveDash;
}

void UPG_ComponentDash::UpdateDashLocation(float fDeltaTime)
{
	if (false == m_bActiveDash)
		return;

	auto AOwner = Cast<APG_Char>(GetOwner());
	if (nullptr == AOwner)
	{
		m_bActiveDash = false;
		ABCHECK(nullptr != AOwner);
	}

	FVector vLocation = AOwner->GetActorLocation();
	FVector vPos = FMath::VInterpTo(vLocation, m_vGoalPos, fDeltaTime, m_fDashSpeed);
	// 어느정도 목표지점에 들어오면 대쉬가 끝났다는걸 알린다.
	if (10 >= FMath::Abs<float>(FVector::Distance(vLocation, m_vGoalPos)))
	{
		OnDashEnd.Broadcast();
		return;
	}

	AOwner->SetActorLocation(vPos);
	AOwner->AddMovementInput(m_vDashDir);
}
