
#pragma once

#include "GameplayTagContainer.h"

#define PLAYERTAG_ACTION_ATTACK FGameplayTag::RequestGameplayTag(FName("Player.Action.Attack"))
#define PLAYERTAG_STATE_ISDEAD FGameplayTag::RequestGameplayTag(FName("Player.State.IsDead"))
#define PLAYERTAG_STATE_INVINSIBLE FGameplayTag::RequestGameplayTag(FName("Player.State.Invinsible"))
