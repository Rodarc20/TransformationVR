// Fill out your copyright notice in the Description page of Project Settings.

#include "Escena.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "TransformacionWidget.h"


// Sets default values
AEscena::AEscena() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    TWidget = CreateDefaultSubobject<UTransformacionWidget>(TEXT("Ejes"));
    RootComponent = TWidget;

    TWidget->MostrarWidget();

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
}

void AEscena::OcultarWidget() {
}

