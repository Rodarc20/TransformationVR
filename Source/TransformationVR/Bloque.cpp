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
    RotacionInicialObjeto = parents[0]->GetRelativeTransform().GetRotation().Rotator();//esto es por que necesitoi rotacion del control, no del componente punto referencia

}

void ABloque::SeguirObjetos(USceneComponent * ObjetoInicial, USceneComponent * ObjetoFinal) {//el segundo seria el segundo control, que debeir estar sugentando tambien al objeto
    ObjetoSeguir = ObjetoInicial;
    ObjetoSeguirFinal = ObjetoFinal;
    UE_LOG(LogClass, Log, TEXT("Cambiando a seguiendo"));
    bSeguir = true;
    RotacionInicial = GetActorTransform().GetRotation().Rotator();
    TArray<USceneComponent *> parentsInicial;
    ObjetoInicial->GetParentComponents(parentsInicial);
    TArray<USceneComponent *> parentsFinal;
    ObjetoFinal->GetParentComponents(parentsFinal);
    RotacionInicialObjeto = parentsInicial[0]->GetRelativeTransform().GetRotation().Rotator();//esto es por que necesitoi rotacion del control, no del componente punto referencia
    DistanciaInicialObjetos = (parentsFinal[0]->GetComponentLocation() - parentsInicial[0]->GetComponentLocation()).Size();
    //deberia ser usando la posicion de los controles no de los componetes referencia
    UE_LOG(LogClass, Log, TEXT("DistanciaInicialObjetos: %f"), DistanciaInicialObjetos);
}

void ABloque::NoSeguir() {
    bSeguir = false;
    ObjetoSeguir = nullptr;
    ObjetoSeguirFinal = nullptr;
}


//voya necesitar puntos centrales, puntos inciales escalas iniciales, para que sean usados por el hud de cada tarea