#pragma once

#include "CoreMinimal.h"
#include "CBaseAction.h"
#include "CActionAbility.generated.h"

/**
 * @class UCActionAbility
 *
 * @brief Represents an ability that an actor can perform.
 *
 * The UCActionAbility class is a subclass of UCBaseAction. It's almost the same as the UCBaseAction with that exception that is has been explicitly specificed that it does not auto start.
 */
UCLASS(Blueprintable, Abstract)
class UNREALCOURSE_API UCActionAbility : public UCBaseAction
{
	GENERATED_BODY()
	
public:
	UCActionAbility();
};
