

#pragma once

#include "Util/CaedeBlueprintFunctionLibrary.h"

#define HUD_SIMPLE_BAR_HEIGHT 0.05f
#define HUD_SIMPLE_BAR_MAX_WIDTH 0.15f

//////////////////////////////////////////////////////////////////////////

struct FSimpleStatusBar
{
	FText Text;

	float CurrentWidthPercentage;

	float Width;

	float Height;

	FVector2D Position;

	FVector2D Size;

	FVector2D MaxSize;

	FCanvasTileItem* TileItem;
	FCanvasTileItem* BackroundItem;
	FCanvasTextItem* TextItem;

	FVector2D GetSize(class UCanvas* canvas)
	{
		FVector2D fullSize(canvas->ClipX * Width, canvas->ClipY * Height + TextItem->DrawnSize.Y);
		return fullSize;
	}

	FSimpleStatusBar(FVector2D position, FText text, float height = HUD_SIMPLE_BAR_HEIGHT, float maxWidth = HUD_SIMPLE_BAR_MAX_WIDTH)
	{
		Position = position;
		Height = height;
		Text = text;
		Width = maxWidth;
		CurrentWidthPercentage = 1.0f;
	}

	void Update(class UCanvas* canvas, float newPercentage)
	{
		CurrentWidthPercentage = newPercentage;
		MaxSize = FVector2D(canvas->ClipX * Width, canvas->ClipY * Height);
		Size = FVector2D(canvas->ClipX * GetPixelWidth(), canvas->ClipY * Height);
	}

	void Draw(class UCanvas* canvas, UFont* font, FColor& fontColor, const FColor& color, const FColor& bgColor, float scale = 1.0f, float textScale = 0.75f, bool centered = true)
	{
		float TextScale = textScale;
		float TextPaddingX = 0.002f;
		float TextPaddingY = 0.001f;
		const FString TestDesc = TEXT("Test");


		FVector2D DescSize(0.0f, 0.0f);
		canvas->StrLen(font, Text.ToString(), DescSize.X, DescSize.Y);

		FVector2D TilePosition(Position);

		if (centered)
		{
			FVector2D tileSize = MaxSize * scale;
			TilePosition.X -= tileSize.X * 0.5f;
			TilePosition.Y -= tileSize.Y * 0.5f;
		}

		TileItem = new FCanvasTileItem(TilePosition, Size * scale, color);
		TileItem->BlendMode = SE_BLEND_AlphaBlend;
		FVector2D BackroundPosition(TilePosition);
		BackroundPosition.X -= MaxSize.X * scale * 0.0075f;
		BackroundPosition.Y -= MaxSize.Y * scale * 0.3f;
		FVector2D BackroundScale(MaxSize * scale);
		BackroundScale.X *= 1.015f;
		BackroundScale.Y *= 1.6f;
		BackroundItem = new FCanvasTileItem(BackroundPosition, BackroundScale, bgColor);
		BackroundItem->BlendMode = SE_BLEND_AlphaBlend;
		canvas->DrawItem(*BackroundItem);
		canvas->DrawItem(*TileItem);
		FVector2D textPosition(TilePosition);
		textPosition.X += canvas->ClipX * TextPaddingX;
		textPosition.Y -= (canvas->ClipY * TextPaddingY + DescSize.Y * textScale);
		TextItem = UCaedeBlueprintFunctionLibrary::DrawTextToCanvas_S(canvas, textPosition, textScale, font, Text, fontColor, true, FLinearColor(fontColor));
	}

private:
	float GetPixelWidth()
	{
		return Width * CurrentWidthPercentage;
	}

};

