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

	bool IsReadyLethal();		// �ʻ�⸦ ����� �� �ִ� �����ΰ�?

	UFUNCTION(BlueprintImplementableEvent)
	void OnInitLethal();
	UFUNCTION(BlueprintImplementableEvent)
	void OnReadyLethal();
	
protected :
	// ��ü �ݸ��� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PG LethalAttackInfo")
	class USphereComponent* CollisionComponent;

	// �ʻ�⸦ ����� �� �������� ����Ʈ.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PG LethalAttackInfo")
	class UParticleSystemComponent* LShowingEffect;

	// �ʻ�Ⱑ ����ɶ� ����Ǵ� ����Ʈ. ���� ������ �÷��̾� �߽�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PG LethalAttackInfo")
	class UParticleSystemComponent*	LEffectVisual;

	// �ʻ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PG LethalAttackInfo")
	class UAudioComponent* LethalSoundComponent;

private :
	// �ʻ�� �������� �������� �޴� ���͸� ã�´�.
	bool FindingDamagedMonsters(OUT TArray<class APG_MonChar*>& GetMonsters);
	float GetAttackRange();
	float GetRatioAttackRange();

private :
	//UFUNCTION(BlueprintImplementableEvent)
	void OnReadyToUseTheLethalAttack();		// �ʻ�� ��� �غ�
	void OnUseTheLethalAttackFail();		// �ʻ�� ��� ����
	void OnTryUseTheLethalAttack();			// �ʻ�� ��� �õ�
	void OnAddLethalPoint(class APG_MonChar* AMonster);		// �ʻ�� ����Ʈ ȹ��

	UFUNCTION()
	void OnEndEffectVisual(UParticleSystemComponent* ParticleSystemComponent);		// �ʻ�� ����� ����Ǵ� ����Ʈ ����� ȣ��

	UFUNCTION()
	void OnBegineOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);	

	UFUNCTION()
	void OnParticleCollide(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity, FVector Direction, FVector Normal, FName BoneName, UPhysicalMaterial* PhysMat);

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private :
	// ��������Ʈ ���ſ� Handle ����
	FDelegateHandle HandleOnBasicAttackMonsterKill;
	FDelegateHandle HandleOnReadyToUseTheLethalAttack;
	FDelegateHandle HandleOnUseTheLethalAttackSuccess;
	FDelegateHandle HandleOnUseTheLethalAttackFail;

private :
	// �ʻ�� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG LethalAttackInfo", Meta = (AllowPrivateAccess = true))
	struct FPGLethalAttackData LethalAttackData;

	// �ʻ�� ������ġ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG LethalAttackInfo", Meta = (AllowPrivateAccess = true))
	int32 LethalPoint;

	// �ʻ��� ���� ������ ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG LethalAttackInfo", Meta = (AllowPrivateAccess = true))
	int32 MaxLethalKill;

	bool m_bReadyLethal = false;
};
