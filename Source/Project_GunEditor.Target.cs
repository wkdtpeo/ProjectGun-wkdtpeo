// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Project_GunEditorTarget : TargetRules
{
	public Project_GunEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		DefaultBuildSettings = BuildSettingsVersion.V2;
		//ShadowVariableWarningLevel = WarningLevel.Warning;

		ExtraModuleNames.AddRange( new string[] { "Project_Gun" } );
	}
}
