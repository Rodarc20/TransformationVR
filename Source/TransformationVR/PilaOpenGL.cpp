// Fill out your copyright notice in the Description page of Project Settings.

#include "PilaOpenGL.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
APilaOpenGL::APilaOpenGL()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bActualizado = true;
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("WidgetBlueprintGeneratedClass'/Game/Trasnformation/UMG/PilaWidget.PilaWidget_C'"));
    Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Codigo"));
	RootComponent = Widget;
    Widget->SetWidgetSpace(EWidgetSpace::World);
    //Widget->SetupAttachment(MotionControllerLeft);
    Widget->SetDrawSize(FVector2D(2800.0f, 2800.0f));
    Widget->SetPivot(FVector2D(0.5f, 0.5f));
    Widget->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
    if (WidgetClass.Succeeded()) {
        Widget->SetWidgetClass(WidgetClass.Class);
    }

}

// Called when the game starts or when spawned
void APilaOpenGL::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APilaOpenGL::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APilaOpenGL::CambiarCodigo(FString NuevoCodigo) {
	Codigo = NuevoCodigo;
	bActualizado = false;//el erro se marca en esta linea
}

void APilaOpenGL::Mostrar() {
    Widget->SetVisibility(true);
}

void APilaOpenGL::Ocultar() {
    Widget->SetVisibility(false);
}

