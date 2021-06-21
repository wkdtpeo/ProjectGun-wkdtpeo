// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PG_MonCharSkillType.h"
#include "Character/PG_MonChar.h"
#include "Character/PG_MyChar.h"
#include "AI/PG_AICon_Mon.h"
#include "BlueScriptObject/Weapon/PG_Weapon.h"
#include "Character/Component/PG_ComponentWeapon.h"
#include "Character/Component/PG_ComponentDash.h"
#include "Character/Component/PG_ComponentDivisionSpawn.h"
#include <Engine/BlockingVolume.h>
#include <DrawDebugHelpers.h>

APG_MonCharSkillType::APG_MonCharSkillType()
{
	PrimaryActorTick.bCanEverTick = true;

	MonsterType = EMonsterType::eMT_SkillType;
	AIControllerClass = APG_AICon_Mon::StaticClass();

	WeaponComponent = CreateDefaultSubobject<UPG_ComponentWeapon>(TEXT("PROJECTILEFIRECOMPONENT"));
	DashComponent = CreateDefaultSubobject<UPG_ComponentDash>(TEXT("DASHCOMPONENT"));
	PreviewMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PREVIEWMESHCOMPONENT"));
	PreviewMeshComponent->SetupAttachment(GetMesh());
	PreviewMeshComponent->SetVisibility(false);
	DivisionSpawnComponent = CreateDefaultSubobject<UPG_ComponentDivisionSpawn>(TEXT("COMPONENTDIVISIONSPAWN"));
}

void APG_MonCharSkillType::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bEquipWeapon)
		UpdateWeaponSystem();

	if (m_bEquipDash)
		UpdateDashSystem();
}

void APG_MonCharSkillType::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponComponent)
	{
		m_bEquipWeapon = WeaponComponent->IsEquipWeapon();
		if (m_bEquipWeapon)
		{
			// 스테이지 시작시 플레이어가 몬스터 사정권에 있으면 바로 총알이 날아가버리므로,
			// 난이도 밸런스를 위해 시작시 스킬 쿨타임을 바로 적용한다.
			m_bCanWeaponFire = false;
			GetWorldTimerManager().SetTimer(HandleWeaponCoolTime, this, &APG_MonCharSkillType::OnEndWeaponFireCoolTime, MonsterSkillTypeData.MWeaponCoolTime, false, MonsterSkillTypeData.MWeaponCoolTime);
		}
	}

	if (DashComponent)
	{
		m_bEquipDash = (MonsterSkillTypeData.MDashCoolTime > 0 && MonsterSkillTypeData.MDashMoveDistance > 0 && MonsterSkillTypeData.MDashSpeed > 0);
		if (m_bEquipDash)
		{
			DashComponent->OnDashEnd.AddUObject(this, &APG_MonCharSkillType::OnEndDash);
			// 스테이지 시작시 플레이어가 몬스터 사정권에 있으면 바로 몬스터가 대시하므로,
			// 난이도 밸런스를 위해 시작시 스킬 쿨타임을 바로 적용한다.
			m_bCanDash = false;
			GetWorldTimerManager().SetTimer(HandleDashCoolTime, this, &APG_MonCharSkillType::OnEndDashCoolTime, MonsterSkillTypeData.MDashCoolTime, false, MonsterSkillTypeData.MDashCoolTime);
		}
	}
}

void APG_MonCharSkillType::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MonsterSkillTypeData.MRemainDivisionCount = MonsterSkillTypeData.MDivisionCount;
	MonsterData = &MonsterSkillTypeData;
}

void APG_MonCharSkillType::Destroyed()
{
	GetWorldTimerManager().ClearTimer(HandleWeaponCoolTime);
	GetWorldTimerManager().ClearTimer(HandleDashDelayTime);
	GetWorldTimerManager().ClearTimer(HandleDashTime);
	GetWorldTimerManager().ClearTimer(HandleDashCoolTime);

	if (WeaponComponent)
		WeaponComponent->OnDestroy();

	Super::Destroyed();
}

void APG_MonCharSkillType::OnHitCallback(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	auto pBlockingVolume = Cast<ABlockingVolume>(OtherActor);
	auto pMonChar = Cast<APG_MonChar>(OtherActor);
	if (pBlockingVolume || pMonChar)
	{
		m_bOnHitBlocking = true;
		//FString DebugMsg(TEXT("Call"));
		//GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::Yellow, DebugMsg);
	}
}

void APG_MonCharSkillType::UpdateWeaponSystem()
{
	if (nullptr == WeaponComponent)
		return;

	if (m_bCanWeaponFire)
	{
		if (FindTargetToMyCharForWeapon())
		{
			m_bCanWeaponFire = false;
			ABCHECK(0 < MonsterSkillTypeData.MWeaponCoolTime);
						
			FireWeapon();

			GetWorldTimerManager().SetTimer(HandleWeaponCoolTime, this, &APG_MonCharSkillType::OnEndWeaponFireCoolTime, MonsterSkillTypeData.MWeaponCoolTime, false, MonsterSkillTypeData.MWeaponCoolTime);
		}
	}
	else
	{
		if (WeaponComponent->IsEquipWeapon())
			WeaponComponent->StopFire();
	}
}

void APG_MonCharSkillType::UpdateDashSystem()
{
	if (nullptr == DashComponent)
		return;

	if (m_bCanDash)
	{
		FVector vTarget;
		if (FindTargetToMyCharForDash(vTarget))
		{
			m_bCanDash = false;
			Dash(vTarget);
		}
	}

	// 장애물에 부딪히면 대쉬를 멈추게 한다.
	if (DashComponent->IsDash() && m_bOnHitBlocking)
	{
		DashComponent->StopDash();
		//FString DebugMsg(TEXT("Stop Dash"));
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, DebugMsg);
	}
}

void APG_MonCharSkillType::FireWeapon()
{
	ABCHECK(nullptr != WeaponComponent);
	WeaponComponent->Fire();
}

void APG_MonCharSkillType::Dash(FVector vTarget)
{
	ABCHECK(nullptr != DashComponent);

	if (false == DashComponent->IsDash())
	{
		// 몬스터 AI 이동을 정지시키고 대쉬 이동 방향을 vTarget으로 설정한 후
		// MonsterSkillTypeData.MDashReadyTime 만큼 대기하였다가 대쉬한다.
		if (DashComponent->SetupDashData(vTarget, MonsterSkillTypeData.MDashMoveDistance, MonsterSkillTypeData.MDashSpeed))
		{
			m_bDoNotRotation = true;

			StopAI();

			PreviewMeshComponent->SetVisibility(true);
			PreviewMeshComponent->SetRelativeScale3D(FVector(1.0f, GetDashDistaceScale(), 1.0f));
			GetWorldTimerManager().SetTimer(HandleDashDelayTime, this, &APG_MonCharSkillType::OnStartDash, MonsterSkillTypeData.MDashReadyTime, false, MonsterSkillTypeData.MDashReadyTime);
		}
	}
}

float APG_MonCharSkillType::GetDashDistaceScale()
{
	ABCHECK(nullptr != PreviewMeshComponent->GetStaticMesh(), 0.0f);

	// MDashMoveDistance : 1000
	FBoxSphereBounds MeshBounds = PreviewMeshComponent->GetStaticMesh()->GetBounds();

	float fReturn = (MonsterSkillTypeData.MDashMoveDistance / MeshBounds.BoxExtent.X) / 2;
	return fReturn;
}

void APG_MonCharSkillType::SetMonsterDataDivision(FPGMonsterData_SkillType& MonsterData_SkillType)
{
	MonsterSkillTypeData = MonsterData_SkillType;
}

bool APG_MonCharSkillType::IsPossibleToDivision()
{
	if (0 >= MonsterSkillTypeData.MRemainDivisionCount || 0 >= MonsterSkillTypeData.MDivisionCount)
		return false;

	// 분열 대역
	float fPossibleToDivions_HPRatio = (1.0f / (MonsterSkillTypeData.MDivisionCount + 1)) * MonsterSkillTypeData.MRemainDivisionCount;
	return (GetHPRatio() < fPossibleToDivions_HPRatio);
}

int32 APG_MonCharSkillType::GetDivisionCountToHPRatio()
{
	if (MonsterSkillTypeData.MRemainDivisionCount <= 0 || 0.0f >= GetHPRatio())
		return 0;

	float fActiveHPRatio = 1.0f / (MonsterSkillTypeData.MDivisionCount + 1);
	return (int32)(GetHPRatio() / fActiveHPRatio);
}

bool APG_MonCharSkillType::FindTargetToMyCharForWeapon()
{
	auto AMyChar = GetWorld()->GetFirstPlayerController()->GetCharacter();
	if (AMyChar)
	{
		float fDistance = FMath::Abs<float>(FVector::Distance(AMyChar->GetActorLocation(), GetActorLocation()));
		if (fDistance <= WeaponComponent->GetAttackRange())
			return true;
	}

	return false;
}

bool APG_MonCharSkillType::FindTargetToMyCharForDash(FVector& vGetTargetPos)
{
	auto AMyChar = GetWorld()->GetFirstPlayerController()->GetCharacter();
	if (AMyChar)
	{
		float fDistance = FMath::Abs<float>(FVector::Distance(AMyChar->GetActorLocation(), GetActorLocation()));
		if (fDistance <= MonsterSkillTypeData.MDetectRange)
		{
			vGetTargetPos = AMyChar->GetActorLocation();
			return true;
		}
	}

	return false;
}

bool APG_MonCharSkillType::OnDamage(int32 nDamage, bool IsUseLetharAttack)
{
	bool bIsDie = Super::OnDamage(nDamage, IsUseLetharAttack);
	if (false == bIsDie && IsPossibleToDivision())
	{
		if (0 < MonsterSkillTypeData.MRemainDivisionCount)
		{
			FVector vRange(MonsterSkillTypeData.MDivisionSpawnRange, MonsterSkillTypeData.MDivisionSpawnRange, 0.0f);
			if (DivisionSpawnComponent)
				DivisionSpawnComponent->DivisionSpawnActor<APG_MonCharSkillType>(MonsterSkillTypeData.MDivisionAmount, vRange);
		}

		MonsterSkillTypeData.MRemainDivisionCount = GetDivisionCountToHPRatio();
	}

	return bIsDie;
}

void APG_MonCharSkillType::OnEndWeaponFireCoolTime()
{
	m_bCanWeaponFire = true;
}

void APG_MonCharSkillType::OnStartDash()
{
	ABCHECK(nullptr != DashComponent);
	
	m_bOnHitBlocking = false;

	DashComponent->StartDash();
	PreviewMeshComponent->SetVisibility(false);

	// 임시코드
	//GetWorldTimerManager().SetTimer(HandleDashTime, this, &APG_MonCharSkillType::OnEndDash, 1.0f, false, 1.0f);
}

void APG_MonCharSkillType::OnEndDash()
{
	ABCHECK(nullptr != DashComponent);

	if (DashComponent->IsDash())
		DashComponent->StopDash();

	m_bDoNotRotation = false;

	RunAI();

	GetWorldTimerManager().SetTimer(HandleDashCoolTime, this, &APG_MonCharSkillType::OnEndDashCoolTime, MonsterSkillTypeData.MDashCoolTime, false, MonsterSkillTypeData.MDashCoolTime);

	//FString DebugMsg(TEXT("Dash End"));
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, DebugMsg);
}

void APG_MonCharSkillType::OnEndDashCoolTime()
{
	m_bCanDash = true;
}

void APG_MonCharSkillType::OnSpawnActor(APG_MonCharSkillType* ASpawnActor)
{
	ABCHECK(nullptr != ASpawnActor);

	FPGMonsterData_SkillType MakeMonsterData_SkillType = MonsterSkillTypeData;
	MakeMonsterData_SkillType.MRemainDivisionCount = GetDivisionCountToHPRatio();

	ASpawnActor->SetMonsterDataDivision(MakeMonsterData_SkillType);
	ASpawnActor->SetHP(MakeMonsterData_SkillType.MHP);
}