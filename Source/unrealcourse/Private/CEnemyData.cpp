#include "CEnemyData.h"

FPrimaryAssetId UCEnemyData::GetPrimaryAssetId() const
{
	// #weirdunrealenginestuff: This'll always need to be matched against the following: Editor/Project Settings/Game/Asset Manager/Primary Asset Types To Scan/[relevant item]
	return FPrimaryAssetId("EnemyData", GetFName());
}
