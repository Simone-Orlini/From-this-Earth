#pragma once
#include "CoreMinimal.h"
#include "TileData.h"
#include "AIdata.generated.h"

UENUM(BlueprintType)
enum class EActionType : uint8
{
	Stay,
	Attack,
	Move,
	AttackToMove,
	MoveToAttack,
};

USTRUCT(BlueprintType)
struct FAIData
{
	GENERATED_BODY();
	
	FAIData():ActionType(EActionType::Stay),TileToReach(FIntPoint(-1, -1)),ActorToAttack(nullptr)
	{
		
	}
	
	EActionType ActionType;
	FIntPoint TileToReach;
	AActor* ActorToAttack;
};