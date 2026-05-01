// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIData.h"
#include "AIManager.generated.h"

struct FTileData;
struct FTileInfo;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEDEV_PROTOTYPEV2_API UAIManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIManager();
	
	UPROPERTY(BlueprintReadWrite, Category="AIManager")
	TMap<AActor*, FAIData> AIDatas; 
	
	
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AIManager|Weight|Attack")
	float unitDistWeight = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AIManager|Weight|Attack")
	float damageWeight = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AIManager|Weight")
	float tileWeight = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AIManager|Weight")
	float distWeight = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AIManager|Weight")
	float attackWeight = 1.0f;
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AIManager|Weight")
	// float distFromUnitWeight = 1.0f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void CalculateAttackCost(const FTileData& TileData, AActor* Enemy, TArray<AActor*>& PUnitsAttacked, FTileInfo& OutInfo, float& OutAttackCost);
	int32 FindShorterPath(const TArray<AActor*> Allies, const FTileData& Tiledata, AActor* Enemy);
	
	

public:	
	UFUNCTION(BlueprintCallable, Category = "AIManager")
	void StartLogic(const TArray<AActor*>& Enemies, const TArray<AActor*>& Allies);
		
};
