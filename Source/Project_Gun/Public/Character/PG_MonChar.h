// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <GameFramework/Character.h>
#include "Character/PG_Char.h"
#include "Define/PG_TableData.h"
#include "PG_MonChar.generated.h"

UCLASS()
class PROJECT_GUN_API APG_MonChar : public APG_Char
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APG_MonChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void FellOutOfWorld(const class UDamageType & dmgType) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpdateRotation(float DeltaTime);

public :
	bool RunAI();
	void StopAI();

	void SetHP(int32 nHP);
	void SetDie(bool IsUseLetharAttack);

	EMonsterType GetMonsterType();
	const FPGMonsterData* GetMonsterData();
	int32 GetDetectRange() const;		// 몬스터 시야
	int32 GetChaseRange() const;		// 몬스터가 쫓아오는 최대 거리
	float GetHPRatio() const;			// 현재 체력 비율
	float GetMaxHP() const;				// 최대 체력
	float GetDropRate() const;			// 재화 드랍확율

	class APG_MyChar* FindTargetToMyChar();

public :
	virtual bool OnDamage(int32 nDamage, bool IsUseLetharAttack = false);

protected :
	virtual bool SetDefaultMonsterData();

	virtual void OnAssetLoadCharacterCompleted() override;

	UFUNCTION()
	virtual void OnCharacterHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnHitCallback(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
protected:
	UPROPERTY(VisibleInstanceOnly, Category = MonsterInfo, Meta = (AllowPrivateAccess = true))
	EMonsterType MonsterType = EMonsterType::eMT_Max;

	UPROPERTY(VisibleInstanceOnly, Category = MonsterComponent)
	class UPG_ComponentCharScale* CharScaleComponent;

	UPROPERTY()
	class APG_AICon_Mon* AIController;

	struct FPGMonsterData* MonsterData = nullptr;

protected:
	bool		m_bDoNotRotation = false;		// 몬스터가 회전하지 않음
	bool		m_bDoNotMoving = false;			// 몬스터가 이동하지 않음
	float		m_fCharTargetAngle = 0.0f;
	float		m_fMaxHP;
	FRotator	m_vTargetRotation = FRotator::ZeroRotator;
	FVector		m_vBeginScale;
	
private :
	void DebugOnly();
};
