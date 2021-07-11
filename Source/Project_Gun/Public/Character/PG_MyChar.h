// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <GameFramework/Character.h>
#include "Character/PG_Char.h"
#include "PG_MyChar.generated.h"

UCLASS()
class PROJECT_GUN_API APG_MyChar : public APG_Char
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APG_MyChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void FellOutOfWorld(const class UDamageType & dmgType) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpdateRotation(float DeltaTime);
	void UpdateCamera(float DeltaTime);
	void UpdateMovement(float DeltaTime);

public :
	float GetMaxSpeed();

	UFUNCTION(BlueprintCallable, Category = "PG TargetMonster")
	bool IsTargetMonster();

	//==========================================================
	// Weapon
	//==========================================================
	bool EquipWeapon(int32 nWeaponTalbeIndex);		// 장비 장착
	void Fire() override;							// 무기 발사
	float GetAttackRange();							// 공격 범위

	//==========================================================
	// LethalAttack
	//==========================================================
	bool EquipLethalAttack(int32 LethalAttackID);	// 스킬 장착
	bool UnEquipLethalAttack();


public :
	UFUNCTION()
	virtual void OnGenerateOverlabEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult);

	void OnAttackedByMonster();						// 몬스터에게 피격당함
	void OnPlayerWorldOut();						// 맵에서 떨어짐. (Kill Z)
	void OnBasicAttackMonsterKill(class APG_MonChar* AMon);	// 몬스터 사망
	void OnChangeWeapon(int32 nWeaponTableIndex);

private :
	//==========================================================
	// Camera
	//==========================================================
	enum class ECameraView : int
	{
		eCV_QuaterView = 0,
		eCV_Fly,
	};

	void SetCameraView(ECameraView eCameraView);

	//==========================================================
	// Character Movement
	//==========================================================
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void TouchReleased();

	void UpdateMovementData();

	//==========================================================
	// Weapon
	//==========================================================
	UFUNCTION()
	bool SetWeapon(APG_Weapon* NewWeapon);
#ifdef TEST_CODE
	void WeaponEquipRlife();
	void WeaponEquipFlame();
	void WeaponEquipShutgun();
	void WeaponEquipLaserBeam();
	void WeaponEquipGrandleLauncher();
#endif // TEST_CODE

	//==========================================================
	// LethalAttack
	//==========================================================
	UFUNCTION()
	bool SetLethalAttack(class APG_LethalAttack* NewLethalAttack);

	class APG_MonChar* FindTargetToMonster();

public :
	UPROPERTY(VisibleAnywhere, Category = "PG CameraInfo")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "PG CameraInfo")
	UCameraComponent* Camera;


private :
	//==========================================================
	// Animation (애니메이션)
	//==========================================================
	UPROPERTY()
	class UPG_AnimIns_MyChar* ABAnimInstance;

	//==========================================================
	// CharacterCameraTable (캐릭터 카메라 정보)
	//==========================================================
	float		ArmLengthTo = 0.0f;
	FRotator	ArmRotationTo = FRotator::ZeroRotator;
	float		ArmLengthSpeed = 0.0f;
	float		ArmRotationSpeed = 0.0f;
	ECameraView m_eCurrentCameraView;

	UPROPERTY()
	class APG_PlayerController*	m_pPlayerContorller;
	UPROPERTY(Transient, EditAnywhere, Category = "PG CharacterCameraTableInfo")
	float		Pitch;
	UPROPERTY(Transient, EditAnywhere, Category = "PG CharacterCameraTableInfo")
	float		Yaw;
	UPROPERTY(Transient, EditAnywhere, Category = "PG CharacterCameraTableInfo")
	float		Roll;
	UPROPERTY(Transient, EditAnywhere, Category = "PG CharacterCameraTableInfo")
	float		ArmLength;
	UPROPERTY(Transient, EditAnywhere, Category = "PG CharacterCameraTableInfo")
	float		Fov;
	UPROPERTY(Transient, EditAnywhere, Category = "PG CharacterCameraTableInfo")
	float		ScreenY;

	//==========================================================
	// CharacterMovementTable (캐릭터 이동관련 정보)
	//==========================================================
	FVector		DirectionToMove = FVector::ZeroVector;

	UPROPERTY(Transient, EditAnywhere, Category = "PG CharacterMovementTableInfo")
	float		MaxSpeed;
	UPROPERTY(Transient, EditAnywhere, Category = "PG CharacterMovementTableInfo")
	float		AccelerationSpeed;
	UPROPERTY(Transient, EditAnywhere, Category = "PG CharacterMovementTableInfo")
	float		BodyRotateSpeed;
	UPROPERTY(Transient, EditAnywhere, Category = "PG CharacterMovementTableInfo")
	float		AttackBodyRotateSpeed;
	UPROPERTY(Transient, EditAnywhere, Category = "PG CharacterMovementTableInfo")
	float		AttackUpperBodyRotateSpeed;

	float		MonsterTargetAngle = 0.0f;
	FRotator	TargetRotation = FRotator::ZeroRotator;

	//==========================================================
	// Weapon (무기)
	//==========================================================
	UPROPERTY(VisibleAnywhere, Category = "PG WeaponInfo")
	TWeakObjectPtr<class APG_Weapon> CurrentWeapon = nullptr;

	//==========================================================
	// LethalAttack (필살기)
	//==========================================================
	UPROPERTY(VisibleAnywhere, Category = "PG LethalAttackInfo")
	TWeakObjectPtr<class APG_LethalAttack> CurrentLethalAttack = nullptr;
};
