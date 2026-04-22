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
	
	FAIData():ActionType(EActionType::Stay),TileToReach(FTileData()),ActorToAttack(nullptr)
	{
		
	}
	
	EActionType ActionType;
	FTileData TileToReach;
	AActor* ActorToAttack;
};