// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Define/PG_TableData.h"
#include <GameFramework/Actor.h>
#include "PG_LethalAttack.generated.h"

UCLASS()
class PROJECT_GUN_API APG_LethalAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APG_LethalAttack();

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BindPlayerState(class APG_MyPlayerState* APlayerState);
	void UnBindPlayerState(class APG_MyPlayerState* APlayerState);

	void SetLethaAttackData(const FPGLethalAttackData* pLethalAttackData);

	bool IsReadyLethal();		// 필살기를 사용할 수 있는 상태인가?

	UFUNCTION(BlueprintImplementableEvent)
	void OnInitLethal();
	UFUNCTION(BlueprintImplementableEvent)
	void OnReadyLethal();
	
protected :
	// 구체 콜리전 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PG LethalAttackInfo")
	class USphereComponent* CollisionComponent;

	// 필살기를 사용할 때 보여지는 이펙트.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PG LethalAttackInfo")
	class UParticleSystemComponent* LShowingEffect;

	// 필살기가 실행될때 재생되는 이펙트. 시전 지점은 플레이어 중심.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PG LethalAttackInfo")
	class UParticleSystemComponent*	LEffectVisual;

	// 필살기 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PG LethalAttackInfo")
	class UAudioComponent* LethalSoundComponent;

private :
	// 필살기 사용범위내 데미지를 받는 몬스터를 찾는다.
	bool FindingDamagedMonsters(OUT TArray<class APG_MonChar*>& GetMonsters);
	float GetAttackRange();
	float GetRatioAttackRange();

private :
	//UFUNCTION(BlueprintImplementableEvent)
	void OnReadyToUseTheLethalAttack();		// 필살기 사용 준비
	void OnUseTheLethalAttackFail();		// 필살기 사용 실패
	void OnTryUseTheLethalAttack();			// 필살기 사용 시도
	void OnAddLethalPoint(class APG_MonChar* AMonster);		// 필살기 포인트 획득

	UFUNCTION()
	void OnEndEffectVisual(UParticleSystemComponent* ParticleSystemComponent);		// 필살기 실행시 재생되는 이펙트 종료시 호출

	UFUNCTION()
	void OnBegineOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);	

	UFUNCTION()
	void OnParticleCollide(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity, FVector Direction, FVector Normal, FName BoneName, UPhysicalMaterial* PhysMat);

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private :
	// 델리게이트 제거용 Handle 보관
	FDelegateHandle HandleOnBasicAttackMonsterKill;
	FDelegateHandle HandleOnReadyToUseTheLethalAttack;
	FDelegateHandle HandleOnUseTheLethalAttackSuccess;
	FDelegateHandle HandleOnUseTheLethalAttackFail;

private :
	// 필살기 설정값
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG LethalAttackInfo", Meta = (AllowPrivateAccess = true))
	struct FPGLethalAttackData LethalAttackData;

	// 필살기 누적수치
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG LethalAttackInfo", Meta = (AllowPrivateAccess = true))
	int32 LethalPoint;

	// 필살기로 죽인 몬스터의 수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG LethalAttackInfo", Meta = (AllowPrivateAccess = true))
	int32 MaxLethalKill;

	bool m_bReadyLethal = false;
};
