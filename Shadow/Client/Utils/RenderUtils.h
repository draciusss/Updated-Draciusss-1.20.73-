#pragma once
#include "../../Base/SDK/Classes/Render/MinecraftUIRenderContext.h"

enum Type
{
	Internal = 0, // UserPackage
	External = 1, // Raw
};

class RenderUtils
{
public:
	static inline MinecraftUIRenderContext* renderContext = nullptr;

	static void Initialize(MinecraftUIRenderContext* RenderCtx) {
		static bool inited = false;
		if (inited) return;
		renderContext = RenderCtx;
		if (renderContext != nullptr) inited = true;
	};

	inline static void fillRectangle(Vector4<float> pos, UIColor col, float alpha) {
		renderContext->fillRectangle(Vector4<float>(pos.x, pos.z, pos.y, pos.w), col, alpha);
	}

	static void fillGradientRectangle(const Vector4<float>& rect, float alpha, int quality, int index) {
		int i = 0;
		for (float x = rect.x; x < rect.z; x += quality) {
			fillRectangle(Vector4<float>(x, rect.y, x + quality, rect.w), ColorUtils::RainbowDark(i * index), alpha);
			i++;
		}
	}

	static void renderImage(std::string filePath, Vector2<float> ImagePos, Vector2<float> ImageDimension, Type type = External)
	{
		renderContext->getTexture(new TexturePtr(), new ResourceLocation(type, filePath));
		renderContext->drawImage(new TexturePtr(), &ImagePos, &ImageDimension, Vector2<float>(0.f, 0.f), Vector2<float>(1.f, 1.f));
	}
};