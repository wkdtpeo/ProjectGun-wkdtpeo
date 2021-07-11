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
		eMaxWeaponSlotIndex = PG_MAX_WEAPON_SLOT,	// 최대 장착 가능한 무기의 수
	};
	
protected:
	virtual void NativeConstruct() override;
	
public :
	void InitWeaponBoxData();
	bool InitWeaponBoxData(bool bEnable, int32 nSlotIndex, int32 nWeaponTableIndex, int32 nAmmo);

	// 화면에 현재 착용중인 무기를 표시한다.
	void SetSelectWeapon(int32 nSlotIndex);

public :
	//=====================================================
	// Reward Point
	//=====================================================
	// 포인트 획득 업데이트
	UFUNCTION(BlueprintImplementableEvent, Category = "RewardPoint")
	void OnChangeRewardPoint();

	//=====================================================
	// 스테이지 클리어 목표 현황
	//=====================================================
	// 현재 몬스터 킬수 업데이트
	UFUNCTION(BlueprintCallable)
	void OnUpdateFlag();
	// 몬스터 필살기 킬수 업데이트
	UFUNCTION(BlueprintCallable)
	void OnUpdateLethal();
	// 타이머 업데이트
	UFUNCTION(BlueprintCallable)
	void OnUpdateTime();

	//=====================================================
	// 무기 슬롯
	//=====================================================
	UFUNCTION(BlueprintCallable)
	void OnUpdateWeaponAmmo(int32 nSlotIndex, int32 nAmmo);
	// 해당 SlotIndex의 무기를 변경 요청하였다.
	UFUNCTION(BlueprintCallable)
	void OnChangeEquipWeaponIndex(int32 nSlotIndex);

private :
	class UPG_UserWidgetWeaponBox* GetUserWidgetWeaponBox(int32 nSlotIndex);
	class UOverlay* GetOverlaySelectWeapon(int32 nSlotIndex);

protected:
	// 스테이지 클리어 목표 현황
	UPROPERTY(BlueprintReadWrite)
	class UPG_UserWidgetStageClearTarget* WB_StageClearTarget;

	// 무기 슬롯
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
