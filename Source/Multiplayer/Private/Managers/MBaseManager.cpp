// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/MBaseManager.h"

// Sets default values
AMBaseManager::AMBaseManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMBaseManager::BeginPlay()
{
	Super::BeginPlay();
}

void AMBaseManager::SetFinishBind()
{
	bFinish = true;
}

bool AMBaseManager::IsFinishBind()
{
	return bFinish;
}

