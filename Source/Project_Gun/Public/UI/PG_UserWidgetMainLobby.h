// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <Blueprint/UserWidget.h>
#include "PG_UserWidgetMainLobby.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetMainLobby : public UUserWidget
{
	GENERATED_BODY()
	

protected :
	virtual void NativeConstruct() override;

public :
	UFUNCTION(BlueprintCallable)
	FString GetSelectedStageName();

	// �������� �� ȹ�� UI ����
	UFUNCTION(BlueprintCallable)
	void UpdateClearStageLayout();

	// ���� ���� ����
	UFUNCTION(BlueprintCallable)
	void UpdateWeaponSlotLayout();

	// �κ��丮 ���� ���� ������ �̹���
	UFUNCTION(BlueprintCallable)
	void AllHideIconOpenSlot();
	UFUNCTION(BlueprintCallable)
	void SetEnableIconOpenSlot(int32 nSlotIndex, bool bEnable);

public :
	UFUNCTION(BlueprintCallable)
	bool OnNextSelectStage();
	UFUNCTION(BlueprintCallable)
	bool OnPrevSelectStage();

	UFUNCTION()
	void OnBtnClickedWeaponSlot(int32 nSlotIndex);
	UFUNCTION()
	void OnCloseWeaponInventory(int32 nSlotIndex, int32 nWeaponIndex);

private :
	UPROPERTY()
	class UOverlay* Overlay_ActiveFlag;
	UPROPERTY()
	class UOverlay* Overlay_DeActiveFlag;
	UPROPERTY()
	class UTextBlock* Text_FlagValue;
	

	UPROPERTY()
	class UOverlay* Overlay_ActiveTime;
	UPROPERTY()
	class UOverlay* Overlay_DeActiveTime;
	UPROPERTY()
	class UTextBlock* Text_TimeValue;

	UPROPERTY()
	class UOverlay* Overlay_ActiveLethal;
	UPROPERTY()
	class UOverlay* Overlay_DeActiveLethal;
	UPROPERTY()
	class UTextBlock* Text_LethalValue;


	UPROPERTY()
	class UPG_UserWidgetWeaponInventory* WB_WeaponInventory;
};
