// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arista.generated.h"

UCLASS()
class TRANSFORMATIONVR_API AArista : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArista();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    float Distancia;//la distancia no puede ser menor que el radio, esta es por la capsula la cual sera el cllider

    //radio o grosor de la arista
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    float Radio;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TransformationVR")
    int SourceId;

    //id del nodo al que se dirige la arista, es decir el hijo
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visualization")
    int TargetId;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TransformationVR")
    USceneComponent * Source;

    //referencia al nodo hijo
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TransformationVR")
    USceneComponent * Target;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    UStaticMeshComponent * AristaMesh;//un capsule mesh es dificl de scalar de forma adecuada para que las semiesferas no se deformen

    UFUNCTION(BlueprintCallable, Category = "TransformationVR")
    void Actualizar();
	
};
