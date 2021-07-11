// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AD/PG_ADData.h"
#include "Game/GameInstance/PG_GameInstance.h"
#include "Game/GameMode/PG_GameModeStage.h"
#include "Game/GameMode/PG_GameModeMainLobby.h"

PG_ADData::PG_ADData()
{
}

PG_ADData::~PG_ADData()
{
}

void PG_ADData::Init(UPG_GameInstance* pGameInstance)
{
	ABCHECK(nullptr != pGameInstance);
	m_kRemainReViewableTime.Init(0.0f, (int32)EADType::eAT_Max);

	for (int32 nIndex = 0; nIndex < m_kRemainReViewableTime.Num(); ++nIndex)
	{
		auto pADTableData = pGameInstance->GetADTableData((EADType)nIndex);
		if (nullptr == pADTableData)
		{
			ABLOG(Warning, TEXT("Can't Find ADTable Data [Index : %d]"), nIndex);
			continue;
		}

		m_kRemainReViewableTime[nIndex] = (float)pADTableData->ReViewableTime;
	}
}

void PG_ADData::Update(float DeltaTime)
{
	ABCHECK(nullptr != G_PGWorld);

	auto pGameInstance = Cast<UPG_GameInstance>(G_PGWorld->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	for (int32 nIndex = 0; nIndex < m_kRemainReViewableTime.Num(); ++nIndex)
	{
		auto pADTableData = pGameInstance->GetADTableData((EADType)nIndex);
		if (nullptr == pADTableData)
			continue;

		if (0.0f < m_kRemainReViewableTime[nIndex])
		{
			// MainLobby
			if (pADTableData->TimeDeduction_MainLobby && Cast<APG_GameModeMainLobby>(G_PGWorld->GetAuthGameMode()))
			{
				m_kRemainReViewableTime[nIndex] -= DeltaTime;
				if (0.0f > m_kRemainReViewableTime[nIndex])
					m_kRemainReViewableTime[nIndex] = 0.0f;

				continue;
			}

			// GameModeStage
			if (pADTableData->TimeDeduction_StagePlay && Cast<APG_GameModeStage>(G_PGWorld->GetAuthGameMode()))
			{
				m_kRemainReViewableTime[nIndex] -= DeltaTime;
				if (0.0f > m_kRemainReViewableTime[nIndex])
					m_kRemainReViewableTime[nIndex] = 0.0f;

				continue;
			}
		}
	}
}

void PG_ADData::Reset(EADType ekADType)
{
	ABCHECK(nullptr != G_PGWorld);

	auto pGameInstance = Cast<UPG_GameInstance>(G_PGWorld->GetGameInstance());
	ABCHECK(nullptr != pGameInstance);

	auto pADTableData = pGameInstance->GetADTableData(ekADType);
	ABCHECK(nullptr != pADTableData);

	m_kRemainReViewableTime[(int32)ekADType] = (float)pADTableData->ReViewableTime;
}

bool PG_ADData::IsAbleViewAD(EADType ekADType)
{
	ABCHECK(ekADType < EADType::eAT_Max, false);
	return (0.0f >= m_kRemainReViewableTime[(int32)ekADType]);
}

int32 PG_ADData::GetRemainReViewableTime(EADType ekADType)
{
	ABCHECK(ekADType < EADType::eAT_Max, 0);
	return (int32)m_kRemainReViewableTime[(int32)ekADType];
}