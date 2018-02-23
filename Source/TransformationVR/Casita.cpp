// Fill out your copyright notice in the Description page of Project Settings.

#include "Casita.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"

ACasita::ACasita() {
    BloqueMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Casita"));
    RootComponent = BloqueMesh;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CasitaMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Casita/Casita.Casita'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (CasitaMeshAsset.Succeeded()) {
        BloqueMesh->SetStaticMesh(CasitaMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> CasitaMaterialAsset(TEXT("Material'/Game/Trasnformation/Assets/Meshes/Casita/Casa.Casa'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (CasitaMaterialAsset.Succeeded()) {
            BloqueMesh->SetMaterial(0, CasitaMaterialAsset.Object);
        }
    }
    BloqueMesh->SetCollisionProfileName(FName(TEXT("Bloque")));
	TWidget = CreateDefaultSubobject<UTransformacionWidget>(TEXT("TWidget"));
	TWidget->SetupAttachment(RootComponent);
	TWidget->SetRelativeLocation(FVector::ZeroVector);
}

void ACasita::BeginPlay() {
}

void ACasita::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}
