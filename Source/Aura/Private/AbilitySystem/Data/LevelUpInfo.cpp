// Copyrights Skorokhod.


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP) const
{
	int32 level = 1;

	while (true)
	{
		// LevelUpInformation[1] = level 1 information
		// LevelUpInformation[2] = level 1 information
		if (LevelUpInformation.Num() - 1 <= level) return level;

		if(XP >= LevelUpInformation[level].LevelUpRequirement)
		{
			++level;
		}
		else
		{
			break;
		}

	}
	return level;
}
