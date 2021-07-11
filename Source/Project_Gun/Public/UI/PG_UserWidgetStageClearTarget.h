// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <Blueprint/UserWidget.h>
#include "PG_UserWidgetStageClearTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetStageClearTarget : public UUserWidget
{
	GENERATED_BODY()
	
public :
	void SetDataFlag(bool bClear, int32 nCurrValue, int32 nMaxValue);
	void SetDataLethal(bool bClear, int32 nCurrValue, int32 nMaxValue);
	void SetDataTime(bool bClear, int32 nCurrTime);
	
protected:
	virtual void NativeConstruct() override;
	
	//==============================================
	// Flag
	//==============================================
	UPROPERTY(BlueprintReadWrite)
	class UOverlay* Flag_Clear;
	UPROPERTY(BlueprintReadWrite)
	class UOverlay* Flag_InProgress;
	UPROPERTY(BlueprintReadWrite)
	class UTextBlock* Text_CurrFlag;
	UPROPERTY(BlueprintReadWrite)
	class UTextBlock* Text_MaxFlag;

	//==============================================
	// Lethal
	//==============================================
	UPROPERTY(BlueprintReadWrite)
	class UOverlay* Lethal_Clear;
	UPROPERTY(BlueprintReadWrite)
	class UOverlay* Lethal_InProgress;
	UPROPERTY(BlueprintReadWrite)
	class UTextBlock* Text_CurrLethal;
	UPROPERTY(BlueprintReadWrite)
	class UTextBlock* Text_MaxLethal;

	//==============================================
	// Time
	//==============================================
	UPROPERTY(BlueprintReadWrite)
	class UOverlay* Time_Clear;
	UPROPERTY(BlueprintReadWrite)
	class UOverlay* Time_InProgress;
	UPROPERTY(BlueprintReadWrite)
	class UTextBlock* Text_CurrTime;
};
