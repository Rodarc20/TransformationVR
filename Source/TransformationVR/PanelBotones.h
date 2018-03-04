// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TransformacionWidget.h"
#include "Boton.h"
#include "PanelBotones.generated.h"

UCLASS()
class TRANSFORMATIONVR_API APanelBotones : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APanelBotones();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    ABoton * BotonTrasladar;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    ABoton * BotonRotar;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    ABoton * BotonEscalar;

    UFUNCTION(BlueprintCallable, Category = "Transformation")
    void Press(ETransformacionTarea BotonPresionado);

    UFUNCTION(BlueprintCallable, Category = "Transformation")
    void Release(ETransformacionTarea BotonPresionado);
	
};
