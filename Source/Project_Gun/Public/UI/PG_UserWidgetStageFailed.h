// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <Blueprint/UserWidget.h>
#include "PG_UserWidgetStageFailed.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetStageFailed : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public :
	UFUNCTION(BlueprintCallable)
	bool IsAbleViewStagePlayAD();

	UFUNCTION(BlueprintCallable)
	void UpdateClearStageLayout();
	
public:
	UFUNCTION(BlueprintCallable)
	void OnBtnRetry();
	UFUNCTION(BlueprintCallable)
	void OnBtnGoToMainLobby();

	//============================================================================================
	// 광고 시청에 대한 결과
	//============================================================================================
	UFUNCTION(BlueprintCallable)
	void OnADPlayStageVideoSucessed();	// 스테이지 플레이형 광고를 성공적으로 보았음
	UFUNCTION(BlueprintCallable)
	void OnADPlayStageVideoClosed();	// 스테이지 플레이형 광고를 닫거나 실패하였음
	//============================================================================================

private:
	UPROPERTY()
	class UOverlay* Overlay_ActiveFlag;
	UPROPERTY()
	class UOverlay* Overlay_DeActiveFlag;

	UPROPERTY()
	class UOverlay* Overlay_ActiveTime;
	UPROPERTY()
	class UOverlay* Overlay_DeActiveTime;

	UPROPERTY()
	class UOverlay* Overlay_ActiveLethal;
	UPROPERTY()
	class UOverlay* Overlay_DeActiveLethal;
};
