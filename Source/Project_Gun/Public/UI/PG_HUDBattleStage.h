// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <Blueprint/UserWidget.h>
#include "PG_HUDBattleStage.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_HUDBattleStage : public UUserWidget
{
	GENERATED_BODY()
	
protected :
	virtual void NativeConstruct() override;
	//virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent);

public :
	void BindPlayerState(class APG_MyPlayerState* APlayerState);

	void UpdateStageTimer();
	void UpdateRewardPoint();
	void UpdateLethalAttack();

public:
	UFUNCTION()
	void OnBtnRifle();
	UFUNCTION()
	void OnBtnFlame();
	UFUNCTION()
	void OnBtnShutgun();
	UFUNCTION()
	void OnBtnLaser();
	UFUNCTION()
	void OnBtnGrandle();

private :
	UPROPERTY()
	class UTextBlock* StageTimer;
	
	UPROPERTY()
	class UTextBlock* RewardPoint;

	UPROPERTY()
	class UTextBlock* LethalAttack;

	UPROPERTY()
	class UButton* Btn_Rifle;

	UPROPERTY()
	class UButton* Btn_Flame;

	UPROPERTY()
	class UButton* Btn_Shutgun;

	UPROPERTY()
	class UButton* Btn_Laser;

	UPROPERTY()
	class UButton* Btn_Grandle;
};
