

#include "Caede.h"
#include "Util/CaedeBlueprintFunctionLibrary.h"
#include "HUD/CaedeHUD.h"
#include "WorldTime.h"


UCaedeBlueprintFunctionLibrary::UCaedeBlueprintFunctionLibrary(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

	ShadowedFont.bEnableShadow = true;
}

ACaedeHUD* UCaedeBlueprintFunctionLibrary::GetRPGHUD(const APlayerController* PlayerController, bool& IsValid)
{
	if (!PlayerController) return false;

	AHUD* HUD = PlayerController->GetHUD();
	if (!HUD) IsValid = false;

	ACaedeHUD* caedeHUD = Cast<ACaedeHUD>(HUD);
	if (!caedeHUD) IsValid = false;

	IsValid = true;

	return caedeHUD;
}

FCanvasTextItem* UCaedeBlueprintFunctionLibrary::DrawTextToCanvas_S(class UCanvas* canvas, FVector2D position, float& scale, UFont* font, FText text,
	FColor& color, bool shadows /*= false*/, FLinearColor shadowColor /*= FLinearColor::Black*/, 
	bool outline /*= false*/, FLinearColor outlineColor /*= FLinearColor::White*/)
{
	float ScaleUI = canvas->ClipY / 1080.0f;

	FCanvasTextItem* TextItem = new FCanvasTextItem(position, text, font, color);
	TextItem->Scale = FVector2D(scale, scale);
	if (shadows)
	{
		TextItem->FontRenderInfo = ShadowedFont;
		TextItem->EnableShadow(shadowColor, FVector2D::ZeroVector);
	}
	TextItem->bOutlined = outline;
	TextItem->OutlineColor = outlineColor;
	canvas->DrawItem(*TextItem);
	return TextItem;
}

FCanvasTileItem UCaedeBlueprintFunctionLibrary::DrawTileToCanvas_S(class UCanvas* canvas, FVector2D position, FVector2D pixelSize, const FColor& color, float scale /*= 1.0f*/)
{
	float ScaleUI = canvas->ClipY / 1080.0f;
	FCanvasTileItem Tile(position, pixelSize * scale * ScaleUI, color);
	canvas->DrawItem(Tile);
	return Tile;
}

bool UCaedeBlueprintFunctionLibrary::IsInViewAngle(const FVector& Start, const FVector& ViewDirection, float HalfAngle, float ViewLength, const FVector& CheckLocation)
{
	FVector TargetVector = CheckLocation - Start;
	TargetVector = FVector(TargetVector.X, TargetVector.Y, Start.Z);
	float distance = TargetVector.Size2D();
	if (distance > ViewLength)
	{
		return false;
	}

	FVector LeftAngle = FVector(ViewDirection);
	LeftAngle = LeftAngle.RotateAngleAxis(HalfAngle, FVector::UpVector);
	FVector RightAngle = FVector(ViewDirection);
	RightAngle = RightAngle.RotateAngleAxis(-HalfAngle, FVector::UpVector);

	FVector2D view = FVector2D(TargetVector);		// B
	FVector2D left2D = FVector2D(LeftAngle);		// C
	FVector2D right2D = FVector2D(RightAngle);		// A
	view.Normalize();
	left2D.Normalize();
	right2D.Normalize();

	float AxB = FVector2D::CrossProduct(right2D, view);
	float AxC = FVector2D::CrossProduct(right2D, left2D);
	float CxB = FVector2D::CrossProduct(left2D, view);
	float CxA = FVector2D::CrossProduct(left2D, right2D);
	
	// How it works:
	//
	// B --------------
	// C 
	// A \
	// |  \
	// |   \
	// |    \
	// |     \
	//
	// ------------------------------ 
	// Check if B is inside A and C
	//
	//		if (AxB * AxC >= 0 && CxB * CxA >= 0)
	//			=> true
	//

	if (AxB * AxC >= 0 && CxB * CxA >= 0)
	{
		return true;
	}
	return false;
}

FFontRenderInfo UCaedeBlueprintFunctionLibrary::ShadowedFont = FFontRenderInfo();
