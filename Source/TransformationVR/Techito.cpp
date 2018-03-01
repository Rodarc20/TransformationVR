// Fill out your copyright notice in the Description page of Project Settings.

#include "Techito.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Components/WidgetComponent.h"
#include "TransformacionWidget.h"
#include "Engine/StaticMesh.h"

ATechito::ATechito() {
	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    BloqueMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Techito"));
    RootComponent = BloqueMesh;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TechitoMeshAsset(TEXT("StaticMesh'/Game/Trasnformation/Assets/Meshes/Casita/Techito.Techito'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
    if (TechitoMeshAsset.Succeeded()) {
        BloqueMesh->SetStaticMesh(TechitoMeshAsset.Object);
        static ConstructorHelpers::FObjectFinder<UMaterial> TechitoMaterialAsset(TEXT("Material'/Game/Trasnformation/Assets/Meshes/Casita/Techo.Techo'"));//de usar este creo que debo crear un obtener un  material y ponerselo, este tiene el pivot en el centro de la esfera
        if (TechitoMaterialAsset.Succeeded()) {
            BloqueMesh->SetMaterial(0, TechitoMaterialAsset.Object);
        }
    }
    BloqueMesh->SetCollisionProfileName(FName(TEXT("Bloque")));
    Umbral = 2.0f;
    PosicionObjetivo = FVector(0.0f, 0.0f, 30.0f);

    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("WidgetBlueprintGeneratedClass'/Game/Trasnformation/UMG/WidgetTraslacion.WidgetTraslacion_C'"));
    Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetTraslacion"));
    Widget->SetupAttachment(RootComponent);
    Widget->SetWidgetSpace(EWidgetSpace::World);
    //Widget->SetupAttachment(MotionControllerLeft);
    Widget->SetDrawSize(FVector2D(1080.0f, 400.0f));
    Widget->SetPivot(FVector2D(0.5f, 0.5f));
    Widget->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
    Widget->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
    if (WidgetClass.Succeeded()) {
        Widget->SetWidgetClass(WidgetClass.Class);
    }
    Widget->SetVisibility(false);

	TWidget = CreateDefaultSubobject<UTransformacionWidget>(TEXT("TWidget"));
	TWidget->SetupAttachment(RootComponent);
	TWidget->SetRelativeLocation(FVector::ZeroVector);
    //TWidget->MostrarWidgetOrigen();
    TWidget->OcultarWidgetTraslacion();
}

void ATechito::BeginPlay() {
    Super::BeginPlay();
}

void ATechito::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    //UE_LOG(LogClass, Log, TEXT("siguiendo %d"), bSeguir);
    //if (bSobrepasoUmbral) {
        //SetActorLocation(ObjetoSeguir->GetComponentLocation());
    //}
    //else {
        if (bSeguir) {//si estoy siguiente, debo de habilitar o deshabilitar los widgets, esto va en funcion de cada parte, encapsular esta parte en funciones
            FVector PuntoSeguido = GetAttachParentActor()->GetActorTransform().InverseTransformPosition(ObjetoSeguir->GetComponentLocation());//de world a local
            float Diferencia = FMath::Abs(PuntoSeguido.Y - PosicionObjetivo.Y);
            if (Diferencia > Umbral){
                bSobrepasoUmbral = true;
            }
            else {
                bSobrepasoUmbral = false;
            }
            if (bSobrepasoUmbral) {//debo moverlo en el eje
                SetActorLocation(ObjetoSeguir->GetComponentLocation());
                SetActorRelativeLocation(FVector(PosicionObjetivo.X, PuntoSeguido.Y, PosicionObjetivo.Z));
                TraslacionTemp = PuntoSeguido.Y - PosicionInicial.Y;
                Widget->SetWorldLocation(PosicionInicialWorld);
                //debo verificar que este otra vz dentro del rango para regresar elsobrepaso el umbra a falso
                //a travez de esto puedo solucionar el problema de fisicas
            }
            else {//debo ponerlo en la posicion objetivo
                SetActorRelativeLocation(PosicionObjetivo);
                TraslacionTemp = PosicionObjetivo.Y - PosicionInicial.Y;
                Widget->SetWorldLocation(PosicionInicialWorld);
            }
            //UE_LOG(LogClass, Log, TEXT("siguiendo"));
        }
        /*if (bSeguir) {//si estoy siguiente, debo de habilitar o deshabilitar los widgets, esto va en funcion de cada parte, encapsular esta parte en funciones
            FVector PuntoSeguido = GetAttachParentActor()->GetActorTransform().InverseTransformPosition(ObjetoSeguir->GetComponentLocation());//de world a local
            FVector PuntoSeguidoSinY = PuntoSeguido;
            PuntoSeguidoSinY.Y = 0.0f;
            float Diferencia = (PuntoSeguidoSinY - PosicionObjetivo).Size();
            if (Diferencia > Umbral){
                bSobrepasoUmbral = true;
            }
            else {
                bSobrepasoUmbral = false;
            }
            if (bSobrepasoUmbral) {
                SetActorLocation(ObjetoSeguir->GetComponentLocation());
                //debo verificar que este otra vz dentro del rango para regresar elsobrepaso el umbra a falso
                //a travez de esto puedo solucionar el problema de fisicas
            }
            else {
                SetActorRelativeLocation(FVector(PosicionObjetivo.X, PuntoSeguido.Y, PosicionObjetivo.Z));
                TraslacionTemp = PuntoSeguido.Y - PosicionInicial.Y;
                Widget->SetWorldLocation(PosicionInicialWorld);
            }
            //UE_LOG(LogClass, Log, TEXT("siguiendo"));
        }*/
    //}
}

void ATechito::WidgetSeguir() {
    TWidget->HabilitarEje(ETransformacionEje::EEjeY);
}

void ATechito::WidgetNoSeguir() {
    TWidget->DeshabilitarEje(ETransformacionEje::EEjeY);
}
