// Fill out your copyright notice in the Description page of Project Settings.

#include "Techito.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"

ATechito::ATechito() {
	//PrimaryActorTick.bCanEverTick = true;

    BloqueMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Techito"));
    RootComponent = BloqueMesh;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TechitoMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Casita/Techito.Techito'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (TechitoMeshAsset.Succeeded()) {
        BloqueMesh->SetStaticMesh(TechitoMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> TechitoMaterialAsset(TEXT("Material'/Game/Trasnformation/Assets/Meshes/Casita/TEcho.Techo'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (TechitoMaterialAsset.Succeeded()) {
            BloqueMesh->SetMaterial(0, TechitoMaterialAsset.Object);
        }
    }
    BloqueMesh->SetCollisionProfileName(FName(TEXT("Bloque")));
}

void ATechito::BeginPlay() {
}

