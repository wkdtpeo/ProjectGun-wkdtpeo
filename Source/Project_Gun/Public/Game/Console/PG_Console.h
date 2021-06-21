// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"

/**
 * 
 */
class PROJECT_GUN_API PG_Console
{
public :
	//****************************************************************************************
	// 커맨드 추가시 PG_Command를 상속받은 클래스 제작한 후
	// PG_Console::CreateCommands() 함수 내에 ADD_COMMAND 매크로를 이용하여
	// 커맨드를 추가한다.
	//****************************************************************************************
	enum ECosloeCommands : int32
	{
		eCC_SET_TARGET_STAGE_ID = 0,		// 현재 플레이하는 레벨에서 스테이지 ID값으로 변경한다. (변경 후 스테이지 클리어, 또는 다시 시작시 변경된 스테이지값으로 시작한다.)
		eCC_RESTART_STAGE,					// 현재 스테이지를 다시 시작한다.
		eCC_NEXT_STAGE,						// 다음 스테이지를 시작한다.
		eCC_PREV_STAGE,						// 이전 스테이지를 시작한다.
		eCC_START_STAGE,					// 입력된 스테이지를 시작한다.
		eCC_SHOW_ME_THE_MONEY,				// 입력한 값으로 Point를 설정한다.
		eCC_ALL_STAGE_UNLOCK,				// 모든 스테이지를 Unlock 한다.
		eCC_Max,
	};

public:
	PG_Console();
	~PG_Console();

public :
	bool InitConsoleCommands();
	void ReleaseCommands();

	void UpdateCommands();

	class PG_Command* GetCommand(class IConsoleVariable* pConsoleVariable);

private :
	void CreateCommands();

private :
	UPROPERTY()
	TArray<class PG_Command*> m_kCommands;
};
