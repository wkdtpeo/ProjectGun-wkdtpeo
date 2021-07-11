// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveData/PG_SaveStageData.h"

UPG_SaveStageData::UPG_SaveStageData()
{

}

FString UPG_SaveStageData::GetSlotName(int32 a_nStageID)
{
	return FString::Printf(TEXT("SaveStageData_%d"), a_nStageID);
}

int32 UPG_SaveStageData::GetPlayerIndex()
{
	return 0;
}