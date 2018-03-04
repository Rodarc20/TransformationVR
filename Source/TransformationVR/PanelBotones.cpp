// Fill out your copyright notice in the Description page of Project Settings.

#include "PanelBotones.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "Casa.h"
#include "Boton.h"
#include "BotonTrasladar.h"
#include "BotonRotar.h"
#include "BotonEscalar.h"


// Sets default values
APanelBotones::APanelBotones()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Centro"));

}

// Called when the game starts or when spawned
void APanelBotones::BeginPlay()
{
	Super::BeginPlay();
    UWorld * const World = GetWorld();
    if (World) {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = Instigator;

        BotonTrasladar = World->SpawnActor<ABoton>(ABotonTrasladar::StaticClass(), FVector(0.0f, -15.0f, 0.0f), FRotator::ZeroRotator, SpawnParams);
        BotonTrasladar->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);//segun el compilador de unral debo usar esto

        BotonRotar = World->SpawnActor<ABoton>(ABotonRotar::StaticClass(), FVector(0.0f, 0.0f, 0.0f), FRotator::ZeroRotator, SpawnParams);
        BotonRotar->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);//segun el compilador de unral debo usar esto

        BotonEscalar = World->SpawnActor<ABoton>(ABotonEscalar::StaticClass(), FVector(0.0f, 15.0f, 0.0f), FRotator::ZeroRotator, SpawnParams);
        BotonEscalar->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);//segun el compilador de unral debo usar esto
    }
}

// Called every frame
void APanelBotones::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APanelBotones::Press(ETransformacionTarea BotonPresionado) {
    //quien tiene a este boton? puedo poner de nuevo verificar quien es el owner, y castear y llamar a su funcion correspondiente, probar con Casa primero
    UE_LOG(LogClass, Log, TEXT("Boton de panel presionado"));//debo despresionar a los otrs dos botones que no se hayan presionado
    ACasa * Casa = Cast<ACasa>(GetOwner());
    if (Casa) {
        Casa->SetTransformacionTarea(BotonPresionado);
    }
    switch (BotonPresionado) {
        case ETransformacionTarea::ETrasladar: {
            BotonRotar->bPressed = false;
            BotonEscalar->bPressed = false;
        }
        break;
        case ETransformacionTarea::ERotar: {
            BotonTrasladar->bPressed = false;
            BotonEscalar->bPressed = false;
        }
        break;
        case ETransformacionTarea::EEscalar: {
            BotonTrasladar->bPressed = false;
            BotonRotar->bPressed = false;
        }
        break;
        default:
        case ETransformacionTarea::ENone: {
        }
        break;
    }
}

void APanelBotones::Release(ETransformacionTarea BotonPresionado) {
}

