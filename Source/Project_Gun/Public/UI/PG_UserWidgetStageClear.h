// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <Blueprint/UserWidget.h>
#include "PG_UserWidgetStageClear.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetStageClear : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

public :
	// �������� �÷��� ���� ��� �������� �˻�
	UFUNCTION(BlueprintCallable)
	bool IsAbleViewStagePlayAD();

	UFUNCTION(BlueprintCallable)
	void UpdateClearStageLayout();

public :
	UFUNCTION(BlueprintCallable)
	void OnBtnRetry();
	UFUNCTION(BlueprintCallable)
	void OnBtnGoToMainLobby();
	UFUNCTION(BlueprintCallable)
	void OnBtnNextStage();
	UFUNCTION(BlueprintCallable)
	void OnBtnViewAD();

	//============================================================================================
	// ���� ��û�� ���� ���
	//============================================================================================
	UFUNCTION(BlueprintCallable)
	void OnADRewardVideoSucessed();		// ������ ������ ���� ���������� ������ (3�� ����)
	UFUNCTION(BlueprintCallable)	
	void OnADRewardVideoClosed();		// ������ ������ ���� ������ �ݰų� �����Ͽ���	(3�� ����)
	UFUNCTION(BlueprintCallable)
	void OnADPlayStageVideoSucessed();	// �������� �÷����� ���� ���������� ������
	UFUNCTION(BlueprintCallable)
	void OnADPlayStageVideoClosed();	// �������� �÷����� ���� �ݰų� �����Ͽ���
	//============================================================================================

	UFUNCTION()
	void OnViewResultAD(bool bEnable);

private:
	void SetBonusFlag(bool bWasClear, int32 nBonus);
	void SetBonusLethal(bool bWasClear, int32 nBonus);
	void SetBonusTime(bool bWasClear, int32 nBonus);
	void SetBonusStageClear(int32 nStageClearBonus);
	void SetBonusMonsterKill(int32 nMonsterKillBonus);
	void SetBonusStar(int32 nStarBonus);
	void SetBonusAD(bool bEnable);
	void SetBtnBonusADTime(int32 nTime);
	void SetBtnEnableAD(bool bEnable);
	void SetTotalReward(int32 nTotalReward);

private :
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

	UPROPERTY()
	class UTextBlock* Text_BonusFlag;
	UPROPERTY()
	class UTextBlock* Text_BonusLethal;
	UPROPERTY()
	class UTextBlock* Text_BonusTime;
	
	UPROPERTY()
	class UTextBlock* Text_BonusStageClear;
	UPROPERTY()
	class UTextBlock* Text_BonusKill;
	UPROPERTY()
	class UTextBlock* Text_BonusStar;
	UPROPERTY()
	class UTextBlock* Text_CaptionBonusAD;
	UPROPERTY()
	class UTextBlock* Text_BonusAD;
	UPROPERTY()
	class UTextBlock* Text_ClearADTimer;
	UPROPERTY()
	class UTextBlock* Text_TotalReward;

	UPROPERTY()
	class UButton* m_pBtn_ViewAD;

	bool m_bWasViewStageClear3X = false;
};
