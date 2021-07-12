// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINIT,	// 캐릭터 생선 전의 스테이트
	LOADING,	// 선택한 캐릭터 애셋을 로딩하는 스테이트
	READY,		// 캐릭터 애셋 로딩이 완료된 스테이트
	DEAD		// 캐릭터 사망 스테이트
};