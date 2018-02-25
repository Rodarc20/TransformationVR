// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "TransformacionWidget.h"
#include "Bloque.generated.h"

UCLASS()
class TRANSFORMATIONVR_API ABloque : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABloque();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * BloqueMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    USceneComponent * ObjetoSeguir;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    USceneComponent * ObjetoSeguirFinal;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UTransformacionWidget * TWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    bool bArmado;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    bool bSeguir;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    bool bSobrepasoUmbral;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    float TraslacionTemp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    float RotacionTemp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    float EscalaTemp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    float Umbral;

    FVector DireccionMovimiento;//para poder restringir el movimiento
    //restringido a este eje de movimiento
    // si sobrepaso el umbral ya no respetara este eje de moviemiento

    void SeguirObjeto(USceneComponent * Objeto);

    void SeguirObjetos(USceneComponent * ObjetoInicial, USceneComponent * ObjetoFinal);

    void NoSeguir();
	
    FVector PosicionInicial;

    FVector PosicionInicialWorld;

    FRotator RotacionInicial;

    FRotator RotacionInicialWidget;

    FVector EscalaInicialWidget;

    FRotator RotacionInicialObjeto;

    float DistanciaInicialObjetos;

    float DistanciaObjetos;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformacion")
    UWidgetComponent * Widget;

	UFUNCTION(Category = "Transformation")
	virtual void WidgetSeguir();

	UFUNCTION(Category = "Transformation")
	virtual void WidgetNoSeguir();
};


//se spone que tendra un movimiento restringigo,
//un enfoque es solucionar
UENUM(BlueprintType)
enum class EVRCasaTask : uint8 {
    EArmarTask UMETA(DisplayName = "Armar"),
    EPlayTask UMETA(DisplayName = "Play"),
    ENoTask UMETA(DisplayName = "Ninguno")
};

