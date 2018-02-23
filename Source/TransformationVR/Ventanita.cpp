// Fill out your copyright notice in the Description page of Project Settings.

#include "Ventanita.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"

AVentanita::AVentanita() {
	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    BloqueMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ventanita"));
    RootComponent = BloqueMesh;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> VentanitaMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Casita/Ventanita.Ventanita'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (VentanitaMeshAsset.Succeeded()) {//debo usar nombres generiicos para no cambair tanto lo repetido
        BloqueMesh->SetStaticMesh(VentanitaMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> VentanitaMaterialAsset(TEXT("Material'/Game/Trasnformation/Assets/Meshes/Casita/Ventana1.Ventana1'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (VentanitaMaterialAsset.Succeeded()) {
            BloqueMesh->SetMaterial(0, VentanitaMaterialAsset.Object);
        }
    }
    BloqueMesh->SetCollisionProfileName(FName(TEXT("Bloque")));
    Umbral = 15.0f;

    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("WidgetBlueprintGeneratedClass'/Game/Trasnformation/UMG/WidgetEscala.WidgetEscala_C'"));
    Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetEscala"));
    Widget->SetupAttachment(RootComponent);
    Widget->SetWidgetSpace(EWidgetSpace::World);
    //Widget->SetupAttachment(MotionControllerLeft);
    Widget->SetDrawSize(FVector2D(500.0f, 150.0f));
    Widget->SetPivot(FVector2D(0.5f, 0.5f));
    Widget->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    Widget->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
    Widget->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    if (WidgetClass.Succeeded()) {
        Widget->SetWidgetClass(WidgetClass.Class);
    }
    RotacionTemp = 0.0f;
}

void AVentanita::BeginPlay() {
    Super::BeginPlay();
}

void AVentanita::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    if (bSeguir && ObjetoSeguirFinal) {
        DistanciaObjetos = (ObjetoSeguirFinal->GetComponentLocation() - ObjetoSeguir->GetComponentLocation()).Size();
        float EscalaTemp = (DistanciaObjetos - DistanciaInicialObjetos) / DistanciaInicialObjetos;
        UE_LOG(LogClass, Log, TEXT("Escala Temp: %f"), EscalaTemp);
        //calcular la escala temporal y aplicar
        //aplicarlao, conservar el valor de escala actual para saber cual es la escala que tiene en este momenot,
        //problemas con la escala relativa
        //la escala se aplcara solo a uno de los ejes, en este caso el eje y tal vez o eje x
        SetActorRelativeScale3D(FVector(EscalaTemp, 1.0f, 1.0f));
        Widget->SetWorldScale3D(EscalaInicialWidget);

    }
}



