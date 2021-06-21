// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINIT,	// ĳ���� ���� ���� ������Ʈ
	LOADING,	// ������ ĳ���� �ּ��� �ε��ϴ� ������Ʈ
	READY,		// ĳ���� �ּ� �ε��� �Ϸ�� ������Ʈ
	DEAD		// ĳ���� ��� ������Ʈ
};