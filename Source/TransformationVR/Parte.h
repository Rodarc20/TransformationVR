// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Public/Math/Color.h"
#include "TransformacionWidget.h"
#include "Parte.generated.h"

UCLASS()
class TRANSFORMATIONVR_API AParte : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParte();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AnimacionRotarTick(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	int Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	FString NombreParte;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * ParteMesh;

	//colision que envuelve la parte, esta deberia ser la raíz por ahora no lo es
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UCapsuleComponent * Colision;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	UTransformacionWidget * TWidget;

	//Puntero al padre en una jerarquía generada expresamente para el objeto
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	AParte * Padre;

	//Puntero a los hijos en una jerarquía generada expresamente para el objeto
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	TArray<AParte *> Hijos;

	//Array de los spherecomponentes de las articulaciones
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	TArray<USphereComponent *> ColisionesArticualciones;

	//Array de los spherecomponentes de las articulaciones
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	TArray<UStaticMeshComponent *> MeshesArticulaciones;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	AParte * OverlapedParte;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	bool bConectado;//verdadeor si la parte esta conectada al muñeco, se usa para saber si se esta sujetando una parte o al muñeco.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	int IdParteRaiz;//cual es la raiz a la que esta conectada esta parte, si es el mismo id que la parte, pues estoy sujetando la raiz del gurpo

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	bool bBuscarArticulacion;//verdadeor si la parte esta conectada al muñeco, se usa para saber si se esta sujetando una parte o al muñeco.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	bool bArticulacionSobrepuesta;//este se vuelve verdadero si cualquiera de las articulaciones se sobrepone, tener cuidado si se sobreponen varias por casualidad
	//este booleano se usa cuando la parte esta sujetada por el control derecho, la cua busca una articulacion apra unir en el muñeco

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	TArray<FLinearColor> ColoresArticulaciones;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	TArray<float> HueArticulaciones;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	FLinearColor ColorArticulacionNoConectada;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	FLinearColor ColorArticulacionSobrepuesta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	FLinearColor ColorArticulacionConectada;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	int IndiceArticulacionSobrepuesta;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	int IndiceArticulacionSobrepuestaOtro;
	
	UFUNCTION()
	int IndiceColisionArticulacion(USphereComponent * ArticulacionSphere);

	UFUNCTION()
	void BuscarArticulacion();

	UFUNCTION()
	void NoBuscarArticulacion();

	UFUNCTION()
	virtual void BuscandoArticulacion();

	UFUNCTION()
	virtual void UnirConParteSobrepuesta();

	UFUNCTION(Category = "Transformation")
	virtual void CambiarColorArticulacion(int IndiceArticulacion, FLinearColor NuevoColor);

    UFUNCTION()
    virtual void OnBeginOverlapArticulacion(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

    UFUNCTION()
    virtual void OnEndOverlapArticulacion(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation", Meta = (BlueprintPublic = "true"))
    UMaterialInstanceDynamic * ArticulacionMaterialDynamic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation", Meta = (BlueprintPublic = "true"))
	TArray<UMaterialInstanceDynamic *> ArticulacionesMaterialDynamic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation", Meta = (BlueprintPublic = "true"))
    UMaterialInstanceDynamic * ArticulacionUnidaMaterialDynamic;
	//cada iarticulacion deberia tener su porpia isntancia dinamica, o deberia usar dos materiales y cambiar entre ellos cuando lo necesite?

	FMatrix Frame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	bool bResaltada;

	UFUNCTION()
	virtual void ActivarResaltado();

	UFUNCTION()
	virtual void DesactivarResaltado();

	UFUNCTION()
	void ColorResaltadoArticulacion(int IdArticulacion);

	UFUNCTION()
	void ColorNormalArticulacion(int IdArticulacion);

	UFUNCTION()
	void ColorConectadoArticulacion(int IdArticulacion);

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void CopiarTransform();

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
    void AgregarRotacion(float angle, ETransformacionEje EjeRotacion);

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
    void ConfirmarRotacion(int IdRotacion);

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
    void CancelarRotacion(int IdRotacion);

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void AnimacionRotar(FVector CantidadRotacion);

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void EjecutarAnimaciones();

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
    void IniciarCicloAnimaciones();

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
    void DetenerCicloAnimaciones();

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	bool RotacionesConfirmadas();

	UFUNCTION(BlueprintCallable, Category = "Transformaciones")
	void CalcularVelociadaRotacion();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	bool bCicloAnimacion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	bool bAnimacion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    int IdRotacionActual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	bool bRotar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	float VelocidadRotacion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	float DuracionAnimacionRotacion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    FRotator RotacionInicial;

	float AngleXIni;//no tendria que usar nada el angulo fin puede determinar la velocidad de rotaionc, es decir eses seria la velocidad por segundo, entonces el delta de X seria al angulo fin *dletatime y listo
	float AngleXCurrent;
	float AngleXFin;
	int signoX;
	bool bRotarX;

	float AngleYIni;
	float AngleYCurrent;
	float AngleYFin;
	int signoY;
	bool bRotarY;

	float AngleZIni;
	float AngleZCurrent;
	float AngleZFin;
	int signoZ;
	bool bRotarZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	TArray<FVector> InstruccionesRotacion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	TArray<bool> InstruccionesRotacionConfirmado;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
	TArray<bool> InstruccionesRotacionValido;
};


//creo que sera mejor poner como centro un scene component, y al com hijos a colision los static mes etc, de esa forma no hay escalas