// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "HoverUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAEDE_API UHoverUserWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Interaction")
	class UInteractionBaseWidget* InteractionWidget;
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interaction", meta = (FriendlyName = "Init Widget"))
	void InitWidget(AActor* hoverActor);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interaction", meta = (FriendlyName = "Cleanup Widget"))
	void CleanupWidget(AActor* hoverActor);
};
