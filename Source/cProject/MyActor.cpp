// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
//#include "Componets/StaticMeshComponent.h"
// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	//L"TEXT" - 특정 안드로이드에서만 되는경우가있으니 꼭붙여주기 TEXT
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

