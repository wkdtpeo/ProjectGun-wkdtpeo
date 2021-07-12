// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/GameMode/PG_GameModeBase.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "SaveData/PG_SaveStageData.h"
#include "SaveData/PG_SavePlayerData.h"
#include "Player/PlayerState/PG_MyPlayerState.h"
#include "BlueScriptObject/Weapon/PG_Weapon.h"

APG_GameModeBase::APG_GameModeBase()
{
	m_pSavePlayerData = nullptr;
	m_pSelectSaveStageData = nullptr;
	PlayerStateClass = APG_MyPlayerState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
}

void APG_GameModeBase::PostLoad()
{
	Super::PostLoad();


}

void APG_GameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	G_PGWorld = GetWorld();

	// 플레이어 세이브 데이터 로드
	{
		m_pSavePlayerData = LoadSavePlayerData();
		ABCHECK(nullptr != m_pSavePlayerData);
	}

	m_nPalyStageID = m_pSavePlayerData->m_nLastPlayStageID;
	if (0 == m_pSavePlayerData->m_nLastPlayStageID)
		m_nPalyStageID = 1;

	// 기본 스테이지 선택하기
	{
		m_pSelectSaveStageData = LoadSaveStageData(m_nPalyStageID);
		ABCHECK(nullptr != m_pSelectSaveStageData);
	}
}

void APG_GameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void APG_GameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void APG_GameModeBase::BeginDestroy()
{
	G_PGWorld = nullptr;
	Super::BeginDestroy();
}

void APG_GameModeBase::Tick(float DeltaTime)
{
	UpdateGameInstance(DeltaTime);
	Super::Tick(DeltaTime);
}

bool APG_GameModeBase::StartGame()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	ABCHECK(pGameInstance->GetTableStageData(m_nPalyStageID), false);

	auto pGameModeBase = Cast<APG_GameModeBase>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != pGameModeBase, false);

	auto pSavePlayerData = GetSavePlayerData();
	ABCHECK(nullptr != pSavePlayerData, false);

	pSavePlayerData->m_nLastPlayStageID = m_nPalyStageID;
	SavePlayerData();

	FString NextStageName = pGameInstance->GetStageName(m_nPalyStageID);
	//FString DebugMsg = FString::Printf(TEXT("Play Stage : %s"), *NextStageName);
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, DebugMsg);

	UGameplayStatics::OpenLevel(GetWorld(), *NextStageName);

	return true;
}

void APG_GameModeBase::StartGame_NextStage()
{
	m_nPalyStageID++;
	StartGame();
}

void APG_GameModeBase::RestartGame()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	FString RestartStageName = pGameInstance->GetStageName(m_nPalyStageID);
	//FString DebugMsg = FString::Printf(TEXT("Stage Restart : %s"), *RestartStageName);
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, DebugMsg);

	UGameplayStatics::OpenLevel(GetWorld(), *RestartStageName);
}

bool APG_GameModeBase::SetNextStageID()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	int32 nNextStage = m_nPalyStageID + 1;

	if (pGameInstance->GetTableStageData(nNextStage))
	{
		int32 nBackupPlayStageID = m_nPalyStageID;
		m_nPalyStageID = nNextStage;

		auto pGameModeBase = Cast<APG_GameModeBase>(GetWorld()->GetAuthGameMode());
		ABCHECK(nullptr != pGameModeBase, false);

		if (false == pGameModeBase->SetSelectSaveStageData(m_nPalyStageID))
		{
			m_nPalyStageID = nBackupPlayStageID;
			return false;
		}

		return true;
	}

	return false;
}

bool APG_GameModeBase::SetPrevStageID()
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	int32 nPrevStage = m_nPalyStageID - 1;

	if (pGameInstance->GetTableStageData(nPrevStage))
	{
		int32 nBackupPlayStageID = m_nPalyStageID;
		m_nPalyStageID = nPrevStage;

		auto pGameModeBase = Cast<APG_GameModeBase>(GetWorld()->GetAuthGameMode());
		ABCHECK(nullptr != pGameModeBase, false);

		if (false == pGameModeBase->SetSelectSaveStageData(m_nPalyStageID))
		{
			m_nPalyStageID = nBackupPlayStageID;
			return false;
		}

		return true;
	}

	return false;
}

bool APG_GameModeBase::SetPlayStageID(int32 nPlayStageID)
{
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);
	ABCHECK(pGameInstance->GetTableStageData(nPlayStageID), false);

	m_nPalyStageID = nPlayStageID;
	return true;
}

bool APG_GameModeBase::SetSelectSaveStageData(int32 a_nStageID)
{
	m_pSelectSaveStageData = LoadSaveStageData(a_nStageID);
	return (nullptr != m_pSelectSaveStageData);
}

bool APG_GameModeBase::IsFirstPlayStage(int32 a_nStageID)
{
	auto pSaveStageData = Cast<UPG_SaveStageData>(UGameplayStatics::LoadGameFromSlot(UPG_SaveStageData::GetSlotName(a_nStageID), UPG_SaveStageData::GetPlayerIndex()));
	return (nullptr == pSaveStageData);
}

bool APG_GameModeBase::IsUnlockWeapon(int32 a_nWeaponIndex)
{
	ABCHECK(nullptr != m_pSavePlayerData, false);

	auto WeaponOwnData = m_pSavePlayerData->WeaponOwnData.Find(a_nWeaponIndex);
	if (nullptr == WeaponOwnData)
		return false;

	return WeaponOwnData->IsUnlock;
}

bool APG_GameModeBase::IsMaxLevelPower(int32 a_nWeaponIndex)
{
	ABCHECK(nullptr != m_pSavePlayerData, false);
	auto WeaponOwnData = m_pSavePlayerData->WeaponOwnData.Find(a_nWeaponIndex);
	if (nullptr == WeaponOwnData)
		return false;

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	auto pWeaponTableData = pGameInstance->GetWeaponTableData(a_nWeaponIndex);
	ABCHECK(nullptr != pWeaponTableData, false);

	// Max Level 체크
	int32 nPower = m_pSavePlayerData->WeaponOwnData[a_nWeaponIndex].PowerLevel * pWeaponTableData->WeaponData.PowerUpPerLevelUp;
	return (nPower >= pWeaponTableData->WeaponData.MaxPower);
}

bool APG_GameModeBase::IsMaxLevelAmmo(int32 a_nWeaponIndex)
{
	ABCHECK(nullptr != m_pSavePlayerData, false);
	auto WeaponOwnData = m_pSavePlayerData->WeaponOwnData.Find(a_nWeaponIndex);
	if (nullptr == WeaponOwnData)
		return false;

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	auto pWeaponTableData = pGameInstance->GetWeaponTableData(a_nWeaponIndex);
	ABCHECK(nullptr != pWeaponTableData, false);

	// Max Level 체크
	int32 nMag = m_pSavePlayerData->WeaponOwnData[a_nWeaponIndex].MagLevel * pWeaponTableData->WeaponData.MagUpPerLevelUp;
	return (nMag >= pWeaponTableData->WeaponData.MaxMag);
}

bool APG_GameModeBase::IsAbleBuyRewardPointPower(int32 a_nWeaponIndex)
{
	ABCHECK(nullptr != m_pSavePlayerData, false);
	auto WeaponOwnData = m_pSavePlayerData->WeaponOwnData.Find(a_nWeaponIndex);
	if (nullptr == WeaponOwnData)
		return false;

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	auto pWeaponTableData = pGameInstance->GetWeaponTableData(a_nWeaponIndex);
	ABCHECK(nullptr != pWeaponTableData, false);

	return (m_pSavePlayerData->m_nRewardPoint >= pWeaponTableData->CostUpgradePower);
}

bool APG_GameModeBase::IsAbleBuyRewardPointAmmo(int32 a_nWeaponIndex)
{
	ABCHECK(nullptr != m_pSavePlayerData, false);
	auto WeaponOwnData = m_pSavePlayerData->WeaponOwnData.Find(a_nWeaponIndex);
	if (nullptr == WeaponOwnData)
		return false;

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	auto pWeaponTableData = pGameInstance->GetWeaponTableData(a_nWeaponIndex);
	ABCHECK(nullptr != pWeaponTableData, false);

	return (m_pSavePlayerData->m_nRewardPoint >= pWeaponTableData->CostUpgradeAmmo);
}

bool APG_GameModeBase::BuyWeapon(int32 a_nWeaponIndex)
{
	m_kLastBuyErrorMsg.Empty();

	if (IsUnlockWeapon(a_nWeaponIndex))
	{
		ABLOG(Warning, TEXT("The weapon has already been released. (WeaponIndex : %d)"), a_nWeaponIndex);
		m_kLastBuyErrorMsg = TEXT("The weapon has already been released.");
		return false;
	}

	ABCHECK(nullptr != m_pSavePlayerData, false);

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	auto pWeaponTableData = pGameInstance->GetWeaponTableData(a_nWeaponIndex);
	ABCHECK(nullptr != pWeaponTableData, false);

	// 포인트 체크
	if (m_pSavePlayerData->m_nRewardPoint < pWeaponTableData->CostUnlock)
	{
		ABLOG(Warning, TEXT("Not enough reward point. (RewortPoint : %d / Cost : %d)"), m_pSavePlayerData->m_nRewardPoint, pWeaponTableData->CostUnlock);
		m_kLastBuyErrorMsg = TEXT("Not enough reward point.");
		return false;
	}

	auto WeaponOwnData = m_pSavePlayerData->WeaponOwnData.Find(a_nWeaponIndex);
	if (nullptr == WeaponOwnData)
	{
		FPGWeaponOwnData kDefalutWeaponOwnData;
		kDefalutWeaponOwnData.IsUnlock = true;
		kDefalutWeaponOwnData.MagLevel = 1;
		kDefalutWeaponOwnData.PowerLevel = 1;
		m_pSavePlayerData->WeaponOwnData.Add(a_nWeaponIndex, kDefalutWeaponOwnData);
	}
	else
	{
		WeaponOwnData->IsUnlock = true;
	}

	auto pMyPlayerState = Cast<APG_MyPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);
	ABCHECK(nullptr != pMyPlayerState, false);

	if (false == SavePlayerData())
	{
		m_kLastBuyErrorMsg = TEXT("Data Save Error!");
		ABCHECK(false, false);
	}

	m_pSavePlayerData->m_nRewardPoint -= pWeaponTableData->CostUnlock;

	if (false == pMyPlayerState->InitPlayerData())
	{
		m_kLastBuyErrorMsg = TEXT("InitPlayerData Error");
		ABCHECK(false, false);
	}

	return true;
}

bool APG_GameModeBase::BuyPowerUp(int32 a_nWeaponIndex)
{
	m_kLastBuyErrorMsg.Empty();

	if (false == IsUnlockWeapon(a_nWeaponIndex))
	{
		ABLOG(Warning, TEXT("You have not purchased a weapon. (WeaponIndex : %d)"), a_nWeaponIndex);
		m_kLastBuyErrorMsg = TEXT("You have not purchased a weapon.");
		return false;
	}

	ABCHECK(nullptr != m_pSavePlayerData, false);

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	auto pWeaponTableData = pGameInstance->GetWeaponTableData(a_nWeaponIndex);
	ABCHECK(nullptr != pWeaponTableData, false);

	// 포인트 체크
	if (false == IsAbleBuyRewardPointPower(a_nWeaponIndex))
	{
		ABLOG(Warning, TEXT("Not enough reward point. (RewortPoint : %d / Cost : %d)"), m_pSavePlayerData->m_nRewardPoint, pWeaponTableData->CostUpgradePower);
		m_kLastBuyErrorMsg = TEXT("Not enough reward point.");
		return false;
	}

	// Max Level 체크
	if (IsMaxLevelPower(a_nWeaponIndex))
	{
		m_kLastBuyErrorMsg = TEXT("Max Level");
		return false;
	}

	auto pMyPlayerState = Cast<APG_MyPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);
	ABCHECK(nullptr != pMyPlayerState, false);

	m_pSavePlayerData->m_nRewardPoint -= pWeaponTableData->CostUpgradePower;
	m_pSavePlayerData->WeaponOwnData[a_nWeaponIndex].PowerLevel += 1;

	if (false == SavePlayerData())
	{
		m_kLastBuyErrorMsg = TEXT("Data Save Error!");
		ABCHECK(false, false);
	}

	if (false == pMyPlayerState->InitPlayerData())
	{
		m_kLastBuyErrorMsg = TEXT("InitPlayerData Error");
		ABCHECK(false, false);
	}

	return true;
}

bool APG_GameModeBase::BuyAmmoUp(int32 a_nWeaponIndex)
{
	m_kLastBuyErrorMsg.Empty();

	if (false == IsUnlockWeapon(a_nWeaponIndex))
	{
		ABLOG(Warning, TEXT("You have not purchased a weapon. (WeaponIndex : %d)"), a_nWeaponIndex);
		m_kLastBuyErrorMsg = TEXT("You have not purchased a weapon.");
		return false;
	}

	ABCHECK(nullptr != m_pSavePlayerData, false);

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, false);

	auto pWeaponTableData = pGameInstance->GetWeaponTableData(a_nWeaponIndex);
	ABCHECK(nullptr != pWeaponTableData, false);

	// 포인트 체크
	if (false == IsAbleBuyRewardPointAmmo(a_nWeaponIndex))
	{
		ABLOG(Warning, TEXT("Not enough money. (RewortPoint : %d / Cost : %d)"), m_pSavePlayerData->m_nRewardPoint, pWeaponTableData->CostUpgradeAmmo);
		m_kLastBuyErrorMsg = TEXT("Not enough reward point.");
		return false;
	}

	// Max Level 체크
	if (IsMaxLevelAmmo(a_nWeaponIndex))
	{
		m_kLastBuyErrorMsg = TEXT("Max Level");
		return false;
	}

	auto pMyPlayerState = Cast<APG_MyPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);
	ABCHECK(nullptr != pMyPlayerState, false);

	m_pSavePlayerData->m_nRewardPoint -= pWeaponTableData->CostUpgradeAmmo;
	m_pSavePlayerData->WeaponOwnData[a_nWeaponIndex].MagLevel += 1;

	if (false == SavePlayerData())
	{
		m_kLastBuyErrorMsg = TEXT("Data Save Error!");
		ABCHECK(false, false);
	}

	if (false == pMyPlayerState->InitPlayerData())
	{
		m_kLastBuyErrorMsg = TEXT("InitPlayerData Error");
		ABCHECK(false, false);
	}

	return true;
}

FString APG_GameModeBase::GetLastBuyErrorMsg()
{
	return m_kLastBuyErrorMsg;
}

int32 APG_GameModeBase::GetMaxPower(int32 a_nWeaponIndex)
{
	ABCHECK(nullptr != m_pSavePlayerData, 0);

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, 0);

	auto pWeaponTableData = pGameInstance->GetWeaponTableData(a_nWeaponIndex);
	if (nullptr == pWeaponTableData)
	{
		ABLOG(Warning, TEXT("%d"), a_nWeaponIndex);
	}
	ABCHECK(nullptr != pWeaponTableData, 0);

	auto pWeaponOwnData = m_pSavePlayerData->WeaponOwnData.Find(a_nWeaponIndex);
	ABCHECK(nullptr != pWeaponOwnData, 0);

	return pWeaponTableData->WeaponData.BPower + ((pWeaponOwnData->PowerLevel - 1) * pWeaponTableData->WeaponData.PowerUpPerLevelUp);
}

int32 APG_GameModeBase::GetMaxAmmo(int32 a_nWeaponIndex)
{
	ABCHECK(nullptr != m_pSavePlayerData, 0);

	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance, 0);

	auto pWeaponTableData = pGameInstance->GetWeaponTableData(a_nWeaponIndex);
	ABCHECK(nullptr != pWeaponTableData, 0);

	if (0 > pWeaponTableData->WeaponData.Mag)
	{
		// 총알 무한 발사의 의미
		return PG_INFINITY_AMMO;
	}

	auto pWeaponOwnData = m_pSavePlayerData->WeaponOwnData.Find(a_nWeaponIndex);
	ABCHECK(nullptr != pWeaponOwnData, 0);

	return pWeaponTableData->WeaponData.Mag + ((pWeaponOwnData->MagLevel - 1) * pWeaponTableData->WeaponData.MagUpPerLevelUp);
}

UPG_SavePlayerData*	APG_GameModeBase::LoadSavePlayerData()
{
	auto pSavePlayerData = Cast<UPG_SavePlayerData>(UGameplayStatics::LoadGameFromSlot(UPG_SavePlayerData::GetSlotName(), UPG_SavePlayerData::GetPlayerIndex()));
	if (nullptr == pSavePlayerData)
		pSavePlayerData = Cast<UPG_SavePlayerData>(UGameplayStatics::CreateSaveGameObject(UPG_SavePlayerData::StaticClass()));

	return pSavePlayerData;
}

bool APG_GameModeBase::SavePlayerData()
{
	ABCHECK(nullptr != m_pSavePlayerData, false);

	if (false == UGameplayStatics::SaveGameToSlot(m_pSavePlayerData, UPG_SavePlayerData::GetSlotName(), UPG_SavePlayerData::GetPlayerIndex()))
	{
		ABLOG(Error, TEXT("Data Save Error!"));
		return false;
	}

	return true;
}

UPG_SaveStageData*	APG_GameModeBase::LoadSaveStageData(int32 nStageID)
{
	auto pSaveStageData = Cast<UPG_SaveStageData>(UGameplayStatics::LoadGameFromSlot(UPG_SaveStageData::GetSlotName(nStageID), UPG_SaveStageData::GetPlayerIndex()));
	if (nullptr == pSaveStageData)
	{
		pSaveStageData = Cast<UPG_SaveStageData>(UGameplayStatics::CreateSaveGameObject(UPG_SaveStageData::StaticClass()));
		pSaveStageData->m_nStageID = nStageID;
	}

	return pSaveStageData;
}


bool APG_GameModeBase::SaveStageData()
{
	ABCHECK(nullptr != m_pSelectSaveStageData, false);

	if (false == UGameplayStatics::SaveGameToSlot(m_pSelectSaveStageData, UPG_SaveStageData::GetSlotName(m_pSelectSaveStageData->m_nStageID), UPG_SaveStageData::GetPlayerIndex()))
	{
		ABLOG(Error, TEXT("Data Save Error!"));
		return false;
	}

	return true;
}

void APG_GameModeBase::UpdateGameInstance(float DeltaTime)
{
	// 게임 플레이 스테이지 모드에서만 처리할 수 있는 콘솔 커맨드
	auto pGameInstance = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	pGameInstance->FromGameModeTick(DeltaTime);
}