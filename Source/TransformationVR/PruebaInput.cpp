// Fill out your copyright notice in the Description page of Project Settings.

#include "PruebaInput.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"


// Sets default values
APruebaInput::APruebaInput()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

}

// Called when the game starts or when spawned
void APruebaInput::BeginPlay()
{
	Super::BeginPlay();

    //o usar el auto receive
    EnableInput(GetWorld()->GetFirstPlayerController());
    bBlockInput = 0;
    //AutoReceiveInput = EAutoReceiveInput::Player0;
    
     UInputComponent* myInputComp = this->InputComponent;  //InputComponent variable is from AActor.h
     //check that it is valid before calling
     if (myInputComp) {
         SetupPlayerInputComponent(myInputComp);
     }
}

// Called every frame
void APruebaInput::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APruebaInput::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent) {
    //PlayerInputComponent->bBlockInput = 0;
    PlayerInputComponent->BindAction("Moverse", IE_Pressed, this, &APruebaInput::PadIzquierdoPressed).bConsumeInput = 0;
    PlayerInputComponent->BindAction("Moverse", IE_Released, this, &APruebaInput::PadIzquierdoReleased).bConsumeInput = 0;
}

void APruebaInput::PadIzquierdoPressed() {
    UE_LOG(LogClass, Log, TEXT("Prueba input pressed"));
}

void APruebaInput::PadIzquierdoReleased() {
    UE_LOG(LogClass, Log, TEXT("Prueba input released"));
}

