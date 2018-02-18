// Fill out your copyright notice in the Description page of Project Settings.

#include "Bloque.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ABloque::ABloque() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

}

// Called when the game starts or when spawned
void ABloque::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void ABloque::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);


}

void ABloque::SeguirObjeto(USceneComponent * Objeto) {
    ObjetoSeguir = Objeto;
    bSeguir = true;
    UE_LOG(LogClass, Log, TEXT("Cambiando a seguir"));
    RotacionInicial = GetActorTransform().GetRotation().Rotator();
    TArray<USceneComponent *> parents;
    Objeto->GetParentComponents(parents);
    RotacionInicialObjeto = parents[0]->GetRelativeTransform().GetRotation().Rotator();

}

void ABloque::NoSeguir() {
    bSeguir = false;
    ObjetoSeguir = nullptr;
}

