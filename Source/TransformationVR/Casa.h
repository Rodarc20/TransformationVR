// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRPawn.h"
#include "Bloque.h"
#include "Components/BoxComponent.h"
#include "TransformacionWidget.h"
#include "PilaOpenGL.h"
#include "Casa.generated.h"

UCLASS()
class TRANSFORMATIONVR_API ACasa : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACasa();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    TSubclassOf<class APilaOpenGL> TipoPila;//esto no es practio llenarlo en el cosntructor, cuando esta clase pase a bluprint sera mejor

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
	APilaOpenGL * PilaCodigo;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    TArray<ABloque *> Bloques;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    EVRCasaTask LastCasaTask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    EVRCasaTask CurrentCasaTask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    EVRCasaTask OldCasaTask;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    UBoxComponent * Zona;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    FVector PosicionGlobalZona;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    FRotator RotacionGlobalZona;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    FVector EscalaGlobalZona;

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
    bool CasaArmada();

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
    void SetCasaTask(EVRCasaTask NewCasaTask);

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
    EVRCasaTask GetCasaTask();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR - Referencias")
    AVRPawn * Usuario;

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
    void Flotar();

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
    void Aterrizar();

    UFUNCTION()
    void OnBeginOverlapZona(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

    UFUNCTION()
	void OnEndOverlapZona(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    bool bMoviendo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    float Velocidad;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    FVector PosicionObjetivo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    FVector PosicionDescanso;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    FVector PosicionFlotando;
    //debo cambiar esto si redistribuyo la habitacion

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
    void PlayTaskTick();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    ETransformacionTarea CurrentTransformacionTarea;

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
    void SetTransformacionTarea(ETransformacionTarea NewTransformacionTarea);

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
    ETransformacionTarea GetTransformacionTarea();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    ETransformacionEje CurrentTransformacionEje;

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
    void SetTransformacionEje(ETransformacionEje NewTransformacionEje);

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
    ETransformacionEje GetTransformacionEje();
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    float ValorAplicar;//sera usado para aplicar traslacion rotacion, esclala, este es el valor modificado por el pad;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    FVector PosicionInicial;//este sera el mismo que el de posicion flotar

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    FRotator RotacionInicial;//la rotacion inical siempre sera 0, 0, 0

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    FVector EscalaInicial;// la escala inical siempre sera 1, 1, 1
    
    //de pronto no tiene mucho sentido conseravar esos valores, pero dejemoslo asi

};
