

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "CaedeBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CAEDE_API UCaedeBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "Caede HUD", meta = (FriendlyName = "Get Caede HUD"))
	static class ACaedeHUD* GetRPGHUD(const APlayerController* PlayerController, bool& IsValid);
	/*
		UFUNCTION(BlueprintCallable, Category = "World Time", meta = (FriendlyName = "Get World Time"))
		static class AWorldTime* GetWorldTime(bool& IsValid);*/

	UFUNCTION(BlueprintCallable, Category = "NPC View", meta = (FriendlyName = "Is In View Angle"))
	static bool IsInViewAngle(const FVector& Start, const FVector& ViewDirection, 
		float HalfAngle, float ViewLength, const FVector& CheckLocation);

	//////////////////////////////////////////////////////////////////////////

	static FCanvasTextItem* DrawTextToCanvas_S(class UCanvas* canvas, FVector2D position, float& scale,
		UFont* font, FText text, FColor& color, bool shadows = false, FLinearColor shadowColor = FLinearColor::Black,
		bool outline = false, FLinearColor outlineColor = FLinearColor::White);

	static FCanvasTileItem DrawTileToCanvas_S(class UCanvas* canvas, FVector2D position, FVector2D pixelSize, const FColor& color, float scale = 1.0f);

	/** FontRenderInfo enabling casting shadows */
	static FFontRenderInfo ShadowedFont;
};
