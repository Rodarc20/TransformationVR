// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "TransformacionWidget.h"
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

    //usar felchas mas grandes
    /*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UTransformacionWidget * TWidget;*/

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * FlechaX;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * FlechaY;
		
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * FlechaZ;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * Origen;

    void MostrarWidget();

    void OcultarWidget();

    //esta clase sera interactiva, seran los botones, algo asi como el panel, busar una colision apropiadaa para ellos para que puedan ser seleccionados
    //que sea la misma de los ejes, para que se puedan seleccionar, lo mismo en lo que es trasnformacionwidget, ya que esos pequeños tambien se seleccionaran
    //en la tarea rotacion

    //tiene que ser con material instance, para poder cambiar los colores, y hacer el efecto de titilar, usar el de los nodos, o el de battery ctor para usarlo aqui

};
