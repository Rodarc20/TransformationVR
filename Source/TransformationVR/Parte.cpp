// Fill out your copyright notice in the Description page of Project Settings.

#include "Parte.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include "Public/Math/Color.h"



// Sets default values
AParte::AParte()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColorArticulacionNoConectada = FLinearColor(0.0f, 0.08336f, 0.234375f, 0.0f);
	ColorArticulacionSobrepuesta = FLinearColor(0.0f, 0.234375f, 0.097833f, 0.0f);
	//ColorArticulacionConectada = FLinearColor(0.0f, 0.234375, 0.212609f, 0.0f);
	ColorArticulacionConectada = FLinearColor(0.234375f, 0.0f, 0.002142f, 0.0f);

}

// Called when the game starts or when spawned
void AParte::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParte::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int AParte::IndiceColisionArticulacion(USphereComponent * ArticulacionSphere) {
	int Index;
	if (ColisionesArticualciones.Find(ArticulacionSphere, Index)) {
		return Index;
	}
	return -1;
}

void AParte::BuscarArticulacion() {
	bBuscarArticulacion = true;
}

void AParte::NoBuscarArticulacion() {
	bBuscarArticulacion = false;
}

void AParte::BuscandoArticulacion() {
	//creo que esta funcion esta demas, ya que la busqueda estara controlada por la funcion everlaping, de cada articulacion, que reaccionara siempre solo si el bool de buscar articulacion es verdadero
	//si al final no llego a usar esta funcion, la debo eliminar
}

void AParte::UnirConParteSobrepuesta() {
	if (bArticulacionSobrepuesta) {//si hay una articulacion sobrepuesta entoces si o si habra un overlaperparte, tener en cuenta que esta funcion deberia manerja los caso de articulaciones con varias partes por ahora dejar sencillo
		Padre = OverlapedParte;
		OverlapedParte->Hijos.Add(this);
		//aqui deberia haber mas partes de color
		bConectado = true;
		if(IndiceArticulacionSobrepuesta != -1)
			CambiarColorArticulacion(IndiceArticulacionSobrepuesta, ColorArticulacionConectada);
		//tambien se deben inhabilitar los de alguna forma estas articulaciones, dejarlo asi por ahora
	}
}

void AParte::CambiarColorArticulacion(int IndiceArticulacion, FLinearColor NuevoColor) {
}

void AParte::OnBeginOverlapArticulacion(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	if (bBuscarArticulacion) {
		AParte * const ParteEncontrada = Cast<AParte>(OtherActor);
		if (ParteEncontrada) {
			USphereComponent * const ArticulacionEncontrada = Cast<USphereComponent>(OtherComp);//en realidad deberia manejar canles para que esto solo se active con los spherecomponents ocnfigurados como articulacion
			if (ArticulacionEncontrada) {
				//debo cambiar el color de esta articulacion y de la otra, parte para ello debo darle, el componente encontrado, y que lo busque en su array de sphere components, el indice me dira cual es su mesh corresondiente para cambiarlo de color
				bArticulacionSobrepuesta = true;
				OverlapedParte = ParteEncontrada;

				if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Articulacion Sobrepuesta"));
				//asi mismo debo saber cual de todas mis articulaciones es la que esta en contacto, para ello esta overlappedcomponent, que debo guardarlo despues de castear para luego buscar y saber su inidice para guardar el puntero de la parte padre, e inhabilitar en ese componente el contacto
				//debo deshabilitar para que en una nueva busqueda de alguna otra parte, no tome en cuenta las rticualciones unidas
				int IndiceArticulacionSobrepuesta = IndiceColisionArticulacion(Cast<USphereComponent>(OverlappedComponent));// que busque mejor la parte en lugar de yo hacerlo aqui, por un lado reduzco busquedas si lo hago antes
				if(IndiceArticulacionSobrepuesta != -1)
					CambiarColorArticulacion(IndiceArticulacionSobrepuesta, ColorArticulacionSobrepuesta);
				int IndiceArticulacionSobrepuestaOtro = ParteEncontrada->IndiceColisionArticulacion(ArticulacionEncontrada);
				if(IndiceArticulacionSobrepuestaOtro != -1)
					ParteEncontrada->CambiarColorArticulacion(IndiceArticulacionSobrepuestaOtro, ColorArticulacionSobrepuesta);
			}
		}
	}
}

void AParte::OnEndOverlapArticulacion(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex) {
	if (bBuscarArticulacion) {
		AParte * const ParteEncontrada = Cast<AParte>(OtherActor);
		if (ParteEncontrada) {
			USphereComponent * const ArticulacionEncontrada = Cast<USphereComponent>(OtherComp);//en realidad deberia manejar canles para que esto solo se active con los spherecomponents ocnfigurados como articulacion
			if (ArticulacionEncontrada) {
				bArticulacionSobrepuesta = false;
				OverlapedParte = nullptr;
				if (GEngine)//no hacer esta verificación provocaba error al iniciar el editor
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Saliendo Articulacion Sobrepuesta"));
				IndiceArticulacionSobrepuesta = IndiceColisionArticulacion(Cast<USphereComponent>(OverlappedComponent));// que busque mejor la parte en lugar de yo hacerlo aqui, por un lado reduzco busquedas si lo hago antes
				if(IndiceArticulacionSobrepuesta != -1)
					CambiarColorArticulacion(IndiceArticulacionSobrepuesta, ColorArticulacionNoConectada);
				int IndiceArticulacionSobrepuestaOtro = ParteEncontrada->IndiceColisionArticulacion(ArticulacionEncontrada);
				if(IndiceArticulacionSobrepuestaOtro != -1)
					ParteEncontrada->CambiarColorArticulacion(IndiceArticulacionSobrepuestaOtro, ColorArticulacionNoConectada);
			}
		}
		//tambien aplicar los cambios de color adecuados, por ahora solo dejare mensajes en consola
	}
}

//creo que las articulaciones deberian ser un nuevo componente llamado articulacion, el cual tendria un pshere component y un statickmesh
//asi cuando este reaacione en el overlap, y deba cambiar el color por ejemplo, solo debo llamar a una funcion de este componente, lo mismo cuando usceda el endoverlap
