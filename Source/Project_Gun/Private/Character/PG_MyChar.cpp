// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PG_MyChar.h"
#include "Player/PlayerController/PG_PlayerController.h"
#include "Player/PlayerState/PG_MyPlayerState.h"
#include "Game/GameState/PG_GameStateStage.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Anim/PG_AnimIns_MyChar.h"
#include "BlueScriptObject/Projectile/PG_Projectile.h"
#include "Character/PG_MonChar.h"
#include "BlueScriptObject/Weapon/PG_Weapon.h"
#include "BlueScriptObject/Weapon/PG_WeaponRifle.h"
#include "BlueScriptObject/Weapon/PG_WeaponGrenadeLauncher.h"
#include "BlueScriptObject/Weapon/PG_WeaponLaserBemGun.h"
#include "BlueScriptObject/Weapon/PG_WeaponFlameGun.h"
#include "BlueScriptObject/LethalAttack/PG_LethalAttack.h"
#include "Game/GameMode/PG_GameModeStage.h"
#include "SaveData/PG_SavePlayerData.h"
#include <Engine/SkeletalMeshSocket.h>
#include <DrawDebugHelpers.h>


// Sets default values
APG_MyChar::APG_MyChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// CharacterMovement
	MaxSpeed = 0.0f;
	AccelerationSpeed = 0.0f;
	BodyRotateSpeed = 0.0f;
	MonsterTargetAngle = 0.0f;
	TargetRotation = FRotator::ZeroRotator;

	// 카메라 설정
	{
		SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
		Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

		SpringArm->SetupAttachment(GetCapsuleComponent());
		Camera->SetupAttachment(SpringArm);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
		SpringArm->TargetArmLength = 400.0f;
		SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

		// 화면비율 조정
		Camera->bConstrainAspectRatio = false;	// false : 어느 디바이스에서든 동일한 비율로 출력하게 한다.
	}

	// 기본 캐릭터 설정
	//{
		// 스켈레탈
		//static ConstructorHelpers::FObjectFinder<USkeletalMesh>	SK_DEFAULT_CHARACTER(TEXT("SkeletalMesh'/Game/PG/Meshs/Player/GunCH_Skel.GunCH_Skel'"));
		//if (SK_DEFAULT_CHARACTER.Succeeded())
		//{
		//	GetMesh()->SetSkeletalMesh(SK_DEFAULT_CHARACTER.Object);
		//}

		//// 애니메이션 블루프린트
		//GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		//static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("AnimBlueprint'/Game/PG/Blueprints/Anim/BP_MyCharacter_Work.BP_MyCharacter_Work_C'"));
		//if (WARRIOR_ANIM.Succeeded())
		//{
		//	GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
		//}
	//}
	
	{
		// 콜리전 설정
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyCharacter"));
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APG_MyChar::OnGenerateOverlabEvent);
	}
}

// Called when the game starts or when spawned
void APG_MyChar::BeginPlay()
{
	Super::BeginPlay();

	m_pPlayerContorller = Cast<APG_PlayerController>(GetController());

	{
		// 기본 무기 장착
		auto pGameModeStage = Cast<APG_GameModeStage>(GetWorld()->GetAuthGameMode());
		ABCHECK(nullptr != pGameModeStage);

		auto pSavePlayerData = pGameModeStage->GetSavePlayerData();
		EquipWeapon(pSavePlayerData->m_kEquipWeaponTableIndex[0]);
	}

	{
		auto AMyPlayerState = Cast<APG_MyPlayerState>(GetPlayerState());
		ABCHECK(nullptr != AMyPlayerState);
		AMyPlayerState->OnBasicAttackMonsterKill.AddUObject(this, &APG_MyChar::OnBasicAttackMonsterKill);
		AMyPlayerState->OnChangeWeapon.AddUObject(this, &APG_MyChar::OnChangeWeapon);
	}
}

void APG_MyChar::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UPG_GameInstance* pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	if (pGameInstance)
	{
		MaxSpeed = pGameInstance->GetTableCharacterMovementData()->MaxSpeed;
		AccelerationSpeed = pGameInstance->GetTableCharacterMovementData()->AccelerationSpeed;
		BodyRotateSpeed = pGameInstance->GetTableCharacterMovementData()->BodyRotateSpeed;
		AttackBodyRotateSpeed = pGameInstance->GetTableCharacterMovementData()->AttackBodyRotateSpeed;
		AttackUpperBodyRotateSpeed = pGameInstance->GetTableCharacterMovementData()->AttackUpperBodyRotateSpeed;

		Pitch = pGameInstance->GetTableCharacterCameraData()->Pitch;
		Yaw = pGameInstance->GetTableCharacterCameraData()->Yaw;
		Roll = pGameInstance->GetTableCharacterCameraData()->Roll;
		ArmLength = pGameInstance->GetTableCharacterCameraData()->ArmLength;
		Fov = pGameInstance->GetTableCharacterCameraData()->Fov;
		ScreenY = pGameInstance->GetTableCharacterCameraData()->ScreenY;

		// 스크린 화면 기준 카메라 Y축 이동
		FVector vWorldBack(0.f, 0.f, ScreenY);
		FVector vCamVector = Camera->GetRelativeTransform().GetLocation() + vWorldBack;
		Camera->SetRelativeLocation(vCamVector);

		UpdateMovementData();
	}

	ABAnimInstance = Cast<UPG_AnimIns_MyChar>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != ABAnimInstance);
	ABAnimInstance->OnFire.AddUObject(this, &APG_MyChar::Fire);

	SetCameraView(ECameraView::eCV_QuaterView);
}

void APG_MyChar::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	// 에디터 작업에서 선택한 액터의 속성이나 트랜스폼 정보가 변경될 때 호출된다.
}

void APG_MyChar::FellOutOfWorld(const UDamageType & dmgType)
{
	if (dmgType.bCausedByWorld)
		OnPlayerWorldOut();
	else
		Super::FellOutOfWorld(dmgType);
}

// Called every frame
void APG_MyChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

//#ifdef TEST_CODE
	// 임시코드 : 개발 테스트 코드 (배포시 삭제)
	//{
	//	UpdateMovementData();
	//	SetCameraView(ECameraView::eCV_QuaterView);
	//}
//#endif
	
	UpdateMovement(DeltaTime);
	UpdateRotation(DeltaTime);
	UpdateCamera(DeltaTime);
}

// Called to bind functionality to input
void APG_MyChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &APG_MyChar::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &APG_MyChar::LeftRight);

	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APG_MyChar::Fire);

#ifdef TEST_CODE
	// Weapon Test Code
	PlayerInputComponent->BindAction("Weapon_Rifle", IE_Released, this, &APG_MyChar::WeaponEquipRlife);
	PlayerInputComponent->BindAction("Weapon_Flame", IE_Released, this, &APG_MyChar::WeaponEquipFlame);
	PlayerInputComponent->BindAction("Weapon_Shutgun", IE_Released, this, &APG_MyChar::WeaponEquipShutgun);
	PlayerInputComponent->BindAction("Weapon_LaserBeam", IE_Released, this, &APG_MyChar::WeaponEquipLaserBeam);
	PlayerInputComponent->BindAction("Weapon_GrandleLauncher", IE_Released, this, &APG_MyChar::WeaponEquipGrandleLauncher);
#endif // TEST_CODE
}

void APG_MyChar::UpdateRotation(float DeltaTime)
{
	APG_MonChar* pPG_MonChar = FindTargetToMonster();
	if (pPG_MonChar)
	{
		// 하체(Root) 회전 ==========================================================
		FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), pPG_MonChar->GetActorLocation());
		FRotator rotator = FMath::RInterpTo(TargetRotation, PlayerRot, DeltaTime, AttackBodyRotateSpeed);
		rotator.Pitch = 0.0f;
		TargetRotation = rotator;
		SetActorRotation(rotator);

		// 상체(Aim Offset) 회전 ==========================================================
		FVector vLookAt = GetActorForwardVector();
		vLookAt.Z = 0.0f;

		FVector vTarget = pPG_MonChar->GetActorLocation() - GetActorLocation();
		vTarget.Z = 0.0f;
		vTarget.Normalize();

		// 내적 (캐릭터와 몬스터간의 각도)
		float NewTargetAngle = (FMath::Acos(FVector::DotProduct(vLookAt, vTarget))) * 57.29578F;
		
		// 외적 (적이 왼쪽 또는 오른쪽에 있는지 확인)
		FVector Cross = FVector::CrossProduct(vLookAt, vTarget);
		if (Cross.Z < 0)
			NewTargetAngle *= -1;

		// 몬스터 조준 및 해제
		if (-45.0f <= NewTargetAngle && NewTargetAngle <= 45.0f)	
			MonsterTargetAngle = FMath::FInterpTo(MonsterTargetAngle, NewTargetAngle, DeltaTime, AttackUpperBodyRotateSpeed);
		else
			MonsterTargetAngle = FMath::FInterpTo(MonsterTargetAngle, 0.0f, DeltaTime, AttackUpperBodyRotateSpeed);

		ABAnimInstance->SetRotation(MonsterTargetAngle);


//#ifdef  ENABLE_DRAW_DEBUG
//		//FString DebugMsg = FString::Printf(TEXT("Yaw : %f / %f"), NewTargetAngle, MonsterTargetAngle);
//		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, DebugMsg);
//		DrawDebugPoint(GetWorld(), GetActorLocation() + (vLookAt * 200), 10.0f, FColor::Red, false, 0.02f);
//		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (vLookAt * 200), FColor::Red, false, 0.02f);
//#endif
	}
	else
	{
		float angle = FMath::FInterpTo(MonsterTargetAngle, 0.0f, DeltaTime, AttackUpperBodyRotateSpeed);
		MonsterTargetAngle = angle;
		ABAnimInstance->SetRotation(angle);

		TargetRotation = GetActorRotation();
	}

	if (ABAnimInstance)
		ABAnimInstance->OnTargetMonster(pPG_MonChar);

	if (CurrentWeapon.IsValid(true) && nullptr == pPG_MonChar)
		CurrentWeapon->StopFire();
}

void APG_MyChar::UpdateCamera(float DeltaTime)
{
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLength, DeltaTime, ArmLengthSpeed);

	switch (m_eCurrentCameraView)
	{
	case ECameraView::eCV_QuaterView:
		SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed));
		break;
	}
}

void APG_MyChar::UpdateMovement(float DeltaTime)
{
	switch (m_eCurrentCameraView)
	{
	case ECameraView::eCV_QuaterView:
		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			AddMovementInput(DirectionToMove);
		}

		break;
	}
}

void APG_MyChar::OnGenerateOverlabEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult)
{
	auto pMonChar = Cast<APG_MonChar>(OtherActor);
	if (pMonChar)
	{
		OnAttackedByMonster();
		return;
	}
}

void APG_MyChar::OnAttackedByMonster()
{
	auto AGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	if (AGameStateStage)
		AGameStateStage->OnCheckStageFailed();
}

void APG_MyChar::OnPlayerWorldOut()
{
	if (m_pPlayerContorller)
	{
		m_pPlayerContorller->RestartLevel();
		ABLOG_S(Warning);
	}
}

void APG_MyChar::OnBasicAttackMonsterKill(APG_MonChar* AMon)
{
	if (false == CurrentLethalAttack.IsValid())
	{
		EquipLethalAttack(0);

		auto MyPlayerState = Cast<APG_MyPlayerState>(GetPlayerState());
		if (MyPlayerState)
			MyPlayerState->ReadyToUseTheSkill();
	}
}

void APG_MyChar::OnChangeWeapon(int32 nWeaponTableIndex)
{
	if (EquipWeapon(nWeaponTableIndex))
	{
		//auto MyPlayerState = Cast<APG_MyPlayerState>(GetPlayerState());
		//if (MyPlayerState)
		//	MyPlayerState->
	}
	//ABCHECK(EquipWeapon(EquipWeaponClass));
}

void APG_MyChar::SetCameraView(ECameraView eCameraView)
{
	switch (eCameraView)
	{
	case ECameraView::eCV_QuaterView:
	{
		ArmLengthTo = ArmLength;
		ArmRotationTo = FRotator(Pitch, Yaw, Roll);
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, BodyRotateSpeed, 0.0f);

		Camera->SetFieldOfView(Fov);

		break;
	}
	case ECameraView::eCV_Fly:
	{
		break;
	}
	default:
	{
		break;
	}
	}
}

void APG_MyChar::UpDown(float NewAxisValue)
{
	DirectionToMove.Y = -NewAxisValue;
}

void APG_MyChar::LeftRight(float NewAxisValue)
{
	DirectionToMove.X = NewAxisValue;
}

void APG_MyChar::TouchReleased()
{
	if (CurrentLethalAttack.IsValid())
	{
		auto MyPlayerState = Cast<APG_MyPlayerState>(GetPlayerState());
		if (MyPlayerState)
			MyPlayerState->TryUseTheSkill();
	}
}

void APG_MyChar::UpdateMovementData()
{
	ABCHECK(nullptr != GetCharacterMovement());

	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
	GetCharacterMovement()->MaxAcceleration = AccelerationSpeed;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, BodyRotateSpeed, 0.0f);
}

float APG_MyChar::GetMaxSpeed()
{
	return GetCharacterMovement() ? GetCharacterMovement()->MaxWalkSpeed : 0.0f;
}

float APG_MyChar::GetAttackRange()
{
	return CurrentWeapon.IsValid(true) ? CurrentWeapon->GetAttackRange() : 0.0f;
}

void APG_MyChar::Fire()
{
	if (false == ABAnimInstance->IsAimingComplete())
		return;

	if (CurrentWeapon.IsValid(true))
		CurrentWeapon->Fire();

	m_pPlayerContorller->OnFire();
}

bool APG_MyChar::EquipLethalAttack(int32 LethalAttackID)
{
	auto GameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != GameInstance, false);

	const FPGLethalAttackData* FLethalAttackData = GameInstance->GetTableLethalAttackData(LethalAttackID);
	ABCHECK(nullptr != FLethalAttackData, false);

	APG_LethalAttack* NewLethalAttack = GetWorld()->SpawnActor<APG_LethalAttack>(FLethalAttackData->BLethalAttack, FVector::ZeroVector, FRotator::ZeroRotator);
	ABCHECK(nullptr != NewLethalAttack, false);

	NewLethalAttack->SetLethaAttackData(FLethalAttackData);

	if (false == SetLethalAttack(NewLethalAttack))
	{
		CurrentLethalAttack->UnBindPlayerState(Cast<APG_MyPlayerState>(GetPlayerState()));
		CurrentLethalAttack->Destroy();
		return false;
	}

	return true;
}

bool APG_MyChar::UnEquipLethalAttack()
{
	if(CurrentLethalAttack.IsValid())
	{
		CurrentLethalAttack->UnBindPlayerState(Cast<APG_MyPlayerState>(GetPlayerState()));
		CurrentLethalAttack = nullptr;
	}

	return true;
}

bool APG_MyChar::SetLethalAttack(class APG_LethalAttack* NewLethalAttack)
{
	if (CurrentLethalAttack.IsValid())
	{
		CurrentLethalAttack->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentLethalAttack->UnBindPlayerState(Cast<APG_MyPlayerState>(GetPlayerState()));
		CurrentLethalAttack->Destroy();
		CurrentLethalAttack = nullptr;
	}

	if (NewLethalAttack)
	{
		NewLethalAttack->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		NewLethalAttack->SetOwner(this);
		NewLethalAttack->BindPlayerState(Cast<APG_MyPlayerState>(GetPlayerState()));
		CurrentLethalAttack = NewLethalAttack;

		ABLOG(Warning, TEXT("Equip Skill : %s"), *NewLethalAttack->GetName());
	}

	return true;
}

#ifdef TEST_CODE
void APG_MyChar::WeaponEquipRlife()
{
	auto Weapon = LoadClass<APG_Weapon>(NULL, TEXT("Blueprint'/Game/PG/Blueprints/Weapon/BP_Weapon_Rifle.BP_Weapon_Rifle_C'"));
	ABCHECK(EquipWeapon(0));
}

void APG_MyChar::WeaponEquipFlame()
{
	auto Weapon = LoadClass<APG_Weapon>(NULL, TEXT("Blueprint'/Game/PG/Blueprints/Weapon/BP_Weapon_FlameGun.BP_Weapon_FlameGun_C'"));
	ABCHECK(EquipWeapon(1));
}

void APG_MyChar::WeaponEquipShutgun()
{
	auto Weapon = LoadClass<APG_Weapon>(NULL, TEXT("Blueprint'/Game/PG/Blueprints/Weapon/BP_Weapon_ShotGun.BP_Weapon_ShotGun_C'"));
	ABCHECK(EquipWeapon(2));
}

void APG_MyChar::WeaponEquipLaserBeam()
{
	auto Weapon = LoadClass<APG_Weapon>(NULL, TEXT("Blueprint'/Game/PG/Blueprints/Weapon/BP_Weapon_LaserBemGun.BP_Weapon_LaserBemGun_C'"));
	ABCHECK(EquipWeapon(3));
}

void APG_MyChar::WeaponEquipGrandleLauncher()
{
	auto Weapon = LoadClass<APG_Weapon>(NULL, TEXT("Blueprint'/Game/PG/Blueprints/Weapon/BP_Weapon_GraenadeLauncher.BP_Weapon_GraenadeLauncher_C'"));
	ABCHECK(EquipWeapon(4));
}
#endif // TEST_CODE

bool APG_MyChar::EquipWeapon(int32 nWeaponTalbeIndex)
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	auto pWeaponTableData = pGameInstance->GetWeaponTableData(nWeaponTalbeIndex);
	ABCHECK(nullptr != pWeaponTableData, false);

	APG_Weapon* NewWeapon = GetWorld()->SpawnActor<APG_Weapon>(pWeaponTableData->BPWeapon, FVector::ZeroVector, FRotator::ZeroRotator);
	ABCHECK(nullptr != NewWeapon, false);

	if (NewWeapon->SetWeaponData(nWeaponTalbeIndex) && false == SetWeapon(NewWeapon))
	{
		ABLOG(Error, TEXT("SetWeapon Failed!! (TableIndex : %d)"), nWeaponTalbeIndex);
		NewWeapon->Destroy();
		NewWeapon = nullptr;

		return false;
	}

	return true;
}

// PlayerState 로 옮기는 작업 필요
//bool APG_MyChar::EquipWeapon(const TSubclassOf<class APG_Weapon>& EquipWeaponClass)
//{
//	APG_Weapon* NewWeapon = GetWorld()->SpawnActor<APG_Weapon>(EquipWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
//	ABCHECK(nullptr != NewWeapon, false);
//
//	if (false == SetWeapon(NewWeapon))
//	{
//		NewWeapon->Destroy();
//		return false;
//	}
//
//	return true;
//}

// PlayerState 로 옮기는 작업 필요
bool APG_MyChar::SetWeapon(APG_Weapon* NewWeapon)
{
	if (CurrentWeapon.IsValid(true))
	{
		ABLOG(Warning, TEXT("Unequip Weapon : %s"), *CurrentWeapon->GetName());

		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	if (NewWeapon)
	{
		FName CharacterGunSocket(TEXT("Bip001-L-GunSocket"));
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CharacterGunSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
		ABAnimInstance->SetMontageFireReaction(CurrentWeapon->GetMontageFireReaction());

		ABLOG(Warning, TEXT("Equip Weapon : %s"), *NewWeapon->GetName());
	}

	return true;
}

// APG_Weapon 으로 함수 옮기는 작업 필요.
APG_MonChar* APG_MyChar::FindTargetToMonster()
{
	APG_MonChar* pFindTarget = nullptr;
	// 캐릭터 시야 내 주변 몬스터를 찾아 공격 애니메이션을 실행한다.
	// 실제 총알 발사는 공격 애니메이션의 애님 노티파이를 이용한다.
	if (CurrentWeapon.IsValid(true))
	{
		float fAttackRange = CurrentWeapon->GetAttackRange();

		TArray<FHitResult> HitResult;
		FCollisionQueryParams Params(NAME_None, false, this);
		
		bool bResult = GetWorld()->SweepMultiByChannel(
			HitResult,
			GetActorLocation(),
			GetActorLocation(),
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel4,
			FCollisionShape::MakeSphere(fAttackRange),
			Params);

//#ifdef ENABLE_DRAW_DEBUG
//		FVector TraceVec = GetActorForwardVector() * fAttackRange;
//		FVector Center = GetActorLocation();
//		float HalfHeight = fAttackRange;
//		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
//		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
//		float DebugLifeTime = 0.02f;
//
//		DrawDebugCapsule(GetWorld(),
//			Center,
//			HalfHeight,
//			fAttackRange,
//			CapsuleRot,
//			DrawColor,
//			false,
//			DebugLifeTime);
//#endif

		if (bResult)
		{
			// 캐릭터로부터 가장 가까운 몬스터를 찾아 타겟으로 정한다.
			float fFindNearDistance = fAttackRange;
			for (auto Monster : HitResult)
			{
				if (nullptr == Monster.GetActor())
					continue;

				float fDistance = GetDistanceTo(Monster.GetActor());
				if (fDistance < fFindNearDistance)
				{
					fFindNearDistance = fDistance;
					pFindTarget = Cast<APG_MonChar>(Monster.GetActor());
				}
			}
//#ifdef ENABLE_DRAW_DEBUG
//			if (pFindTarget)
//			{
//				DrawDebugPoint(GetWorld(), pFindTarget->GetActorLocation(), 10.0f, FColor::Yellow, false, 0.02f);
//				DrawDebugLine(GetWorld(), GetActorLocation(), pFindTarget->GetActorLocation(), FColor::Yellow, false, 0.02f);
//			}
//#endif
		}
	}

	return pFindTarget;
}

bool APG_MyChar::IsTargetMonster()
{
	ABCHECK(nullptr != ABAnimInstance, false);
	return ABAnimInstance->IsVaildTargetMonster();
}