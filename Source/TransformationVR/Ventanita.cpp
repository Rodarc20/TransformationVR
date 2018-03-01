// Fill out your copyright notice in the Description page of Project Settings.

#include "Ventanita.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Engine/StaticMesh.h"

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
    Widget->SetDrawSize(FVector2D(1080.0f, 400.0f));
    Widget->SetPivot(FVector2D(0.5f, 0.5f));
    Widget->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    Widget->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
    Widget->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
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

    DimensionReal = 10.0f;
    DimensionActual = 5.0f;
    DimensionInicial = 5.0f;

    Umbral = 2.0f;
    //el objetio es que la  dimension actual llegue a su dimension real
}

void AVentanita::BeginPlay() {
    Super::BeginPlay();
}

void AVentanita::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    if (bSeguir && ObjetoSeguirFinal) {
        DistanciaObjetos = (ObjetoSeguirFinal->GetComponentLocation() - ObjetoSeguir->GetComponentLocation()).Size();
        EscalaTemp = (DistanciaObjetos - DistanciaInicialObjetos) / DistanciaInicialObjetos;
        UE_LOG(LogClass, Log, TEXT("Escala Temp: %f"), EscalaTemp);
        DimensionActual = EscalaTemp * DimensionInicial;
        //Escala temp es la escala que se aplica
        //pero para aplicarla se deberia multiplicar ed alguna forma con la escala de trasformacion, me explico
        //para aumentar al doble de tamaño, escala temp deberia llegar a valer 2, entonces la dimesionacutal deberia ser 2 * dimensioninicial
        //entonces la dimension seria 20
        //si yo quiero aumentar, y consigo de nuevo un valor de 2, la dimensioninicial seria 20, entonces la dimension actual seria 2 * 20 osea 40
        //este 40, debo taducirlo a una escala para unreal, la dimensio real es 10, por lo tanto, la escala es 40 /10 osea 40.
        //es curios que la escala es la multiplicacion de las otras dos, osea 2 * 2, 
        
        //calcular la escala temporal y aplicar
        //aplicarlao, conservar el valor de escala actual para saber cual es la escala que tiene en este momenot,
        //problemas con la escala relativa
        //la escala se aplcara solo a uno de los ejes, en este caso el eje y tal vez o eje x

        //EscalaReal = DimensionActual / DimensionReal;
        //SetActorRelativeScale3D(FVector(EscalaReal, 1.0f, 1.0f));
        //Widget->SetWorldScale3D(EscalaInicialWidget);

        float Diferencia = FMath::Abs(DimensionActual - DimensionReal);
        if (Diferencia > Umbral){
            bSobrepasoUmbral = true;
        }
        else {
            bSobrepasoUmbral = false;
        }
        if (bSobrepasoUmbral) {//debo moverlo en el eje
            EscalaReal = DimensionActual / DimensionReal;
            SetActorRelativeScale3D(FVector(EscalaReal, 1.0f, 1.0f));
            //SetActorRelativeScale3D(FVector(EscalaTemp, 1.0f, 1.0f));
            Widget->SetWorldScale3D(EscalaInicialWidget);
        }
        else {//debo ponerlo en la posicion objetivo
            //falta calcular el valor de la escala temp
            SetActorRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
            //SetActorRelativeScale3D(FVector(EscalaTemp, 1.0f, 1.0f));
            Widget->SetWorldScale3D(EscalaInicialWidget);
        }
    }
}

void AVentanita::WidgetSeguir() {
    TWidget->HabilitarEje(ETransformacionEje::EEjeX);
    TWidget->HabilitarEje(ETransformacionEje::EEjeZ);
}

void AVentanita::WidgetNoSeguir() {
    TWidget->DeshabilitarEje(ETransformacionEje::EEjeX);
    TWidget->DeshabilitarEje(ETransformacionEje::EEjeZ);
}


