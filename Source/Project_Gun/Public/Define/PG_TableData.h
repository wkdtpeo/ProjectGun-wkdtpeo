#pragma once

#include "Project_Gun/Project_Gun.h"
#include <Engine/DataTable.h>
#include "PG_TableData.generated.h"

#define PG_INVALID_INDEX		-1
#define PG_INFINITY_AMMO		-1		// 무한 총알
#define PG_MAX_WEAPON_SLOT		3		// 무기 최대 장착 수
#define PG_ADVIEW_REWARD		3		// 광고 보상 배수
#define PG_ADVIEW_SHOP_REWARD	200		// 무기고에서 광고 보았을 때 보상 포인트

//=========================================================================================
// 캐릭터
//=========================================================================================
USTRUCT(BlueprintType)
struct FPGCharacterMovementData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FPGCharacterMovementData() : MaxSpeed(0.0f), AccelerationSpeed(0.0f), BodyRotateSpeed(0.0f)
		, AttackBodyRotateSpeed(0.0f), AttackUpperBodyRotateSpeed(0.0f)
	{ }

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float	MaxSpeed;						// 최대 이동속도

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float	AccelerationSpeed;				// 이동 가속도

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float	BodyRotateSpeed;				// 이동시 캐릭터 회전속도

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float	AttackBodyRotateSpeed;			// 공격시 캐릭터 회전속도

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float	AttackUpperBodyRotateSpeed;		// 공격시 상체 회전속도
};

USTRUCT(BlueprintType)
struct FPGCharacterCameraData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FPGCharacterCameraData() : Pitch(-45.0f), Yaw(0.0f), Roll(0.0f), ArmLength(0.0f), Fov(0.0f), ScreenY(0.0f)
	{ }

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float	Pitch;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float	Yaw;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float	Roll;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float	ArmLength;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float	Fov;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float	ScreenY;
};

//=========================================================================================
// 몬스터
//=========================================================================================
UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	eMT_Normal = 0,				// 일반 몬스터
	eMT_SkillType,				// 스킬 사용하는 몬스터
	eMT_Division,				// 분열하는 몬스터
	eMT_DivisionSkillType,		// 스킬사용 및 분열하는 몬스터
	eMT_Boss,					// 보스

	eMT_Max
}; 

USTRUCT(BlueprintType)
struct FPGMonsterData
{
	GENERATED_BODY()

public:
	FPGMonsterData() : ID(0), MHP(0), MSpeed(0.0f), MAcceleration(0.0f), MRotateSpeed(0.0f), MBodyScale(1.0f), MDetectRange(0), MChaseRange(0), MDropRate(0.0f), MCollide(true)
	{ }

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32	ID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32	MHP;				// 몬스터 체력

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float	MSpeed;				// 몬스터의 이동 속도

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float	MAcceleration;		// 몬스터 이동 가속도

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float	MRotateSpeed;		// 몬스터가 캐릭터를 바라보기 위해 몸을 돌리는 속도

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float	MBodyScale;			// 몬스터의 체력에 따른 몸체의 크기

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32	MDetectRange;		// 몬스터가 플레이어 캐릭터를 인식하는 거리 (시야)

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32	MChaseRange;		// 몬스터가 플레이어 캐릭터를 쫓아오는 최대 거리

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float	MDropRate;			// 몬스터가 재화를 드랍하는 확률

		//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		//enum	MBeHit;				// 몬스터가 피격당할 때 발생할 효과 (0 : 없음, 1: 피격 위치 기준 뒤로 30cm)

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool	MCollide;			// 몬스터가 몬스터끼리 충돌할지 말지 결정

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	//USkeletalMesh* MMeshPath;
};

USTRUCT(BlueprintType)
struct FPGMonsterData_Normal : public FPGMonsterData
{
	GENERATED_BODY()

public:
	FPGMonsterData_Normal() : FPGMonsterData(){ }

};

USTRUCT(BlueprintType)
struct FPGMonsterData_SkillType : public FPGMonsterData
{
	GENERATED_BODY()

public:
	FPGMonsterData_SkillType() : MWeaponCoolTime(0.0f), MDashReadyTime(0.0f), MDashCoolTime(0.0f), MDashMoveDistance(0.0f), MDashSpeed(0.0f),
		MDivisionCount(0), MDivisionAmount(0), MDivisionSpawnRange(0.0f), MRemainDivisionCount(0) { }

	//===========================================
	// Weapon
	//===========================================
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MWeaponCoolTime;		// 스킬 타입 몬스터의 공격 시전 쿨타임(ms)

	//===========================================
	// Dash
	//===========================================
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MDashReadyTime;		// 대쉬 준비 시간

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MDashCoolTime;		// 대쉬 쿨타임

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MDashMoveDistance;	// 대쉬 이동거리

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MDashSpeed;			// 대쉬 속도


	//===========================================
	// Division
	//===========================================
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MDivisionCount;		// 분화 몬스터의 분화 횟수. 횟수를 입력하면, 100에서 횟수+1을 나누어 해당 HP대역에 들어가면 몬스터가 분화한다.

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MDivisionAmount;		// 생성될(분화) 몬스터 갯수. 해당 개수만큼 현재위치 앞쪽 50 거리로 시계방향 Rotate시켜 개수에 맞춰 생성각도를 360을 나눠 생성한다.

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MDivisionSpawnRange;	// 분화시 기준 몬스터에서 해당 범위만큼 떨어져서 생성된다.

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	int32 MRemainDivisionCount;	// 분화 몬스터의 남은 분화 횟수
};

USTRUCT(BlueprintType)
struct FPGMonsterData_Division : public FPGMonsterData
{
	GENERATED_BODY()

public:
	FPGMonsterData_Division() : MDivisionCount(0), MDivisionAmount(0), MDivisionSpawnRange(0.0f), MRemainDivisionCount(0) { }

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MDivisionCount;		// 분화 몬스터의 분화 횟수. 횟수를 입력하면, 100에서 횟수+1을 나누어 해당 HP대역에 들어가면 몬스터가 분화한다.

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MDivisionAmount;		// 생성될(분화) 몬스터 갯수. 해당 개수만큼 현재위치 앞쪽 50 거리로 시계방향 Rotate시켜 개수에 맞춰 생성각도를 360을 나눠 생성한다.

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MDivisionSpawnRange;	// 분화시 기준 몬스터에서 해당 범위만큼 떨어져서 생성된다.

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	int32 MRemainDivisionCount;	// 분화 몬스터의 남은 분화 횟수
};


USTRUCT(BlueprintType)
struct FPGMonsterData_DivisionSkillType : public FPGMonsterData_Division
{
	GENERATED_BODY()

public:
	FPGMonsterData_DivisionSkillType() : MWeaponCoolTime(0), MAttackSpeed(0), MAttackMethod(0){ }

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MWeaponCoolTime;		// 스킬 타입 몬스터의 공격 시전 쿨타임(ms)

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MAttackSpeed;			// 스킬 타입 몬스터의 공격 시전 속도(ms)

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MAttackMethod;		// 스킬 타입 몬스터의 공격 방식	(Projectile의 경우 이곳에 Projectile 이펙트BP 삽입, 돌진의 경우 Decal 이펙트 삽입)
};

USTRUCT(BlueprintType)
struct FPGMonsterData_Boss : public FPGMonsterData
{
	GENERATED_BODY()

public:
	FPGMonsterData_Boss() { }

};


//=========================================================================================
// 무기 데이터
//=========================================================================================
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	eWT_Rifle = 0,
	eWT_GrenadeLauncher,
	eWT_LaserBemGun,
	eWT_FlameGun,

	eWT_Max
};

USTRUCT(BlueprintType)
struct FPGWeaponData
{
	GENERATED_BODY()

public:
	FPGWeaponData() : Bullet(0), AttackRange(0), BulletRange(0), BulletSpeed(0), BPower(0), Collide(false), PowerUpPerLevelUp(0), MaxPower(0),
		Mag(0), MagUpPerLevelUp(0),	MaxMag(0), Vibration(0), VibrationCool(0), WPushRange(0)
	{ }

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FString Name;

	// 발사체의 외관(Mesh/텍스처)
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<class APG_Projectile> BProjectile;
	//TSubclassOf<class APG_Projectile> BProjectile;

	// 1회 발사할 때 발사체의 생성 개수. 해당 항목이 입력되면 아래 생성위치, 
	// 발사각도가 각 발사체마다 입력되게 생긴다. 예를들어 Bullet에 3을 입력하면 아래로 BStartPosition, BAngle, BEffect가 3개씩 생긴다. 
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32	Bullet;		

	// 몬스터가 접근해왔을 때 공격하기 시작 하는 공격 범위.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 AttackRange;

	// 발사체가 도달하는 최대 범위. 이후 발사체는 무조건 소멸한다.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 BulletRange;

	// 총알 속도
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 BulletSpeed;
	
	// 발사체의 공격력.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 BPower;
	
	// 발사체가 몬스터를 관통할지 여부. (단, 오브젝트와 충돌할 경우 이 옵션에 영향을 받지 않고 무조건 소멸한다.)
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool Collide;

	// 무기 공격력 업그레이드를 할 때 마다 올라가는 공격력 수치
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 PowerUpPerLevelUp;

	// 최대 무기 공격력 수치
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MaxPower;

	// 기본 탄창 수. (초기에 얻으면 설정되어 있는 기본 최대 탄창 수)
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Mag;

	// 상점에서 AMMO레벨업 할 때마다 올라가는 무기 최대 탄창 수.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MagUpPerLevelUp;
	
	// 최대 탄창 수. (성장으로 최대로 높힐 수 있는 최대 탄창 수)
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MaxMag;

	// 발사체가 몬스터 피격 시 기기에 주는 진동 강도.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Vibration;

	// 진동이 울리는 주기(1초에 Int 회).
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 VibrationCool;

	// 몬스터 피격 시 몬스터가 후방으로 밀리는 범위(Cm)
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 WPushRange;
};

//=========================================================================================
// 스테이지 데이터
//=========================================================================================
USTRUCT(BlueprintType)
struct FPGStageData : public FTableRowBase
{
	GENERATED_BODY()
public :
	FPGStageData(): M_Amount(0), Time(0), Multi_Kill(0), StageClearReward(0), StarReward(0)
	{ }

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	FString StageName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 M_Amount;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 Time;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 Multi_Kill;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 StageClearReward;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 StarReward;
};

//=========================================================================================
// 스킬 데이터
//=========================================================================================
USTRUCT(BlueprintType)
struct FPGLethalAttackData : public FTableRowBase
{
	GENERATED_BODY()

	public:
	FPGLethalAttackData() : LMinPoint(0), LMaxPoint(0), LMinRange(0), LMaxRange(0), LPower(0), LEffectDuration(0), Vibration(0), VibrationCool(0)
	{ }

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	FString Name;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	TSubclassOf<class APG_LethalAttack> BLethalAttack;

	// 필살기 사용에 필요한 최소 몬스터 킬 수치.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 LMinPoint;

	// 필살기 사용에 쓸 수 있는 최대 몬스터 킬 수치.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 LMaxPoint;

	// 필살기가 펼쳐질 수 있는 최소 타격 범위.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 LMinRange;

	// 필살기가 펼쳐질 수 있는 최대 타격 범위.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 LMaxRange;

	// 필살기의 공격력
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 LPower;

	// 필살기 효과의 지속 시간.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 LEffectDuration;

	// 발사체가 몬스터 피격 시 기기에 주는 진동 강도.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 Vibration;

	// 진동이 울리는 주기(1초에 Int 회).
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 VibrationCool;
};

//=========================================================================================
// Weapon Shop 데이터
//=========================================================================================
UENUM()
enum class EBuyType : int32
{
	eBT_Weapon = 0,
	eBT_Power,
	eBT_Ammo,
	eBT_Max,
};

USTRUCT(BlueprintType)
struct FPGWeaponTableData : public FTableRowBase
{
	GENERATED_BODY()
public:
	FPGWeaponTableData() : ServiceOpen(false), MontageFireReaction(nullptr), CostUnlock(0), CostUpgradePower(0), CostUpgradeAmmo(0) { }

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	TSubclassOf<class APG_Weapon> BPWeapon;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	bool ServiceOpen;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	struct FPGWeaponData WeaponData;

	// UI에 표현할 무기 이미지
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Image")
	struct FSlateBrush WeaponImage;

	// 무기 발사시 사용되는 애니메이션
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Animation")
	class UAnimMontage* MontageFireReaction;

	// 무기 잠금 해제 가격
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG ShopData")
	int32 CostUnlock;

	// 무기 공격력 강화 가격
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG ShopData")
	int32 CostUpgradePower;

	// 무기 탄약 수 증가 가격
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG ShopData")
	int32 CostUpgradeAmmo;
};


//=========================================================================================
// 광고 데이터
//=========================================================================================
UENUM()
enum class EADType : int32
{
	eAT_WeaponShop = 0,		// 무기고 내 광고
	eAT_StageClear3X,		// 스테이지 클리어 후 3배 보상 광고
	eAT_StagePlayAD,		// 스테이지 플레이 광고
	eAT_Max,
};

USTRUCT(BlueprintType)
struct FPGADTableData : public FTableRowBase
{
	GENERATED_BODY()
public:
	FPGADTableData() : Type(EADType::eAT_Max), ReViewableTime(0), TimeDeduction_MainLobby(false), TimeDeduction_StagePlay(false) { }

	// 광고 타입
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	EADType Type;	

	// 재시청 제한시간
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 ReViewableTime;

	// 메인로비에서 재시청 시간을 차감한다.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool TimeDeduction_MainLobby;

	// 스테이지 플레이에서 재시청 시간을 차감한다.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool TimeDeduction_StagePlay;
};