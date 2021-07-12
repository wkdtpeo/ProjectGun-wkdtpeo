// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Character/PG_MonChar.h"

#include "PG_MonCharSkillType.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_GUN_API APG_MonCharSkillType : public APG_MonChar
{
	GENERATED_BODY()

public:
	APG_MonCharSkillType();

	virtual void Tick(float DeltaTime) override;

protected :
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Destroyed() override;

	virtual void OnHitCallback(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;


public :
	//=============================================
	// Weapon
	//=============================================
	void	FireWeapon();

	//=============================================
	// Dash
	//=============================================
	void	Dash(FVector vTarget);
	float	GetDashDistaceScale();

	//=============================================
	// Division
	//=============================================
	void	SetMonsterDataDivision(FPGMonsterData_SkillType& MonsterData_SkillType);
	bool	IsPossibleToDivision();				// 분할 가능한지 검사
	int32	GetDivisionCountToHPRatio();		// 현재 몬스터의 남은 체력 기준, 분할 할 수 있는 횟수를 반환한다.

protected :
	void UpdateWeaponSystem();
	void UpdateDashSystem();

	bool FindTargetToMyCharForWeapon();
	bool FindTargetToMyCharForDash(OUT FVector& vGetTargetPos);

	virtual bool OnDamage(int32 nDamage, bool IsUseLetharAttack = false) override;
	void OnEndWeaponFireCoolTime();	// 무기 발사 쿨타임 종료
	void OnStartDash();				// 대쉬 시작
	UFUNCTION()
	void OnEndDash();				// 대쉬 종료
	void OnEndDashCoolTime();		// 대쉬 쿨타임 종료

public :
	// 스폰되면 생성된 몬스터 액터가 호출된다.
	void OnSpawnActor(APG_MonCharSkillType* ASpawnActor);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PG MonsterInfo", Meta = (AllowPrivateAccess = true))
	struct FPGMonsterData_SkillType MonsterSkillTypeData;

	//=============================================
	// Weapon
	//=============================================
	UPROPERTY(VisibleAnywhere, Category = "PG Weapon MonsterComponent", Meta = (AllowPrivateAccess = true))
	class UPG_ComponentWeapon* WeaponComponent;
	bool			m_bEquipWeapon = false;			// 무기 사용 가능여부
	bool			m_bCanWeaponFire = false;		// 무기 발사 사용여부
	FTimerHandle	HandleWeaponCoolTime;			// 무기 발사 쿨타임

	//=============================================
	// Dash
	//=============================================
	UPROPERTY(VisibleAnywhere, Category = "PG Dash MonsterComponent", Meta = (AllowPrivateAccess = true))
	class UPG_ComponentDash* DashComponent;
	UPROPERTY(EditAnywhere, Category = "PG Dash MonsterComponent", Meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* PreviewMeshComponent;
	bool			m_bOnHitBlocking = false;		// 대쉬 중 장애물에 부딪힘
	bool			m_bEquipDash = false;			// 대쉬 사용 가능여부
	bool			m_bCanDash = false;				// 대쉬 활성화 여부
	FTimerHandle	HandleDashDelayTime;			// 대쉬 딜레이 타임
	FTimerHandle	HandleDashTime;					// 대쉬 시간
	FTimerHandle	HandleDashCoolTime;				// 대쉬 쿨타임


	//=============================================
	// Division
	//=============================================
	UPROPERTY(VisibleAnywhere, Category = "PG Division MonsterComponent", Meta = (AllowPrivateAccess = true))
	class UPG_ComponentDivisionSpawn* DivisionSpawnComponent;
};
