// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

    bool bSeguir;

    bool bSobrepasoUmbral;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    float Umbral;

    FVector DireccionMovimiento;//para poder restringir el movimiento
    //restringido a este eje de movimiento
    // si sobrepaso el umbral ya no respetara este eje de moviemiento

    void SeguirObjeto(USceneComponent * Objeto);

    void NoSeguir();
	
    FRotator RotacionInicial;
    FRotator RotacionInicialObjeto;
};


//se spone que tendra un movimiento restringigo,
//un enfoque es solucionar
UENUM(BlueprintType)
enum class EVRCasaTask : uint8 {
    EArmarTask UMETA(DisplayName = "Armar"),
    EPlayTask UMETA(DisplayName = "Play"),
    ENoTask UMETA(DisplayName = "Ninguno")
};

