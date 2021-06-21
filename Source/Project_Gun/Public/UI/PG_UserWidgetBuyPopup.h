// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include <Blueprint/UserWidget.h>
#include "PG_UserWidgetBuyPopup.generated.h"


//DECLARE_MULTICAST_DELEGATE_OneParam(FOnBtnClickedOK_OneValue, int32);
//DECLARE_MULTICAST_DELEGATE_OneParam(FOnBtnClickedCancel_OneValue, int32);
/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetBuyPopup : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

public :
	UFUNCTION(BlueprintCallable)
	void SetTitle(FString kTitle);
	UFUNCTION(BlueprintCallable)
	void SetPrice(int32 nPrice);
	UFUNCTION(BlueprintCallable)
	void SetDescription(FString kDescription);
	UFUNCTION(BlueprintCallable)
	void SetValue(int32 nValue);

	FOnBtnClickedOK_OneValue			OnBtnClickedOK;
	FOnBtnClickedCancel_OneValue		OnOnBtnClickedCancel;

protected:
	UFUNCTION()
	void OnBtnOK();
	UFUNCTION()
	void OnBtnCancel();

protected :
	UPROPERTY(BlueprintReadWrite)
	class UTextBlock* Text_TitleName;
	UPROPERTY(BlueprintReadWrite)
	class UTextBlock* Text_Price;
	UPROPERTY(BlueprintReadWrite)
	class UTextBlock* Text_Description;
	UPROPERTY(BlueprintReadWrite)
	class UButton* Btn_OK;
	UPROPERTY(BlueprintReadWrite)
	class UButton* Btn_Cancel;

	UPROPERTY(EditAnywhere, Category = "PG Sound")
	class USoundBase* PopupPlaySound = nullptr;

	int32 m_nValue = PG_INVALID_INDEX;
};
