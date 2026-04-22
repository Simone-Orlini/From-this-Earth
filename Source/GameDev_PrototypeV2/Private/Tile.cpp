// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "NiagaraComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	outline = CreateDefaultSubobject<UStaticMeshComponent>("Outline");
	outline->SetupAttachment(RootComponent);

	VFX = CreateDefaultSubobject<UNiagaraComponent>("VFX");
	VFX->SetupAttachment(RootComponent);
	VFX->SetVisibility(false);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATile::ApplyEffect()
{
	switch (tileData.GridTileData.TileType)
	{
	default:
	case ETileType::Bush:
	case ETileType::ToReach:
	case ETileType::Empty:
		// Do nothing
		return;
	case ETileType::Fire:
		// ApplyDamageToUnit when enter and at the start of every round
		return;
	case ETileType::Mud:
		// Half the Max movement range 5/2 = 2
		return;
	}
}

void ATile::UnitEnterTile(AActor* Actor)
{
	/* Quando una unit entra nel tile va prima definito se e' alleata o nemica
	 * In caso di alleta si deve dire al gamemode se la unit ha raccolto un item
	 * L'unita deve avere delle informazioni del tile
	 */
	
	
	// if (Actor == Ally)
		
	// The unit reached the ToReachTile
	if (tileData.GridTileData.TileType == ETileType::ToReach)
	{
		VFX->SetVisibility(false);
		tileData.GridTileData.TileType = ETileType::Empty;
		
		// Notify to the maskManager
	}
}

void ATile::SpawnTile(const FTileData& _tileData)
{
	InitTileData(_tileData);

	outline = tileData.Outline;
	VFX = tileData.VFX;
}

void ATile::InitTileData(const FTileData& _tileData)
{
	tileData = _tileData;
}

void ATile::UpdateOutline()
{
	tileData.TileStatuses.Sort([](const ETileStatus& A, const ETileStatus& B)
	{
		return (uint8)A < (uint8)B;
	});

	ETileStatus s = tileData.TileStatuses[0];
	if (s == ETileStatus::None)
	{
		outline->SetVisibility(false);
		return;
	}

	outline->SetVisibility(true);

	FLinearColor colorBorder = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
	FLinearColor colorFill = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);

	switch (s)
	{
	case ETileStatus::Highlight:
		colorBorder = FLinearColor(0.5f, 0.5f, 0.5f);
		colorFill = colorBorder;
	case ETileStatus::Hovered:
		colorBorder = FLinearColor(0.2f, 0.35f, 0.0f);
		colorFill = colorBorder;
	case ETileStatus::Movement:
		colorBorder = FLinearColor(0.15f, 0.45f, 0.0f);
		colorFill = colorBorder;
	case ETileStatus::Attack:
		colorBorder = FLinearColor(0.9f, 0.3f, 0.0f);
		colorFill = colorBorder;
	default:
	case ETileStatus::None:
		break;
	}

	outline->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(FName("Color_Border"), colorBorder);
	outline->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(FName("Color_Fill"), colorFill);
}

void ATile::AddOutlineStatus(ETileStatus newStatus)
{
	tileData.TileStatuses.AddUnique(newStatus);
}

void ATile::RemoveOutlineStatus(ETileStatus oldStatus)
{
	tileData.TileStatuses.Remove(oldStatus);
}
