// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"

/**
 * 
 */
class PG_Command
{
public:
	PG_Command();
	virtual ~PG_Command();

public :
	virtual void Init(class IConsoleVariable* pConsoleVariable);
	virtual void UpdateCommand() = 0;		// 매 틱마다 커맨드가 업데이트 되었는지 확인한다.
	virtual void Excute() = 0;				// 커맨드 실행
	virtual void OnChangeCommand() = 0;		// 해당 커맨드가 입력이 되었음을 알린다.

	class IConsoleVariable* GetConsoleVariable() { return m_pConsoleVariable; }

public:
	static void OnUpdateConsoleValue(IConsoleVariable* pConsoleVariable);
	
protected :
	class IConsoleVariable*		m_pConsoleVariable = nullptr;

private :
	FConsoleVariableDelegate	OnConsoleVariableDelegate;
};
