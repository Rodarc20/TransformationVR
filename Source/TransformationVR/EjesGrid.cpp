// Fill out your copyright notice in the Description page of Project Settings.

#include "EjesGrid.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UEjesGrid::UEjesGrid()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PlanoXY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlanoXY"));
    PlanoXY->SetupAttachment(this);
	PlanoXY->SetRelativeLocation(FVector(100.0f, -100.0f, 0.05f));
	PlanoXY->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
    //ahora estan con sacala 1

	PlanoXZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlanoXZ"));
    PlanoXZ->SetupAttachment(this);
	PlanoXZ->SetRelativeLocation(FVector(100.0f, 0.0f, 100.5f));
	PlanoXZ->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));

	PlanoYZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlanoYZ"));
    PlanoYZ->SetupAttachment(this);
	//FlechaZ->SetRelativeScale3D(FVector(0.5f, 0.25f, 0.25f));
	PlanoYZ->SetRelativeLocation(FVector(0.0f, -100.0f, 100.5f));
	PlanoYZ->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlanoAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (PlanoAsset.Succeeded()) {
        PlanoXY->SetStaticMesh(PlanoAsset.Object);
        PlanoXZ->SetStaticMesh(PlanoAsset.Object);
        PlanoYZ->SetStaticMesh(PlanoAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> PlanoXYMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/GridXY.GridXY'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (PlanoXYMaterialAsset.Succeeded()) {
			MaterialXY = PlanoXYMaterialAsset.Object;
            PlanoXY->SetMaterial(0, PlanoXYMaterialAsset.Object);
        }
        static ConstructorHelpers::FObjectFinder<UMaterial> PlanoXZMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/GridXZ.GridXZ'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (PlanoXZMaterialAsset.Succeeded()) {
			MaterialXZ = PlanoXZMaterialAsset.Object;
            PlanoXZ->SetMaterial(0, PlanoXZMaterialAsset.Object);
        }
        static ConstructorHelpers::FObjectFinder<UMaterial> PlanoYZMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/GridYZ.GridYZ'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (PlanoYZMaterialAsset.Succeeded()) {
			MaterialYZ = PlanoYZMaterialAsset.Object;
            PlanoYZ->SetMaterial(0, PlanoYZMaterialAsset.Object);
        }
    }
}


// Called when the game starts
void UEjesGrid::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEjesGrid::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEjesGrid::Mostrar() {
    PlanoXY->SetVisibility(true);
    PlanoXZ->SetVisibility(true);
    PlanoYZ->SetVisibility(true);
}

void UEjesGrid::Ocultar() {
    PlanoXY->SetVisibility(false);
    PlanoXZ->SetVisibility(false);
    PlanoYZ->SetVisibility(false);
}

