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
	
	UPROPERTY(BlueprintReadOnly, Category="AIManager")
	TMap<AActor*, FAIData> AIDatas;  
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AIManager|Weight")
	float unitDistWeight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AIManager|Weight")
	float damageWeight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AIManager|Weight")
	float tileWeight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AIManager|Weight")
	float distWeight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AIManager|Weight")
	float attackWeight;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void CalculateAttackCost(const FTileData& TileData,  const AActor* Enemy, const TArray<AActor*>& PUnitsAttacked, FTileInfo& OutInfo, float& OutAttackCost);
	
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void StartLogic(const TArray<AActor*>& Enemies);
		
};
