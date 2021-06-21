// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_Gun/Project_Gun.h"
#include "PG_Command.h"

/**
 * 
 */
class PG_CommandRestartStage : public PG_Command
{
public:
	PG_CommandRestartStage();
	~PG_CommandRestartStage();

public:
	virtual void UpdateCommand() override;
	virtual void Excute() override;
	virtual void OnChangeCommand() override;

private:
	bool bOnCommand = false;
};
