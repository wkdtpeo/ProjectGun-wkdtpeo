// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "Define/PG_TableData.h"
#include <GameFramework/Actor.h>
#include "PG_Weapon.generated.h"

UCLASS()
class PROJECT_GUN_API APG_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APG_Weapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Destroyed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public :
	bool SetWeaponData(int32 nWeaponTableIndex);

	virtual int32				GetAttackRange();
	class UAnimMontage*			GetMontageFireReaction();
	const struct FPGWeaponData* GetWeaponData();
	int32						GetWeaponAttackDamage();	// ���� ĳ���� ���� ���� ���¿� ���� ���� �������� ��ȯ�Ѵ�.

	bool IsOwnerMonster();	// ���� ��������
	bool IsOwnerMyPlayer();	// �÷��̾� ��������

	// �߻� ó��
	UFUNCTION()
	virtual void Fire();

	UFUNCTION()
	virtual void StopFire();

protected :
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG WeaponInfo", Meta = (AllowPrivateAccess = true))
	int32 WeaponTableIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG WeaponInfo", Meta = (AllowPrivateAccess = true))
	struct FPGWeaponData WeaponData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG WeaponInfo", Meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* Weapon;

	// �߻�ü�� ������ �� �ش� ��ġ�� �����Ǵ� ����Ʈ(�߻� �ѱ� ����Ʈ).
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG WeaponInfo", Meta = (AllowPrivateAccess = true))
	class UParticleSystemComponent*	FiringEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG WeaponInfo", Meta = (AllowPrivateAccess = true))
	class UAudioComponent* FireSound;

	// ���� �߻�� ���Ǵ� �ִϸ��̼�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PG WeaponInfo", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* MontageFireReaction;
};
