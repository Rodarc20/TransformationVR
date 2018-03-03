// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "TransformacionWidget.h"
#include "Casa.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "Escena.generated.h"

UCLASS()
class TRANSFORMATIONVR_API AEscena : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEscena();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

    //usar felchas mas grandes
    /*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UTransformacionWidget * TWidget;*/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    UCameraComponent * CamaraSeguir;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * FlechaX;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * FlechaY;
		
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * FlechaZ;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * FlechaXNegative;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * FlechaYNegative;
		
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * FlechaZNegative;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * Origen;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformacion")
    UWidgetComponent * LetraX;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformacion")
    UWidgetComponent * LetraY;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformacion")
    UWidgetComponent * LetraZ;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformacion")
    UWidgetComponent * LetraXN;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformacion")
    UWidgetComponent * LetraYN;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformacion")
    UWidgetComponent * LetraZN;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeXNormal;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeXSelected;//sera el resaltado

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeXTitilando;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeXPresionado;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeXInhabilitado;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeYNormal;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeYSelected;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeYTitilando;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeYPresionado;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeYInhabilitado;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeZNormal;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeZSelected;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeZTitilando;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeZPresionado;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * EjeZInhabilitado;

    void NormalizarEje(ETransformacionEje EjeNormalizado);

    void SeleccionarEje(ETransformacionEje EjeSeleccionado);

    void PresionarEje(ETransformacionEje EjePresionado);

    void TitilarEje(ETransformacionEje EjeTitilado);

    void InhabilitarEje(ETransformacionEje EjeInhabilitado);

    void SeleccionEje(ETransformacionEje EjeSeleccionar);//funcion que realizar toda las operaciones necesarias al seleccionar un eje

    void MostrarEjeNegativo(ETransformacionEje EjeNegativo);//funcion que realizar toda las operaciones necesarias al seleccionar un eje

    void OcultarEjeNegativo(ETransformacionEje EjeNegativo);//funcion que realizar toda las operaciones necesarias al seleccionar un eje

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    ACasa * Casa;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    TArray<ETransformacionEje> OverlapedLeftEjes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    TArray<ETransformacionEje> OverlapedRightEjes;

    UFUNCTION(BlueprintCallable, Category = "Transformation")
    void GrabRightPressed();

    UFUNCTION(BlueprintCallable, Category = "Transformation")
    void GrabRightTick();

    UFUNCTION(BlueprintCallable, Category = "Transformation")
    void GrabRightReleased();

    UFUNCTION(BlueprintCallable, Category = "Transformation")
    void GrabLeftPressed();

    UFUNCTION(BlueprintCallable, Category = "Transformation")
    void GrabLeftTick();

    UFUNCTION(BlueprintCallable, Category = "Transformation")
    void GrabLeftReleased();

    void MostrarWidget();

    void OcultarWidget();

    UFUNCTION()
    void OnBeginOverlapEjeX(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

    UFUNCTION()
    void OnBeginOverlapEjeY(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

    UFUNCTION()
    void OnBeginOverlapEjeZ(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

    UFUNCTION()
	void OnEndOverlapEjeX(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
	void OnEndOverlapEjeY(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
	void OnEndOverlapEjeZ(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);
    //esta clase sera interactiva, seran los botones, algo asi como el panel, busar una colision apropiadaa para ellos para que puedan ser seleccionados
    //que sea la misma de los ejes, para que se puedan seleccionar, lo mismo en lo que es trasnformacionwidget, ya que esos pequeños tambien se seleccionaran
    //en la tarea rotacion

    //tiene que ser con material instance, para poder cambiar los colores, y hacer el efecto de titilar, usar el de los nodos, o el de battery ctor para usarlo aqui

};
