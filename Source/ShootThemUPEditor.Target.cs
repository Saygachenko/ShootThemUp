// Shoo Them Up Game

using UnrealBuildTool;
using System.Collections.Generic;

public class ShootThemUPEditorTarget : TargetRules
{
	public ShootThemUPEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "ShootThemUP" } );
	}
}
