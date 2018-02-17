// Fill out your copyright notice in the Description page of Project Settings.

#include "Techito.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"

ATechito::ATechito() {
	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    BloqueMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Techito"));
    RootComponent = BloqueMesh;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TechitoMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Casita/Techito.Techito'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (TechitoMeshAsset.Succeeded()) {
        BloqueMesh->SetStaticMesh(TechitoMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> TechitoMaterialAsset(TEXT("Material'/Game/Trasnformation/Assets/Meshes/Casita/Techo.Techo'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (TechitoMaterialAsset.Succeeded()) {
            BloqueMesh->SetMaterial(0, TechitoMaterialAsset.Object);
        }
    }
    BloqueMesh->SetCollisionProfileName(FName(TEXT("Bloque")));
    Umbral = 15.0f;
    PosicionObjetivo = FVector(0.0f, 0.0f, 15.0f);

}

void ATechito::BeginPlay() {
    Super::BeginPlay();
}

void ATechito::Tick(float DeltaTime) {
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
                SetActorRelativeLocation(FVector(0.0f, PuntoSeguido.Y, PosicionObjetivo.Z));
            }
            //UE_LOG(LogClass, Log, TEXT("siguiendo"));
        }
    //}
}
