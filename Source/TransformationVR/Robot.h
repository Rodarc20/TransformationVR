// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Parte.h"
#include "TransformacionWidget.h"
#include "Jerarquia.h"
#include "PilaOpenGL.h"
#include "Transformacion.h"
#include "VRPawn.h"
#include "Robot.generated.h"

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
class TRANSFORMATIONVR_API ARobot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARobot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
	AJerarquia * Jerarquia;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
	TArray<AJerarquia *> Jerarquias;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
	TArray<ANodo *> Nodos;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    TSubclassOf<class ANodo> TipoNodo;//esto no es practio llenarlo en el cosntructor, cuando esta clase pase a bluprint sera mejor

	TArray<Transformacion *> Transformaciones;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
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
	ETransformacionEje HitEje;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
	ETransformacionEje EjeSeleccionado;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    EVRJerarquiaTask CurrentJerarquiaTask;
	
    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
    void SetJerarquiaTask(EVRJerarquiaTask NewJerarquiaTask);

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
    EVRJerarquiaTask GetJerarquiaTask();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
	FVector PuntoRotacionInicial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
	FRotator RotacionInicial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
	FVector PuntoInterseccion;

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
	void BuscarIntereseccionEjeRotacion();
	
	UFUNCTION(BlueprintCallable, Category = "TransformationVR")
	void RotarParteEnEje();

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
	void SelectPressed();

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
	void SelectReleased();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
	float AngleTemp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
	float AnguloAnterior;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
	float AnguloActual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
	float AnguloAcum;

	


    UFUNCTION(BlueprintCallable, Category = "VRPawn")
    void GrabRightPressed();

    UFUNCTION(BlueprintCallable, Category = "VRPawn")
    void GrabRightTick();

    UFUNCTION(BlueprintCallable, Category = "VRPawn")
    void GrabRightReleased();

    UFUNCTION(BlueprintCallable, Category = "VRPawn")
    void GrabLeftPressed();

    UFUNCTION(BlueprintCallable, Category = "VRPawn")
    void GrabLeftTick();

    UFUNCTION(BlueprintCallable, Category = "VRPawn")
    void GrabLeftReleased();

	UFUNCTION(BlueprintCallable, Category = "VRPawn")
	void UnirJerarquiaPadreHijo(int IdPadre, int IdHijo);



};
