// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PG_MonChar.h"
#include "Define/PG_TableData.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Game/GameState/PG_GameStateStage.h"
#include "Character/PG_MyChar.h"
#include "AI/PG_AICon_Mon.h"
#include "Character/Component/PG_ComponentCharScale.h"
#include <Components/WidgetComponent.h>
#include <Components/ProgressBar.h>
#include <GameFramework/DamageType.h>
#include <DrawDebugHelpers.h>

// Sets default values
APG_MonChar::APG_MonChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#ifdef TEST_CODE
	// 임시코드 : 가데이터 몬스터 리소스 위치 수정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
#endif

	AIControllerClass = APG_AICon_Mon::StaticClass();

	CharScaleComponent = CreateDefaultSubobject<UPG_ComponentCharScale>(TEXT("CHARSCALECOMPONENT"));

	// 플레이어 오버랩 이벤트 설정
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APG_MonChar::OnCharacterHit);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &APG_MonChar::OnHitCallback);

#ifdef ENABLE_DRAW_DEBUG
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBarWidget->SetupAttachment(GetMesh());
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("WidgetBlueprint'/Game/PG/Blueprints/WidgetBP/WB_HPBar.WB_HPBar_C'"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(50.0f, 50.0f));
	}
#endif

	m_fMaxHP = 0.0f;
}

// Called when the game starts or when spawned
void APG_MonChar::BeginPlay()
{
	Super::BeginPlay();

#ifdef ENABLE_DRAW_DEBUG
	if (HPBarWidget)
		HPBarWidget->SetHiddenInGame(true);
#endif

	ABCHECK(SetDefaultMonsterData());

	ABCHECK(CharScaleComponent);
	CharScaleComponent->SetActorScaleRange(m_vBeginScale * MonsterData->MBodyScale, m_vBeginScale);
	SetHP(MonsterData->MHP);

	ABCHECK(RunAI());
}

void APG_MonChar::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	m_vBeginScale = GetCapsuleComponent()->GetComponentScale();
}

void APG_MonChar::FellOutOfWorld(const class UDamageType& dmgType)
{
	if (dmgType.bCausedByWorld)
		SetDie(false);
	else
		Super::FellOutOfWorld(dmgType);
}

// Called every frame
void APG_MonChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateRotation(DeltaTime);
}

// Called to bind functionality to input
void APG_MonChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APG_MonChar::UpdateRotation(float DeltaTime)
{
	// 몬스터가 움직이지 않고 캐릭터 방향으로 회전해야하는 경우
	if (m_bDoNotMoving && false == m_bDoNotRotation)
	{
		APG_MyChar* pPG_MyChar = FindTargetToMyChar();
		if (pPG_MyChar)
		{
			FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), pPG_MyChar->GetActorLocation());
			FRotator rotator = FMath::RInterpTo(m_vTargetRotation, PlayerRot, DeltaTime, MonsterData->MRotateSpeed);
			rotator.Pitch = 0.0f;
			m_vTargetRotation = rotator;
			SetActorRotation(rotator);
		}
		else
		{
			m_vTargetRotation = GetActorRotation();
		}
	}
}

void APG_MonChar::OnCharacterHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult)
{
	auto PlayerCharacter = Cast<APG_MyChar>(OtherActor);
	if (PlayerCharacter)
	{
		//ABLOG(Warning, TEXT("Attack Actor : %s"), *GetName());
		PlayerCharacter->OnAttackedByMonster();
		return;
	}
}

void APG_MonChar::OnHitCallback(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}

bool APG_MonChar::RunAI()
{
	AIController = Cast<APG_AICon_Mon>(GetController());
	if (nullptr == AIController)
	{
		SpawnDefaultController();
		AIController = Cast<APG_AICon_Mon>(GetController());
	}

	ABCHECK(nullptr != AIController, false);

	return AIController->RunAI();
}

void APG_MonChar::StopAI()
{
	ABCHECK(nullptr != AIController);
	AIController->StopAI();
}

void APG_MonChar::SetHP(int32 nHP)
{
	MonsterData->MHP = nHP;

//#ifdef ENABLE_DRAW_DEBUG
//	UProgressBar* HPProgressBar = Cast<UProgressBar>(HPBarWidget->GetUserWidgetObject()->GetWidgetFromName(TEXT("PB_HPBar")));
//	if (HPProgressBar)
//	{
//		HPProgressBar->SetPercent(GetHPRatio());
//	}
//#endif

	if (CharScaleComponent)
		CharScaleComponent->SetActorScale(GetHPRatio());
}

void APG_MonChar::SetDie(bool IsUseLetharAttack)
{
	APG_GameStateStage* AGameStateStage = Cast<APG_GameStateStage>(GetWorld()->GetGameState());
	if (AGameStateStage)
	{
		AGameStateStage->MonsterDie(this, IsUseLetharAttack);
		Destroy();
	}
	else
	{
		ABLOG_S(Warning);
	}
}
EMonsterType APG_MonChar::GetMonsterType()
{
	return MonsterType;
}

const FPGMonsterData* APG_MonChar::GetMonsterData()
{
	return MonsterData;
}

int32 APG_MonChar::GetDetectRange() const
{
	return MonsterData ? MonsterData->MDetectRange : 0;
}

int32 APG_MonChar::GetChaseRange() const
{
	return MonsterData ? MonsterData->MChaseRange : 0;
}

float APG_MonChar::GetHPRatio() const
{
	ABCHECK(MonsterData, 0.0f);

	return (MonsterData->MHP <= 0) ? 0.0f : ((float)MonsterData->MHP / (float)m_fMaxHP);
}

float APG_MonChar::GetMaxHP() const
{
	return m_fMaxHP;
}

float APG_MonChar::GetDropRate() const
{
	return MonsterData ? MonsterData->MDropRate : 0.0f;
}

APG_MyChar* APG_MonChar::FindTargetToMyChar()
{
	auto pMyChar = Cast<APG_MyChar>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ABCHECK(nullptr != pMyChar, nullptr);

	FVector vMonLocation = GetActorLocation();
	FVector vPlayerLocation = pMyChar->GetActorLocation();

	if (FMath::Abs<int32>(FVector::Distance(vMonLocation, vPlayerLocation)) <= GetDetectRange())
		return pMyChar;

	return nullptr;
}

bool APG_MonChar::SetDefaultMonsterData()
{
	ABCHECK(nullptr != MonsterData, false);

	bUseControllerRotationYaw = false;
	
	m_bDoNotRotation = (0.0f >= MonsterData->MRotateSpeed);
	m_bDoNotMoving = (0.0f >= MonsterData->MSpeed);
	GetCharacterMovement()->bOrientRotationToMovement = !m_bDoNotRotation;
	GetCharacterMovement()->bUseControllerDesiredRotation = !m_bDoNotRotation;
	if (false == m_bDoNotRotation)
		GetCharacterMovement()->RotationRate = FRotator(0.0f, MonsterData->MRotateSpeed, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = MonsterData->MSpeed;
	GetCharacterMovement()->MaxAcceleration = MonsterData->MAcceleration;


	// 콜리전 설정
	if (MonsterData->MCollide)
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
	else
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster_NoCollide"));

	m_fMaxHP = MonsterData->MHP;
	SetHP(MonsterData->MHP);

	return true;
}

void APG_MonChar::OnAssetLoadCharacterCompleted()
{
	ABCHECK(AssetStreamingHandle.IsValid());

	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());
	AssetStreamingHandle.Reset();
	ABCHECK(nullptr != AssetLoaded);
	GetMesh()->SetSkeletalMesh(AssetLoaded);


	AIController = Cast<APG_AICon_Mon>(GetController());
	ABCHECK(nullptr != AIController);

	if (false == AIController->RunAI())
	{
		ABLOG(Warning, TEXT("AIController RunAI Failed!!"));
	}
}

bool APG_MonChar::OnDamage(int32 nDamage, bool IsUseLetharAttack/* = false*/)
{
	SetHP(MonsterData->MHP - nDamage);

	if (MonsterData->MHP <= 0.0f)
	{
		// Die
		SetDie(IsUseLetharAttack);
		return true;
	}

	return false;
}

void APG_MonChar::DebugOnly()
{
	//if (ChangeMonsterData)
	//{
	//	if (ID != MonsterData.ID)
	//	{
	//		auto PGGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	//		ABCHECK(nullptr != PGGameInstance);
	//		const FPGMonsterData* pMonsterData = PGGameInstance->GetMonsterData(MonsterType, MonsterData.ID);
	//		if (pMonsterData)
	//		{
	//			MonsterData = *pMonsterData;
	//			AssetLoadCharacter(pMonsterData->MMeshPath->GetPathName());
	//		}
	//	}

	//	SetDefaultMonsterData(&MonsterData);
	//	ChangeMonsterData = false;
	//}
}