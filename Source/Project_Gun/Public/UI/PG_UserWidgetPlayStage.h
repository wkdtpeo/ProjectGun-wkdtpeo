// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <Blueprint/UserWidget.h>
#include "PG_UserWidgetPlayStage.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetPlayStage : public UUserWidget
{
	GENERATED_BODY()
	
	enum
	{
		eMaxWeaponSlotIndex = PG_MAX_WEAPON_SLOT,	// �ִ� ���� ������ ������ ��
	};
	
protected:
	virtual void NativeConstruct() override;
	
public :
	void InitWeaponBoxData();
	bool InitWeaponBoxData(bool bEnable, int32 nSlotIndex, int32 nWeaponTableIndex, int32 nAmmo);

	// ȭ�鿡 ���� �������� ���⸦ ǥ���Ѵ�.
	void SetSelectWeapon(int32 nSlotIndex);

public :
	//=====================================================
	// Reward Point
	//=====================================================
	// ����Ʈ ȹ�� ������Ʈ
	UFUNCTION(BlueprintImplementableEvent, Category = "RewardPoint")
	void OnChangeRewardPoint();

	//=====================================================
	// �������� Ŭ���� ��ǥ ��Ȳ
	//=====================================================
	// ���� ���� ų�� ������Ʈ
	UFUNCTION(BlueprintCallable)
	void OnUpdateFlag();
	// ���� �ʻ�� ų�� ������Ʈ
	UFUNCTION(BlueprintCallable)
	void OnUpdateLethal();
	// Ÿ�̸� ������Ʈ
	UFUNCTION(BlueprintCallable)
	void OnUpdateTime();

	//=====================================================
	// ���� ����
	//=====================================================
	UFUNCTION(BlueprintCallable)
	void OnUpdateWeaponAmmo(int32 nSlotIndex, int32 nAmmo);
	// �ش� SlotIndex�� ���⸦ ���� ��û�Ͽ���.
	UFUNCTION(BlueprintCallable)
	void OnChangeEquipWeaponIndex(int32 nSlotIndex);

private :
	class UPG_UserWidgetWeaponBox* GetUserWidgetWeaponBox(int32 nSlotIndex);
	class UOverlay* GetOverlaySelectWeapon(int32 nSlotIndex);

protected:
	// �������� Ŭ���� ��ǥ ��Ȳ
	UPROPERTY(BlueprintReadWrite)
	class UPG_UserWidgetStageClearTarget* WB_StageClearTarget;

	// ���� ����
	UPROPERTY(BlueprintReadWrite)
	class UPG_UserWidgetWeaponBox* WB_WeaponBox_0;
	UPROPERTY(BlueprintReadWrite)
	class UPG_UserWidgetWeaponBox* WB_WeaponBox_1;
	UPROPERTY(BlueprintReadWrite)
	class UPG_UserWidgetWeaponBox* WB_WeaponBox_2;

	UPROPERTY(BlueprintReadWrite)
	class UOverlay* SelectWeapon_0;
	UPROPERTY(BlueprintReadWrite)
	class UOverlay* SelectWeapon_1;
	UPROPERTY(BlueprintReadWrite)
	class UOverlay* SelectWeapon_2;
};
