// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "NiagaraComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	outline = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Outline"));
	RootComponent = outline;

	VFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
	VFX->SetupAttachment(RootComponent);
	VFX->bAutoActivate = false;

	VisualElementMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualElementMeshComp"));
	VisualElementMeshComp->SetupAttachment(RootComponent);
	VisualElementMeshComp->SetVisibility(false);
	VisualElementMeshComp->SetRelativeScale3D(FVector(4.0f));
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
	 * 
	 * 
	 */


	// if (Actor == Ally)

	// The unit reached the ToReachTile
	if (tileData.GridTileData.TileType == ETileType::ToReach)
	{
		VFX->SetActive(false);
		tileData.GridTileData.TileType = ETileType::Empty;
	}
}

void ATile::SpawnTile(FTileData _tileData, TArray<UStaticMesh*> meshes)
{
	InitTileData(_tileData);
	InitVisualElement(meshes);


	outline->SetStaticMesh(tileData.Outline);
	VFX->SetAsset(tileData.VFX);
	VFX->SetActive(false);

	switch (tileData.GridTileData.TileType)
	{
	default:
	case ETileType::Empty:
		break;
	case ETileType::ToReach:
		VFX->SetActive(true);
		break;
	case ETileType::Bush:
		SpawnVisualElement(EMaskType::Grass);
		break;
	case ETileType::Mud:
		SpawnVisualElement(EMaskType::Mud);
		break;
	case ETileType::Fire:
		SpawnVisualElement(EMaskType::Fire);
		break;
	};

	UpdateOutline();
}

void ATile::InitTileData(FTileData _tileData)
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

	// UE_LOG(LogTemp, Warning, TEXT("Enum:" + s));
	// if (s == ETileStatus::None)
	// {
	// 	outline->SetVisibility(false);
	// 	return;
	// }

	//outline->SetVisibility(true);

	FLinearColor colorBorder = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
	FLinearColor colorFill = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
	outline->CreateAndSetMaterialInstanceDynamic(0)->SetScalarParameterValue(FName("Fill"), 1.0f);

	switch (s)
	{
	case ETileStatus::Highlight:
		colorBorder = FLinearColor(0.5f, 0.5f, 0.5f);
		colorFill = colorBorder;
		outline->CreateAndSetMaterialInstanceDynamic(0)->SetScalarParameterValue(FName("Fill"), 0.5f);

		break;
	case ETileStatus::Hovered:
		colorBorder = FLinearColor(0.2f, 0.35f, 0.0f);
		colorFill = colorBorder;
		break;
	case ETileStatus::Movement:
		colorBorder = FLinearColor(0.15f, 0.45f, 0.0f);
		colorFill = colorBorder;
		break;
	case ETileStatus::Attack:
		colorBorder = FLinearColor(0.9f, 0.3f, 0.0f);
		colorFill = colorBorder;
		break;
	default:
	case ETileStatus::None:
		outline->CreateAndSetMaterialInstanceDynamic(0)->SetScalarParameterValue(FName("Fill"), 0.0f);
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

void ATile::EnableVFX()
{
	VFX->SetActive(true);
}

void ATile::InitVisualElement(TArray<UStaticMesh*> meshes)
{
	int32 index = 0;
	for (int32 i = 0; i < meshes.Num(); i++)
	{
		VisualTileTypeElements.Add(static_cast<EMaskType>(i + 1), meshes[i]);
	}
}

void ATile::SpawnVisualElement(EMaskType MaskType)
{
	if (VisualTileTypeElements.Find(MaskType))
	{
		VisualElementMeshComp->SetStaticMesh(*VisualTileTypeElements.Find(MaskType));
		VisualElementMeshComp->SetVisibility(true);

		switch (MaskType)
		{
		default:
		case EMaskType::NONE:
			// Non esiste la maschera
			break;
		case EMaskType::Grass:
			tileData.GridTileData.TileType = ETileType::Bush;
			break;
		case EMaskType::Mud:
			tileData.GridTileData.TileType = ETileType::Mud;
			break;
		case EMaskType::Fire:
			tileData.GridTileData.TileType = ETileType::Fire;
			break;
		}
	}
}
