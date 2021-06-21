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
	bool	IsPossibleToDivision();				// ���� �������� �˻�
	int32	GetDivisionCountToHPRatio();		// ���� ������ ���� ü�� ����, ���� �� �� �ִ� Ƚ���� ��ȯ�Ѵ�.

protected :
	void UpdateWeaponSystem();
	void UpdateDashSystem();

	bool FindTargetToMyCharForWeapon();
	bool FindTargetToMyCharForDash(OUT FVector& vGetTargetPos);

	virtual bool OnDamage(int32 nDamage, bool IsUseLetharAttack = false) override;
	void OnEndWeaponFireCoolTime();	// ���� �߻� ��Ÿ�� ����
	void OnStartDash();				// �뽬 ����
	UFUNCTION()
	void OnEndDash();				// �뽬 ����
	void OnEndDashCoolTime();		// �뽬 ��Ÿ�� ����

public :
	// �����Ǹ� ������ ���� ���Ͱ� ȣ��ȴ�.
	void OnSpawnActor(APG_MonCharSkillType* ASpawnActor);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PG MonsterInfo", Meta = (AllowPrivateAccess = true))
	struct FPGMonsterData_SkillType MonsterSkillTypeData;

	//=============================================
	// Weapon
	//=============================================
	UPROPERTY(VisibleAnywhere, Category = "PG Weapon MonsterComponent", Meta = (AllowPrivateAccess = true))
	class UPG_ComponentWeapon* WeaponComponent;
	bool			m_bEquipWeapon = false;			// ���� ��� ���ɿ���
	bool			m_bCanWeaponFire = false;		// ���� �߻� ��뿩��
	FTimerHandle	HandleWeaponCoolTime;			// ���� �߻� ��Ÿ��

	//=============================================
	// Dash
	//=============================================
	UPROPERTY(VisibleAnywhere, Category = "PG Dash MonsterComponent", Meta = (AllowPrivateAccess = true))
	class UPG_ComponentDash* DashComponent;
	UPROPERTY(EditAnywhere, Category = "PG Dash MonsterComponent", Meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* PreviewMeshComponent;
	bool			m_bOnHitBlocking = false;		// �뽬 �� ��ֹ��� �ε���
	bool			m_bEquipDash = false;			// �뽬 ��� ���ɿ���
	bool			m_bCanDash = false;				// �뽬 Ȱ��ȭ ����
	FTimerHandle	HandleDashDelayTime;			// �뽬 ������ Ÿ��
	FTimerHandle	HandleDashTime;					// �뽬 �ð�
	FTimerHandle	HandleDashCoolTime;				// �뽬 ��Ÿ��


	//=============================================
	// Division
	//=============================================
	UPROPERTY(VisibleAnywhere, Category = "PG Division MonsterComponent", Meta = (AllowPrivateAccess = true))
	class UPG_ComponentDivisionSpawn* DivisionSpawnComponent;
};
