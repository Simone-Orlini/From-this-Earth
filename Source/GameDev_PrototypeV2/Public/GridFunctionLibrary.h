// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDEV_PROTOTYPEV2_API UGridFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static FVector SnapVectorToVector(FVector Location, FVector GridSize);
	static FVector SnapVectorToVector(FVector Location, FVector2D GridSize);
	
	template<typename T>
	static bool IsEven(T number)
	{
		if (std::is_arithmetic_v<T>)
			return static_cast<int>(number) % 2 == 0;
		
		return false;
	}
	
};

