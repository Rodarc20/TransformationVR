// Fill out your copyright notice in the Description page of Project Settings.

#include "Ventanita.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"

AVentanita::AVentanita() {
	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    BloqueMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ventanita"));
    RootComponent = BloqueMesh;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> VentanitaMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Casita/Ventanita.Ventanita'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (VentanitaMeshAsset.Succeeded()) {//debo usar nombres generiicos para no cambair tanto lo repetido
        BloqueMesh->SetStaticMesh(VentanitaMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> VentanitaMaterialAsset(TEXT("Material'/Game/Trasnformation/Assets/Meshes/Casita/Ventana1.Ventana1'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (VentanitaMaterialAsset.Succeeded()) {
            BloqueMesh->SetMaterial(0, VentanitaMaterialAsset.Object);
        }
    }
    BloqueMesh->SetCollisionProfileName(FName(TEXT("Bloque")));
    Umbral = 15.0f;

}

void AVentanita::BeginPlay() {
    Super::BeginPlay();
}

void AVentanita::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    if (bSeguir && ObjetoSeguirFinal) {
        DistanciaObjetos = (ObjetoSeguirFinal->GetComponentLocation() - ObjetoSeguir->GetComponentLocation()).Size();
        float EscalaTemp = (DistanciaObjetos - DistanciaInicialObjetos) / DistanciaInicialObjetos;
        UE_LOG(LogClass, Log, TEXT("Escala Temp: %f"), EscalaTemp);
        //calcular la escala temporal y aplicar
        //aplicarlao, conservar el valor de escala actual para saber cual es la escala que tiene en este momenot,
        //problemas con la escala relativa
        //la escala se aplcara solo a uno de los ejes, en este caso el eje y tal vez o eje x
        SetActorRelativeScale3D(FVector(EscalaTemp, 1.0f, 1.0f));
    }
}



