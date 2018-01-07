// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Parte.h"
#include "Jerarquia.h"
#include "TransformationVRGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TRANSFORMATIONVR_API ATransformationVRGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    ATransformationVRGameModeBase();
	
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "", Meta = (BlueprintProtected = "true"))//significa que solo se puede cambiar en el constructor
    //TSubclassOf<class UUserWidget> HUDWidgetClass;  
	
    //UUserWidget * WidgetActual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
	AJerarquia * Jerarquia;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformationVR")
    TArray<AParte *> Partes;//se supone que hay 10 partes
	
	
	
};

//esta clase deberia controlar lo que hace le muñeco, y tambien ser un intermediario entre el pawn y el muñeco y la jerarquia del arbol
//por ahora lo esta manjeando tod vr pawn, pero deberia tener una clase que maneje al muñeco pqero que instancie procese las partedel muñeco en el espacio global
//tambiien necesito una clase para la jaerarqui, que tendra asociada al muñeco y sus partes
//entonces cuando el usuario con el control izquierdo encuentra una parte, si pertenece al muñeco mueve al muñeco, pero si la aprte no esta uinida, se mueve la parte en busca de un muñeco
//con esto consigo que cualquiera de las dos manos pueda unir partes o sostener al muñeco