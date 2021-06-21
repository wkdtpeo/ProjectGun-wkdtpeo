#pragma once

#include "PG_DefineDelegate.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnBtnClickedOK);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBtnClickedOK_OneValue, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBtnClickedCancel_OneValue, int32);


UENUM()
enum NoopEnum
{
	eNoopEnum,
};
