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
#include "Components/BoxComponent.h"
#include "PuntoTraslacion.h"
#include "Robot.generated.h"


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

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    UBoxComponent * Zona;

    UFUNCTION()
    void OnBeginOverlapZona(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

    UFUNCTION()
	void OnEndOverlapZona(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
    TArray<FVector> PuntosTraslacion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
    TArray<APuntoTraslacion * > PuntosTraslacionActors;
	
    UFUNCTION(BlueprintCallable, Category = "VRPawn")
    void CreatePuntoTraslacion(FVector PuntoSpawn);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
    FVector AlturaRobot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
    FVector PosicionGrabRobot;

    UFUNCTION(BlueprintCallable, Category = "VRPawn")
    void AnimacionTick(float DeltaTime);//para ejecutar controlar el movimiento en la iteracion del arreglo algo similar a las rotaciones en las partes

    UFUNCTION(BlueprintCallable, Category = "VRPawn")
    void IniciarAnimacion();//, ordena la ejecucion de las animaciones, es parecida a la de las rotaciones de las partes

    UFUNCTION(BlueprintCallable, Category = "VRPawn")
    void AnimacionTrasladar(FVector PuntoInicial, FVector IdPuntoFinal);//se movera dentro de los puntos de array, por lo tanto no necesito crear, solo usar estos indices

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
    FVector PuntoInicialTraslacion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
    FVector PuntoFinalTraslacion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
    bool bAnimacionTrasladar;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
    bool bTrasladar;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
    int IdPuntoTrasladoActual;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
    float VelocidadTraslacion;
    float Alpha;
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

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
    void VerificarRotaciones();



};
