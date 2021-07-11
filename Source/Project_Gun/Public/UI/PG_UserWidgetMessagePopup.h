// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
//#include "PG_DefineDelegate.h"
#include <Blueprint/UserWidget.h>
#include "PG_UserWidgetMessagePopup.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUN_API UPG_UserWidgetMessagePopup : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public :
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

public:
	UFUNCTION(BlueprintCallable)
	void SetTitle(FString kTitle);
	UFUNCTION(BlueprintCallable)
	void SetDescription(FString kDescription);
	UFUNCTION(BlueprintCallable)
	void SetValue(int32 nValue);
	
public :
	FOnBtnClickedOK OnBtnClickedOK;
	FOnBtnClickedOK_OneValue OnBtnClickedOK_OneValue;

protected:
	UFUNCTION()
	void OnBtnOK();

protected :
	UPROPERTY(BlueprintReadWrite)
	class UTextBlock* Text_TitleName;
	UPROPERTY(BlueprintReadWrite)
	class UTextBlock* Text_Description;
	UPROPERTY(BlueprintReadWrite)
	class UButton* Btn_OK;

	UPROPERTY(EditAnywhere, Category = "PG Sound")
	class USoundBase* PopupPlaySound = nullptr;

	int32 m_nValue = PG_INVALID_INDEX;
};
