// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"

/**
 * 
 */
class PROJECT_GUN_API PG_ADData
{
public:
	PG_ADData();
	~PG_ADData();

public :
	void Init(class UPG_GameInstance* pGameInstance);
	void Update(float DeltaTime);

	void	Reset(EADType ekADType);
	int32	GetRemainReViewableTime(EADType ekADType);

	bool	IsAbleViewAD(EADType ekADType);

private :
	UPROPERTY()
	TArray<float> m_kRemainReViewableTime;
};
