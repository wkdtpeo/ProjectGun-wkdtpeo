#pragma once

#include "Project_Gun/Project_Gun.h"

#include <Widgets/Input/SVirtualJoystick.h>

class SPG_VirtualJoystick : public SVirtualJoystick
{

public:
	SLATE_USER_ARGS(SPG_VirtualJoystick)
	{}
	SLATE_END_ARGS()

	virtual void Construct(const FArguments& InArgs);
	virtual FReply OnTouchStarted(const FGeometry& MyGeometry, const FPointerEvent& Event) override;
	virtual FReply OnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& Event) override;
	virtual FReply OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& Event) override;

	void BindPlayerController(class APG_PlayerController* NewPlayerController);

private :
	class APG_PlayerController* MyPlayerController;
};