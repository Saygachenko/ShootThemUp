// Shoo Them Up Game

using UnrealBuildTool;
using System.Collections.Generic;

public class ShootThemUPTarget : TargetRules
{
	public ShootThemUPTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "ShootThemUP" } );
	}
}
