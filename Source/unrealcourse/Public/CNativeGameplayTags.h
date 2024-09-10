#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "CNativeGameplayTags.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Attacking);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Attacking_PrimaryAttacking);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Attacking_SpecialAttacking);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Attacking_TeleportAttacking);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Parrying);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Sprinting);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Keycard_Blue);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Keycard_Red);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Keycard_Yellow);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Burning);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Parrying);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Stunned);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Thorns);

/**  Declares a set of GameplayTags to be used both in the editor and in C++ code. */
UCLASS(Hidden, Const)
class UNREALCOURSE_API UCNativeGameplayTags : public UObject
{
	GENERATED_BODY()
};
