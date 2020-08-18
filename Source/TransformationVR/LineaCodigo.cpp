// Fill out your copyright notice in the Description page of Project Settings.

#include "LineaCodigo.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
ALineaCodigo::ALineaCodigo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("WidgetBlueprintGeneratedClass'/Game/Trasnformation/UMG/LineaCodigo.LineaCodigo_C'"));
    Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Codigo"));
	RootComponent = Widget;
    Widget->SetWidgetSpace(EWidgetSpace::World);
    //Widget->SetupAttachment(MotionControllerLeft);
    Widget->SetDrawSize(FVector2D(1000.0f, 100.0f));
    Widget->SetPivot(FVector2D(0.5f, 0.5f));
    Widget->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
    if (WidgetClass.Succeeded()) {
        Widget->SetWidgetClass(WidgetClass.Class);
    }
}

// Called when the game starts or when spawned
void ALineaCodigo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALineaCodigo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALineaCodigo::Mostrar() {
    Widget->SetVisibility(true);
}

void ALineaCodigo::Ocultar() {
    Widget->SetVisibility(false);
}
