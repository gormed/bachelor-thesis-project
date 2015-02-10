// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/InteractionComponent.h"
#include "InteractionBaseWidget.generated.h"

class AItem;
class ACaedeCharacter;
class ACaedeHUD;

/**
 * 
 */
UCLASS(BlueprintType)
class CAEDE_API UInteractionBaseWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Interaction")
	ACaedeCharacter* ReferencedCharacter;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Interaction")
	ACaedeHUD* PlayerHUD;

	UFUNCTION(BlueprintCallable, Category = "Interaction", meta = (FriendlyName = "Initialize Interaction Widget"))
	void InitWidget(ACaedeCharacter* character);

	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Interaction|Widgets")
	UHandItemUserWidget* HandItemWidget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Interaction|Widgets")
	UHoverUserWidget* HoverWidget;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction|Widgets", meta = (FriendlyName = "Create Hand Item Widget"))
	UHandItemUserWidget* CreateHandItemWidget();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction|Widgets", meta = (FriendlyName = "Create Hover Widget"))
	UHoverUserWidget* CreateHoverWidget();
	
	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction Events", meta = (FriendlyName = "On Item Selected"))
	void OnItemSelected(AItem* item, EHandSlot::Type handSlot);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction Events", meta = (FriendlyName = "On Item Unselected"))
	void OnItemUnselected(AItem* item, EHandSlot::Type handSlot);

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION()
	void OnItemSelected_Internal(AItem* item, TEnumAsByte<EHandSlot::Type> handSlot);

	UFUNCTION()
	void OnItemUnselected_Internal(AItem* item, TEnumAsByte<EHandSlot::Type> handSlot);

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction Events", meta = (FriendlyName = "On Begin Actor Hover"))
	void OnBeginActorHover(AActor* actor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction Events", meta = (FriendlyName = "On End Actor Hover"))
	void OnEndActorHover(AActor* actor);

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION()
	void OnBeginActorHover_Internal(AActor* actor);

	UFUNCTION()
	void OnEndActorHover_Internal(AActor* actor);
};
