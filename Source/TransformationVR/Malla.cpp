// Fill out your copyright notice in the Description page of Project Settings.

#include "Malla.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/StaticMesh.h"


// Sets default values
AMalla::AMalla()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Orgien"));

    Plano = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Malla"));
	Plano->SetupAttachment(RootComponent);
	Plano->SetRelativeLocation(FVector(0.0f, 0.0f, 0.01f));//tendra que ser la mitad del tamañó de la figura
    Plano->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MallaMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (MallaMeshAsset.Succeeded()) {
        Plano->SetStaticMesh(MallaMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> MallaMaterialAsset(TEXT("Material'/Game/Trasnformation/Materials/GridXY.GridXY'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (MallaMaterialAsset.Succeeded()) {
            Plano->SetMaterial(0, MallaMaterialAsset.Object);
        }
    }
    Plano->SetVisibility(false);
}

// Called when the game starts or when spawned
void AMalla::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMalla::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMalla::Mostrar() {
    Plano->SetVisibility(true);
}

void AMalla::Ocultar() {
    Plano->SetVisibility(false);
}

