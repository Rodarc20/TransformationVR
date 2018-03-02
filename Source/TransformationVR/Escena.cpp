// Fill out your copyright notice in the Description page of Project Settings.

#include "Escena.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "TransformacionWidget.h"
#include "Engine/StaticMesh.h"


// Sets default values
AEscena::AEscena() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    //TWidget = CreateDefaultSubobject<UTransformacionWidget>(TEXT("Ejes"));

	Origen = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Origen"));
    Origen->SetupAttachment(RootComponent);
	Origen->SetRelativeScale3D(FVector(0.40f));
	//ArticulacionCuello->SetWorldScale3D(FVector(0.05f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> OrigenAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Articulacion.Articulacion'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (OrigenAsset.Succeeded()) {
        Origen->SetStaticMesh(OrigenAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> OrigenMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/Origen.Origen'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (OrigenMaterialAsset.Succeeded()) {
            Origen->SetMaterial(0, OrigenMaterialAsset.Object);
			//ArticulacionMaterialDynamic = UMaterialInstanceDynamic::Create(CuelloMaterialAsset.Object, this);
        }
    }

	FlechaX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlechaX"));
    FlechaX->SetupAttachment(RootComponent);
	FlechaX->SetRelativeScale3D(FVector(1.0f, 0.25f, 0.25f));

	FlechaY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlechaY"));
    FlechaY->SetupAttachment(RootComponent);
	FlechaY->SetRelativeScale3D(FVector(1.0f, 0.25f, 0.25f));
	FlechaY->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	FlechaZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlechaZ"));
    FlechaZ->SetupAttachment(RootComponent);
	FlechaZ->SetRelativeScale3D(FVector(1.0f, 0.25f, 0.25f));
	FlechaZ->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> FlechaAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/FlechaGrande.FlechaGrande'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (FlechaAsset.Succeeded()) {
        FlechaX->SetStaticMesh(FlechaAsset.Object);
        FlechaY->SetStaticMesh(FlechaAsset.Object);
        FlechaZ->SetStaticMesh(FlechaAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> FlechaXMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaX.FlechaX'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (FlechaXMaterialAsset.Succeeded()) {
			//EjeXNormal = FlechaXMaterialAsset.Object;
            FlechaX->SetMaterial(0, FlechaXMaterialAsset.Object);
        }
        static ConstructorHelpers::FObjectFinder<UMaterial> FlechaYMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaY.FlechaY'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (FlechaYMaterialAsset.Succeeded()) {
			//EjeYNormal = FlechaYMaterialAsset.Object;
            FlechaY->SetMaterial(0, FlechaYMaterialAsset.Object);
        }
        static ConstructorHelpers::FObjectFinder<UMaterial> FlechaZMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/FlechaZ.FlechaZ'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (FlechaZMaterialAsset.Succeeded()) {
			//EjeZNormal = FlechaZMaterialAsset.Object;
            FlechaZ->SetMaterial(0, FlechaZMaterialAsset.Object);
        }
    }

    FlechaX->SetCollisionProfileName(FName(TEXT("Transformacion")));
    FlechaY->SetCollisionProfileName(FName(TEXT("Transformacion")));
    FlechaZ->SetCollisionProfileName(FName(TEXT("Transformacion")));

    //TWidget->OcultarWidget();

}

// Called when the game starts or when spawned
void AEscena::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void AEscena::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AEscena::MostrarWidget() {
	Origen->SetVisibility(true);
	FlechaX->SetVisibility(true);
	FlechaY->SetVisibility(true);
	FlechaZ->SetVisibility(true);
}

void AEscena::OcultarWidget() {
	Origen->SetVisibility(false);
	FlechaX->SetVisibility(false);
	FlechaY->SetVisibility(false);
	FlechaZ->SetVisibility(false);
}

