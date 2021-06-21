#pragma once

#include "Project_Gun/Project_Gun.h"
#include <Engine/DataTable.h>
#include "PG_TableData.generated.h"

#define PG_INVALID_INDEX		-1
#define PG_INFINITY_AMMO		-1		// ���� �Ѿ�
#define PG_MAX_WEAPON_SLOT		3		// ���� �ִ� ���� ��
#define PG_ADVIEW_REWARD		3		// ���� ���� ���
#define PG_ADVIEW_SHOP_REWARD	200		// ������� ���� ������ �� ���� ����Ʈ

//=========================================================================================
// ĳ����
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
	float	MaxSpeed;						// �ִ� �̵��ӵ�

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float	AccelerationSpeed;				// �̵� ���ӵ�

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float	BodyRotateSpeed;				// �̵��� ĳ���� ȸ���ӵ�

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float	AttackBodyRotateSpeed;			// ���ݽ� ĳ���� ȸ���ӵ�

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float	AttackUpperBodyRotateSpeed;		// ���ݽ� ��ü ȸ���ӵ�
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
// ����
//=========================================================================================
UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	eMT_Normal = 0,				// �Ϲ� ����
	eMT_SkillType,				// ��ų ����ϴ� ����
	eMT_Division,				// �п��ϴ� ����
	eMT_DivisionSkillType,		// ��ų��� �� �п��ϴ� ����
	eMT_Boss,					// ����

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
	int32	MHP;				// ���� ü��

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float	MSpeed;				// ������ �̵� �ӵ�

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float	MAcceleration;		// ���� �̵� ���ӵ�

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float	MRotateSpeed;		// ���Ͱ� ĳ���͸� �ٶ󺸱� ���� ���� ������ �ӵ�

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float	MBodyScale;			// ������ ü�¿� ���� ��ü�� ũ��

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32	MDetectRange;		// ���Ͱ� �÷��̾� ĳ���͸� �ν��ϴ� �Ÿ� (�þ�)

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32	MChaseRange;		// ���Ͱ� �÷��̾� ĳ���͸� �Ѿƿ��� �ִ� �Ÿ�

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float	MDropRate;			// ���Ͱ� ��ȭ�� ����ϴ� Ȯ��

		//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		//enum	MBeHit;				// ���Ͱ� �ǰݴ��� �� �߻��� ȿ�� (0 : ����, 1: �ǰ� ��ġ ���� �ڷ� 30cm)

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool	MCollide;			// ���Ͱ� ���ͳ��� �浹���� ���� ����

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
	float MWeaponCoolTime;		// ��ų Ÿ�� ������ ���� ���� ��Ÿ��(ms)

	//===========================================
	// Dash
	//===========================================
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MDashReadyTime;		// �뽬 �غ� �ð�

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MDashCoolTime;		// �뽬 ��Ÿ��

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MDashMoveDistance;	// �뽬 �̵��Ÿ�

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MDashSpeed;			// �뽬 �ӵ�


	//===========================================
	// Division
	//===========================================
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MDivisionCount;		// ��ȭ ������ ��ȭ Ƚ��. Ƚ���� �Է��ϸ�, 100���� Ƚ��+1�� ������ �ش� HP�뿪�� ���� ���Ͱ� ��ȭ�Ѵ�.

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MDivisionAmount;		// ������(��ȭ) ���� ����. �ش� ������ŭ ������ġ ���� 50 �Ÿ��� �ð���� Rotate���� ������ ���� ���������� 360�� ���� �����Ѵ�.

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MDivisionSpawnRange;	// ��ȭ�� ���� ���Ϳ��� �ش� ������ŭ �������� �����ȴ�.

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	int32 MRemainDivisionCount;	// ��ȭ ������ ���� ��ȭ Ƚ��
};

USTRUCT(BlueprintType)
struct FPGMonsterData_Division : public FPGMonsterData
{
	GENERATED_BODY()

public:
	FPGMonsterData_Division() : MDivisionCount(0), MDivisionAmount(0), MDivisionSpawnRange(0.0f), MRemainDivisionCount(0) { }

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MDivisionCount;		// ��ȭ ������ ��ȭ Ƚ��. Ƚ���� �Է��ϸ�, 100���� Ƚ��+1�� ������ �ش� HP�뿪�� ���� ���Ͱ� ��ȭ�Ѵ�.

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MDivisionAmount;		// ������(��ȭ) ���� ����. �ش� ������ŭ ������ġ ���� 50 �Ÿ��� �ð���� Rotate���� ������ ���� ���������� 360�� ���� �����Ѵ�.

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MDivisionSpawnRange;	// ��ȭ�� ���� ���Ϳ��� �ش� ������ŭ �������� �����ȴ�.

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	int32 MRemainDivisionCount;	// ��ȭ ������ ���� ��ȭ Ƚ��
};


USTRUCT(BlueprintType)
struct FPGMonsterData_DivisionSkillType : public FPGMonsterData_Division
{
	GENERATED_BODY()

public:
	FPGMonsterData_DivisionSkillType() : MWeaponCoolTime(0), MAttackSpeed(0), MAttackMethod(0){ }

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MWeaponCoolTime;		// ��ų Ÿ�� ������ ���� ���� ��Ÿ��(ms)

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MAttackSpeed;			// ��ų Ÿ�� ������ ���� ���� �ӵ�(ms)

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MAttackMethod;		// ��ų Ÿ�� ������ ���� ���	(Projectile�� ��� �̰��� Projectile ����ƮBP ����, ������ ��� Decal ����Ʈ ����)
};

USTRUCT(BlueprintType)
struct FPGMonsterData_Boss : public FPGMonsterData
{
	GENERATED_BODY()

public:
	FPGMonsterData_Boss() { }

};


//=========================================================================================
// ���� ������
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

	// �߻�ü�� �ܰ�(Mesh/�ؽ�ó)
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<class APG_Projectile> BProjectile;
	//TSubclassOf<class APG_Projectile> BProjectile;

	// 1ȸ �߻��� �� �߻�ü�� ���� ����. �ش� �׸��� �ԷµǸ� �Ʒ� ������ġ, 
	// �߻簢���� �� �߻�ü���� �Էµǰ� �����. ������� Bullet�� 3�� �Է��ϸ� �Ʒ��� BStartPosition, BAngle, BEffect�� 3���� �����. 
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32	Bullet;		

	// ���Ͱ� �����ؿ��� �� �����ϱ� ���� �ϴ� ���� ����.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 AttackRange;

	// �߻�ü�� �����ϴ� �ִ� ����. ���� �߻�ü�� ������ �Ҹ��Ѵ�.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 BulletRange;

	// �Ѿ� �ӵ�
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 BulletSpeed;
	
	// �߻�ü�� ���ݷ�.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 BPower;
	
	// �߻�ü�� ���͸� �������� ����. (��, ������Ʈ�� �浹�� ��� �� �ɼǿ� ������ ���� �ʰ� ������ �Ҹ��Ѵ�.)
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool Collide;

	// ���� ���ݷ� ���׷��̵带 �� �� ���� �ö󰡴� ���ݷ� ��ġ
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 PowerUpPerLevelUp;

	// �ִ� ���� ���ݷ� ��ġ
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MaxPower;

	// �⺻ źâ ��. (�ʱ⿡ ������ �����Ǿ� �ִ� �⺻ �ִ� źâ ��)
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Mag;

	// �������� AMMO������ �� ������ �ö󰡴� ���� �ִ� źâ ��.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MagUpPerLevelUp;
	
	// �ִ� źâ ��. (�������� �ִ�� ���� �� �ִ� �ִ� źâ ��)
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MaxMag;

	// �߻�ü�� ���� �ǰ� �� ��⿡ �ִ� ���� ����.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Vibration;

	// ������ �︮�� �ֱ�(1�ʿ� Int ȸ).
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 VibrationCool;

	// ���� �ǰ� �� ���Ͱ� �Ĺ����� �и��� ����(Cm)
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 WPushRange;
};

//=========================================================================================
// �������� ������
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
// ��ų ������
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

	// �ʻ�� ��뿡 �ʿ��� �ּ� ���� ų ��ġ.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 LMinPoint;

	// �ʻ�� ��뿡 �� �� �ִ� �ִ� ���� ų ��ġ.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 LMaxPoint;

	// �ʻ�Ⱑ ������ �� �ִ� �ּ� Ÿ�� ����.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 LMinRange;

	// �ʻ�Ⱑ ������ �� �ִ� �ִ� Ÿ�� ����.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 LMaxRange;

	// �ʻ���� ���ݷ�
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 LPower;

	// �ʻ�� ȿ���� ���� �ð�.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 LEffectDuration;

	// �߻�ü�� ���� �ǰ� �� ��⿡ �ִ� ���� ����.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 Vibration;

	// ������ �︮�� �ֱ�(1�ʿ� Int ȸ).
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Data")
	int32 VibrationCool;
};

//=========================================================================================
// Weapon Shop ������
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

	// UI�� ǥ���� ���� �̹���
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Image")
	struct FSlateBrush WeaponImage;

	// ���� �߻�� ���Ǵ� �ִϸ��̼�
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG Animation")
	class UAnimMontage* MontageFireReaction;

	// ���� ��� ���� ����
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG ShopData")
	int32 CostUnlock;

	// ���� ���ݷ� ��ȭ ����
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG ShopData")
	int32 CostUpgradePower;

	// ���� ź�� �� ���� ����
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PG ShopData")
	int32 CostUpgradeAmmo;
};


//=========================================================================================
// ���� ������
//=========================================================================================
UENUM()
enum class EADType : int32
{
	eAT_WeaponShop = 0,		// ����� �� ����
	eAT_StageClear3X,		// �������� Ŭ���� �� 3�� ���� ����
	eAT_StagePlayAD,		// �������� �÷��� ����
	eAT_Max,
};

USTRUCT(BlueprintType)
struct FPGADTableData : public FTableRowBase
{
	GENERATED_BODY()
public:
	FPGADTableData() : Type(EADType::eAT_Max), ReViewableTime(0), TimeDeduction_MainLobby(false), TimeDeduction_StagePlay(false) { }

	// ���� Ÿ��
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	EADType Type;	

	// ���û ���ѽð�
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 ReViewableTime;

	// ���ηκ񿡼� ���û �ð��� �����Ѵ�.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool TimeDeduction_MainLobby;

	// �������� �÷��̿��� ���û �ð��� �����Ѵ�.
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool TimeDeduction_StagePlay;
};