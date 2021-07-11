#include "UI/PG_VirtualJoystick.h"
#include "Player/PlayerController/PG_PlayerController.h"


void SPG_VirtualJoystick::Construct(const FArguments& InArgs)
{
	State = State_Inactive;
	bVisible = true;
	bPreventReCenter = false;

	// just set some defaults
	ActiveOpacity = 1.0f;
	InactiveOpacity = 0.1f; 
	TimeUntilDeactive = 0.5f;
	TimeUntilReset = 2.0f;
	ActivationDelay = 0.f;
	CurrentOpacity = InactiveOpacity;
	StartupDelay = 0.f;  

	// listen for displaymetrics changes to reposition controls
	FSlateApplication::Get().GetPlatformApplication()->OnDisplayMetricsChanged().AddSP(this, &SPG_VirtualJoystick::HandleDisplayMetricsChanged);
}

FReply SPG_VirtualJoystick::OnTouchStarted(const FGeometry& MyGeometry, const FPointerEvent& Event)
{
	return SVirtualJoystick::OnTouchStarted(MyGeometry, Event);
}

FReply SPG_VirtualJoystick::OnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& Event)
{
	return SVirtualJoystick::OnTouchMoved(MyGeometry, Event);
}

FReply SPG_VirtualJoystick::OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& Event)
{
	if (MyPlayerController) 
		MyPlayerController->OnTryLethalAttack();

	return SVirtualJoystick::OnTouchEnded(MyGeometry, Event);
}

void SPG_VirtualJoystick::BindPlayerController(class APG_PlayerController* NewPlayerController)
{
	ABCHECK(nullptr != NewPlayerController);
	MyPlayerController = NewPlayerController;
}