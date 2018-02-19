// Fill out your copyright notice in the Description page of Project Settings.

#include "Puertita.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"

APuertita::APuertita() {
	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    BloqueMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Puertita"));
    RootComponent = BloqueMesh;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> PuertitaMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Casita/Puertita.Puertita'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (PuertitaMeshAsset.Succeeded()) {
        BloqueMesh->SetStaticMesh(PuertitaMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> PuertitaMaterialAsset(TEXT("Material'/Game/Trasnformation/Assets/Meshes/Casita/Puerta1.Puerta1'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (PuertitaMaterialAsset.Succeeded()) {
            BloqueMesh->SetMaterial(0, PuertitaMaterialAsset.Object);
        }
    }
    BloqueMesh->SetCollisionProfileName(FName(TEXT("Bloque")));
    Umbral = 15.0f;
    PosicionObjetivo = FVector(9.0f, 0.0f, 5.0f);

}

void APuertita::BeginPlay() {
    Super::BeginPlay();
}

void APuertita::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    //UE_LOG(LogClass, Log, TEXT("siguiendo %d"), bSeguir);
    //if (bSobrepasoUmbral) {
        //SetActorLocation(ObjetoSeguir->GetComponentLocation());
    //}
    //else {
        if (bSeguir) {
            FVector PuntoSeguido = GetAttachParentActor()->GetActorTransform().InverseTransformPosition(ObjetoSeguir->GetComponentLocation());//de world a local
            FVector PuntoSeguidoSinY = PuntoSeguido;
            PuntoSeguidoSinY.Y = 0.0f;
            float Diferencia = (PuntoSeguidoSinY - PosicionObjetivo).Size();
            if (Diferencia > Umbral){
                bSobrepasoUmbral = true;
            }
            else {
                bSobrepasoUmbral = false;
            }
            if (bSobrepasoUmbral) {
                SetActorLocation(ObjetoSeguir->GetComponentLocation());
                //debo verificar que este otra vz dentro del rango para regresar elsobrepaso el umbra a falso
                //a travez de esto puedo solucionar el problema de fisicas
            }
            else {
                SetActorRelativeLocation(FVector(PosicionObjetivo.X, PuntoSeguido.Y, PosicionObjetivo.Z));
            }
            //UE_LOG(LogClass, Log, TEXT("siguiendo"));
            TArray<USceneComponent *> parents;
            ObjetoSeguir->GetParentComponents(parents);
            FRotator DiferenciaRotacion = parents[0]->GetRelativeTransform().GetRotation().Rotator() - RotacionInicialObjeto;
            //FRotator DiferenciaRotacion = ObjetoSeguir->GetRelativeTransform().GetRotation().Rotator() - RotacionInicialObjeto;
            DiferenciaRotacion.Pitch = 0.0f;
            DiferenciaRotacion.Yaw = 0.0f;
            DiferenciaRotacion *= -1;// este menos deberia ser dependiendo de aodnde este mirando el control
            SetActorRelativeRotation(RotacionInicial + DiferenciaRotacion);
        }
    //}
}




