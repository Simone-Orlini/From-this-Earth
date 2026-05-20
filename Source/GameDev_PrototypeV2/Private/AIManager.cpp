// Fill out your copyright notice in the Description page of Project Settings.


#include "AIManager.h"
#include "TileData.h"
#include "GamemodeAImanger.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

struct FTileInfo
{
	FTileInfo() : ActorToAttack(nullptr), FinalCost(-1.0f)
	{
	}

	float FinalCost;
	float distFromUnitCost;
	float tileCost;

	float distCost;
	float attackCost;

	AActor* ActorToAttack;
};

// Sets default values for this component's properties
UAIManager::UAIManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAIManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UAIManager::StartLogic(const TArray<AActor*>& Enemies, const TArray<AActor*>& Allies)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());

	tileOccupated.Empty();
	pUnitsAttacked.Empty();

	for (AActor* cEnemy : Enemies)
	{
		TArray<FTileData> tiles;
		tiles.Add(IGamemodeAImanger::Execute_GetTileData(GameMode, cEnemy));
		tiles.Append(IGamemodeAImanger::Execute_GetTilesFromBFS(GameMode, cEnemy));

		TMap<FIntPoint, FTileInfo> tilesInfo;
		bool bCanAttackWithoutMove = false;
		bool bCanAttack = false;
		int32 i = 0;

		for (FTileData& cTile : tiles)
		{
			if (i != 0 && (!cTile.bIsWalkable || cTile.IsOccupied() || 
				tileOccupated.Contains(cTile.GridTileData.Index)))
				continue;

			FTileInfo info;


			float tileCost = cTile.GetAICost() / static_cast<float>(FTileData::MaxAICost);
			// 7 is the maxCost - MAGICNUMBER BLEH
			float distCost = IGamemodeAImanger::Execute_GetPointsFromPath(GameMode, cEnemy, cTile).Num() / 4.0f;
			// 4 is the currentMovementRange - MAGICNUMBER BLEH
			// 5 should be the current moverange from the cEnemy; //AGameModeBase.Path(cEnemy, cTile);
			if (distCost < 0.001f || i == 0)
				distCost = 1.0f;

			float attackCost = 0.0f;
			float distFromUnitCost = 0.0f;

			if (!bCanAttackWithoutMove)
			{
				CalculateAttackCost(cTile, cEnemy, info, attackCost);
				if (attackCost > 0.0f)
				{
					bCanAttack = true;
				}

				if (i == 0 && attackCost > 0.0f)
					bCanAttackWithoutMove = true;
			}

			info.tileCost = tileCost;
			info.distCost = distCost;
			info.attackCost = attackCost;
			info.distFromUnitCost = distFromUnitCost;

			float finalCost = tileCost * tileWeight +
				distCost * distWeight +
				attackCost * attackWeight +
				distFromUnitCost * unitDistWeight;

			info.FinalCost = finalCost;
			tilesInfo.Add(cTile.GridTileData.Index, info);

			i++;
		}

		if (!bCanAttack)
		{
			FIntPoint point = FindShorterPath(Allies, cEnemy);
			FTileInfo* info = tilesInfo.Find(point);
			if (info)
			{
				info->FinalCost += 1.0f;
			}
		}

		TArray<FIntPoint> bestTiles;
		float valueToCompare = 0.0f;

		for (auto cKeyValue : tilesInfo)
		{
			float value = cKeyValue.Value.FinalCost;

			if (value > valueToCompare + 0.001f) // 0.001f is a threshold
			{
				valueToCompare = value;
				bestTiles.Empty();
				bestTiles.Add(cKeyValue.Key);
			}
			else if (FMath::IsNearlyEqual(value, valueToCompare, 0.001f))
			{
				bestTiles.Add(cKeyValue.Key);
			}
		}

		int32 randomIndex = FMath::RandRange(0, bestTiles.Num() - 1);
		FIntPoint rBestTile = bestTiles[randomIndex];

		FAIData data;

		AActor* unitToAttack = tilesInfo.Find(rBestTile)->ActorToAttack;

		if (rBestTile == IGamemodeAImanger::Execute_GetTileData(GameMode, cEnemy).GridTileData.Index)
		{
			if (unitToAttack)
			{
				data.ActorToAttack = unitToAttack;

				if (bestTiles.Num() > 1)
				{
					bestTiles.Remove(rBestTile);
					randomIndex = FMath::RandRange(0, bestTiles.Num() - 1);
					rBestTile = bestTiles[randomIndex];

					data.ActionType = EActionType::AttackToMove;
					data.TileToReach = rBestTile;
					tileOccupated.Add(rBestTile);
				}
				else
				{
					// Sarebbe da rivalutare gli altri tile che hanno un peso minore
					data.ActionType = EActionType::Attack;
				}
			}
			else
			{
				data.ActorToAttack = nullptr;

				if (bestTiles.Num() > 1)
				{
					bestTiles.Remove(rBestTile);
					randomIndex = FMath::RandRange(0, bestTiles.Num() - 1);
					rBestTile = bestTiles[randomIndex];

					data.ActionType = EActionType::Move;
					data.TileToReach = rBestTile;
					tileOccupated.Add(rBestTile);
				}
				else
				{
					// Sarebbe da rivalutare gli altri tile che hanno un peso minore
					data.ActionType = EActionType::Stay;
				}
			}
		}
		else
		{
			if (unitToAttack)
			{
				data.ActionType = EActionType::MoveToAttack;
				data.TileToReach = rBestTile;
				data.ActorToAttack = unitToAttack;
				tileOccupated.Add(rBestTile);
			}
			else
			{
				data.ActionType = EActionType::Move;
				data.TileToReach = rBestTile;
				data.ActorToAttack = nullptr;
				tileOccupated.Add(rBestTile);
			}
		}

		AIDatas.Add(cEnemy) = data;
	}
}

void UAIManager::CalculateAttackCost(const FTileData& TileData, AActor* Enemy, FTileInfo& OutInfo, float& OutAttackCost)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());

	TArray<AActor*> unitsAttackable = IGamemodeAImanger::Execute_GetUnitsAttackable(GameMode, TileData, Enemy);
	// Gamemode.CheckAttack(tiledata, Enemy)

	if (unitsAttackable.Num() > 0)
	{
		for (AActor* cUnit : unitsAttackable)
		{
			if (pUnitsAttacked.Contains(cUnit))
			{
				OutInfo.ActorToAttack = nullptr;
				continue;
			}

			float unitDistCost = IGamemodeAImanger::Execute_GetPointsFromPath(GameMode, cUnit, TileData).Num() / 2.0f;
			// Magic number da sostiture con il currentAttackRange
			// float damageCost = 1 - 0.0f; //(FMath::Max(0, cUnit->GetComponentByClass(CombatComponent).currentHealth - Enemy.GetComponentByClass(WeaponCompoennt).damage))
			float tempAttackCost = unitDistCost * unitDistWeight;
			// damageCost * damageWeight;

			if (tempAttackCost > OutAttackCost)
			{
				OutAttackCost = tempAttackCost;
				OutInfo.ActorToAttack = cUnit;
			}
		}

		pUnitsAttacked.Add(OutInfo.ActorToAttack);
	}
}

FIntPoint UAIManager::FindShorterPath(const TArray<AActor*> Allies, AActor* Enemy)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	int32 nearestPath = MAX_int32;
	AActor* unitClosest = nullptr;

	TArray<FTileData> tilesInRange = IGamemodeAImanger::Execute_GetTilesFromBFS(GameMode, Enemy);
	FTileData TileData = IGamemodeAImanger::Execute_GetTileData(GameMode, Enemy);

	for (int i = 0; i < Allies.Num(); ++i)
	{
		if (pUnitsAttacked.Contains(Allies[i]))
		{
			continue;
		}

		TArray<FIntPoint> path = IGamemodeAImanger::Execute_GetPointsFromPath(GameMode, Allies[i], TileData);
		if (path.Num() < nearestPath)
		{
			unitClosest = Allies[i];
			nearestPath = path.Num();
		}
	}

	FIntPoint bestPoint = FIntPoint(-1, -1);
	TArray<FIntPoint> path = IGamemodeAImanger::Execute_GetPointsFromPath(GameMode, unitClosest, TileData);
	for (FIntPoint p : path)
	{
		FTileData d = IGamemodeAImanger::Execute_GetTileDataFromPoint(GameMode, p);
		if (tilesInRange.Contains(d))
		{
			bestPoint = p;
			pUnitsAttacked.Add(unitClosest);
			break;
		}
	}

	return bestPoint;
}
