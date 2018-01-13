// Fill out your copyright notice in the Description page of Project Settings.

#include "Nodo.h"
#include "Components/WidgetComponent.h"
#include "Public/UObject/ConstructorHelpers.h"


// Sets default values
ANodo::ANodo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bActualizado = true;
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("WidgetBlueprintGeneratedClass'/Game/Trasnformation/UMG/NodoWidget.NodoWidget_C'"));
    Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NodoInformacion"));
	RootComponent = Widget;
    Widget->SetWidgetSpace(EWidgetSpace::World);
    //Widget->SetupAttachment(MotionControllerLeft);
    Widget->SetDrawSize(FVector2D(500.0f, 250.0f));
    Widget->SetPivot(FVector2D(0.5f, 0.5f));
    Widget->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
    if (WidgetClass.Succeeded()) {
        Widget->SetWidgetClass(WidgetClass.Class);
    }

}

// Called when the game starts or when spawned
void ANodo::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void ANodo::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void ANodo::CambiarNombreParte(int NuevoIdParte) {
	if (NuevoIdParte == 0) {
		NombreParte = "Torso";
	}
	else if (NuevoIdParte == 1) {
		NombreParte = "Cabeza";
	}
	else if (NuevoIdParte == 2) {
		NombreParte = "Brazo Derecho";
	}
	else if (NuevoIdParte == 3) {
		NombreParte = "Brazo Izquierdo";
	}
	else if (NuevoIdParte == 4) {
		NombreParte = "Pierna Derecha";
	}
	else if (NuevoIdParte == 5) {
		NombreParte = "Pierna Izquierda";
	}
	else if (NuevoIdParte == 6) {
		NombreParte = "Mano Derecha";
	}
	else if (NuevoIdParte == 7) {
		NombreParte = "Mano Izquierda";
	}
	else if (NuevoIdParte == 8) {
		NombreParte = "Pie Derecho";
	}
	else if (NuevoIdParte == 9) {
		NombreParte = "Pie Izquierdo";
	}
	else {
		NombreParte = "Ninguno";
	}
	bActualizado = false;
}

void ANodo::CambiarTraslacion(FVector NuevaTraslacion) {
	TextoTraslacion = "Traslacion (" + FString::SanitizeFloat(NuevaTraslacion.X) + ", " + FString::SanitizeFloat(NuevaTraslacion.Y) + ", " + FString::SanitizeFloat(NuevaTraslacion.Z) + ")";
	bActualizado = false;
}

void ANodo::CambiarRotacion(FVector NuevaRotacion) {
	TextoRotacion = "Traslacion (" + FString::SanitizeFloat(NuevaRotacion.X) + ", " + FString::SanitizeFloat(NuevaRotacion.Y) + ", " + FString::SanitizeFloat(NuevaRotacion.Z) + ")";
	bActualizado = false;
}

