// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Parte.h"
#include "Jerarquia.h"
#include "PilaOpenGL.h"
#include "VRPawn.h"
#include "TransformationVRGameModeBase.generated.h"

/**
 * 
 */
/*UENUM(BlueprintType)
enum class EVRJerarquiaTask : uint8 {
    ETraslationTask UMETA(DisplayName = "Trasladar"),
    ERotationTask UMETA(DisplayName = "Rotar"),
    EScaleTask UMETA(DisplayName = "Escalar"),
    EArmarTask UMETA(DisplayName = "Armar"),
    EPlayTask UMETA(DisplayName = "Play"),
    ENoTask UMETA(DisplayName = "Ninguno")
};*/

UCLASS()
class TRANSFORMATIONVR_API ATransformationVRGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    ATransformationVRGameModeBase();
	
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "", Meta = (BlueprintProtected = "true"))//significa que solo se puede cambiar en el constructor
    //TSubclassOf<class UUserWidget> HUDWidgetClass;  
	
    //UUserWidget * WidgetActual;
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
	AJerarquia * Jerarquia;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jerarquia")
    TSubclassOf<class APilaOpenGL> TipoPila;//esto no es practio llenarlo en el cosntructor, cuando esta clase pase a bluprint sera mejor

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
	APilaOpenGL * PilaCodigo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    TArray<AParte *> Partes;//se supone que hay 10 partes
	
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
    AVRPawn * Usuario;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
    UMotionControllerComponent* RightController;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
    UMotionControllerComponent* LeftController;
	
	UFUNCTION(BlueprintCallable, Category = "TransformationVR")
	FVector BuscarParte(AParte *& ParteEncontrada);

	UFUNCTION(BlueprintCallable, Category = "TransformationVR")
	void BuscandoParteConLaser();

	void BuscandoComponenteRotacionConLaser();

	bool ColisionRotacion(FVector Inicio, FVector Fin, ETransformacionEje & EjeColisionado, FVector & Impact);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
	AParte * ParteSeleccionada;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
	AParte * HitParte;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
	bool bHitParte;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
	float DistanciaLaserMaxima;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
	FVector ImpactPoint;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    EVRJerarquiaTask CurrentJerarquiaTask;
	
    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
    void SetJerarquiaTask(EVRJerarquiaTask NewJerarquiaTask);

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
    EVRJerarquiaTask GetJerarquiaTask();

	void SelectPressed();
*/
};

//esta clase deberia controlar lo que hace le muñeco, y tambien ser un intermediario entre el pawn y el muñeco y la jerarquia del arbol
//por ahora lo esta manjeando tod vr pawn, pero deberia tener una clase que maneje al muñeco pqero que instancie procese las partedel muñeco en el espacio global
//tambiien necesito una clase para la jaerarqui, que tendra asociada al muñeco y sus partes
//entonces cuando el usuario con el control izquierdo encuentra una parte, si pertenece al muñeco mueve al muñeco, pero si la aprte no esta uinida, se mueve la parte en busca de un muñeco
//con esto consigo que cualquiera de las dos manos pueda unir partes o sostener al muñeco