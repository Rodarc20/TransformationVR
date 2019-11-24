// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TransformacionWidget.h"
#include "Materials/Material.h"
#include "Boton.generated.h"

UCLASS()
class TRANSFORMATIONVR_API ABoton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    ETransformacionTarea TareaAsociada;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * Boton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * MaterialNormal;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UMaterial * MaterialPresionado;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    bool bPressed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    bool bPressing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    bool bPosicionNormal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    bool bPosicionPresionado;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    bool bPosicionContacto;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    bool bPosicionFondo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    float AlturaNormal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    float AlturaPresionado;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    float AlturaContacto;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    float AlturaFondo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    float VelocidadNormal;

    UFUNCTION(BlueprintCallable, Category = "Transformation")
    void SendSignalPressed();

    UFUNCTION(BlueprintCallable, Category = "Transformation")
    void SendSignalReleased();

    UFUNCTION(BlueprintCallable, Category = "Transformation")
    void SendSignalActivated();

    UFUNCTION(BlueprintCallable, Category = "Transformation")
    void SendSignalDeactivated();

    UFUNCTION(BlueprintCallable, Category = "Transformation")
    void Presionado();

    UFUNCTION(BlueprintCallable, Category = "Transformation")
    void NoPresionado();
	
};

//this owner, cast to panel botones, llamar a la funcion pressed con el tag de este boton, el panel debera manejar lo que hace con esta informacion, asumo que sera llamar a la funcion casa
//esto es la funcionalidad general del boton , pero debeo crear clases derivadas, por que cada uno tiene statickmes distintos, aun que hacen los mismo
//las opciones de o los static mesh son relativas por lo tanto la funon es facil de predecir solo necesito los putnos y verificar que huan sea mayor en el eje z, y listo
//es decir tener el punto de staticmesh del boton, y un punto objeto, si este aun esta arriba no ha sido presionado, peude ser como el boton de los mecanicos, que siga decendiendo hasta el fondo, o de pronto no