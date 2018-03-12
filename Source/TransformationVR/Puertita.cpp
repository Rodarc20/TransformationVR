// Fill out your copyright notice in the Description page of Project Settings.

#include "Puertita.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Engine/StaticMesh.h"

APuertita::APuertita() {
	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    BloqueMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Puertita"));
    RootComponent = BloqueMesh;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> PuertitaMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Casita/Puertita.Puertita'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (PuertitaMeshAsset.Succeeded()) {
        BloqueMesh->SetStaticMesh(PuertitaMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> PuertitaMaterialAsset(TEXT("Material'/Game/Trasnformation/Assets/Meshes/Casita/Puerta1.Puerta1'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (PuertitaMaterialAsset.Succeeded()) {
            BloqueMesh->SetMaterial(0, PuertitaMaterialAsset.Object);
        }
    }
    BloqueMesh->SetCollisionProfileName(FName(TEXT("Bloque")));
    Umbral = 5.0f;//cuantos frados de umbral
    RotacionObjetivo = FRotator(0.0f, 0.0f, 0.0f);

    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("WidgetBlueprintGeneratedClass'/Game/Trasnformation/UMG/WidgetRotacion.WidgetRotacion_C'"));
    Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetRotacion"));
    Widget->SetupAttachment(RootComponent);
    Widget->SetWidgetSpace(EWidgetSpace::World);
    //Widget->SetupAttachment(MotionControllerLeft);
    Widget->SetDrawSize(FVector2D(920.0f, 920.0f));
    Widget->SetPivot(FVector2D(0.5f, 0.5f));
    Widget->SetRelativeLocation(FVector(1.0f, 0.0f, 0.0f));
    Widget->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
    Widget->SetRelativeRotation(FRotator(00.0f, 0.0f, 0.0f));
    if (WidgetClass.Succeeded()) {
        Widget->SetWidgetClass(WidgetClass.Class);
    }
    Widget->SetVisibility(false);
    RotacionTemp = 0.0f;

	TWidget = CreateDefaultSubobject<UTransformacionWidget>(TEXT("TWidget"));
	TWidget->SetupAttachment(RootComponent);
	TWidget->SetRelativeLocation(FVector::ZeroVector);
    //TWidget->MostrarWidgetOrigen();
    TWidget->OcultarWidgetTraslacion();
}

void APuertita::BeginPlay() {
    Super::BeginPlay();
}

void APuertita::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (bSeguir) {
        //no tengo una rotacion objetivo
        TArray<USceneComponent *> parents;
        ObjetoSeguir->GetParentComponents(parents);
        FRotator DiferenciaRotacion = parents[0]->GetRelativeTransform().GetRotation().Rotator() - RotacionInicialObjeto;
        DiferenciaRotacion.Pitch = 0.0f;
        DiferenciaRotacion.Yaw = 0.0f;
        RotacionTemp = DiferenciaRotacion.Roll;
        DiferenciaRotacion *= -1;// este menos deberia ser dependiendo de aodnde este mirando el control
        FRotator NuevaRotacion = RotacionInicial + DiferenciaRotacion;
        float Diferencia = FMath::Abs(NuevaRotacion.Roll - RotacionObjetivo.Roll);
        if (Diferencia > Umbral){
            bSobrepasoUmbral = true;
        }
        else {
            bSobrepasoUmbral = false;
        }
        if (bSobrepasoUmbral) {
            //FRotator DiferenciaRotacion = ObjetoSeguir->GetRelativeTransform().GetRotation().Rotator() - RotacionInicialObjeto;
            //SetActorRelativeRotation(NuevaRotacion);
            //Widget->SetWorldRotation(RotacionInicialWidget);

            float DiferenciaEntero = DiferenciaRotacion.Roll - EnteroActual;//el entero actual es como la posicion inicial
            if (FMath::Abs(DiferenciaEntero) >= UmbralEntero) {
                if (DiferenciaEntero >= 0.0f) {//osa es un  numero positivo
                    RotacionTemp = FMath::CeilToInt(DiferenciaEntero);
                    DiferenciaRotacion.Roll = FMath::CeilToInt(DiferenciaEntero);
                    FRotator NuevaRot = RotacionInicial + DiferenciaRotacion;
                    //SetActorRelativeLocation(FVector(PosicionObjetivo.X, EnteroActual + FMath::CeilToInt(DiferenciaEntero), PosicionObjetivo.Z));//ese mas uno deberia ser en funcion de cuanto sea la diferencia, podria apelar al techo
                    SetActorRelativeRotation(NuevaRot);
                    Widget->SetWorldRotation(RotacionInicialWidget);
                }
                else {//la deifernica es menor a 0, por lo tanto debo ir hacia abajo, reducir el entero actual
                    RotacionTemp = FMath::FloorToInt(DiferenciaEntero);
                    DiferenciaRotacion.Roll = FMath::FloorToInt(DiferenciaEntero);
                    FRotator NuevaRot = RotacionInicial + DiferenciaRotacion;
                    //SetActorRelativeLocation(FVector(PosicionObjetivo.X, EnteroActual + FMath::CeilToInt(DiferenciaEntero), PosicionObjetivo.Z));//ese mas uno deberia ser en funcion de cuanto sea la diferencia, podria apelar al techo
                    SetActorRelativeRotation(NuevaRot);
                    Widget->SetWorldRotation(RotacionInicialWidget);
                }
            }
        }
        else {
            float DiferenciaEntero = DiferenciaRotacion.Roll - EnteroActual;//el entero actual es como la posicion inicial
            if (FMath::Abs(DiferenciaEntero) >= UmbralEntero) {
                if (DiferenciaEntero >= 0.0f) {//osa es un  numero positivo
                    RotacionTemp = FMath::CeilToInt(DiferenciaEntero);
                    DiferenciaRotacion.Roll = FMath::CeilToInt(DiferenciaEntero);
                    FRotator NuevaRot = RotacionInicial + DiferenciaRotacion;
                    //SetActorRelativeLocation(FVector(PosicionObjetivo.X, EnteroActual + FMath::CeilToInt(DiferenciaEntero), PosicionObjetivo.Z));//ese mas uno deberia ser en funcion de cuanto sea la diferencia, podria apelar al techo
                    SetActorRelativeRotation(NuevaRot);
                    Widget->SetWorldRotation(RotacionInicialWidget);
                }
                else {//la deifernica es menor a 0, por lo tanto debo ir hacia abajo, reducir el entero actual
                    RotacionTemp = FMath::FloorToInt(DiferenciaEntero);
                    DiferenciaRotacion.Roll = FMath::FloorToInt(DiferenciaEntero);
                    FRotator NuevaRot = RotacionInicial + DiferenciaRotacion;
                    //SetActorRelativeLocation(FVector(PosicionObjetivo.X, EnteroActual + FMath::CeilToInt(DiferenciaEntero), PosicionObjetivo.Z));//ese mas uno deberia ser en funcion de cuanto sea la diferencia, podria apelar al techo
                    SetActorRelativeRotation(NuevaRot);
                    Widget->SetWorldRotation(RotacionInicialWidget);
                }
            }
        }
    }
}

void APuertita::WidgetSeguir() {
    TWidget->HabilitarEje(ETransformacionEje::EEjeX);
    TWidget->HabilitarEje(ETransformacionEje::EEjeZ);
}

void APuertita::WidgetNoSeguir() {
    TWidget->DeshabilitarEje(ETransformacionEje::EEjeX);
    TWidget->DeshabilitarEje(ETransformacionEje::EEjeZ);
}



