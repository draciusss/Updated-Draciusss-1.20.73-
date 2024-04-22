#pragma once

class RiseGUI {
public:
	void render(float animation, float inScale, int& scrollDirection) noexcept {
		const float guiWidth = 360.f;
		const float guiHeight = 280.f;

		const float catOffset = 200.f;

		const Vector2<float> guiPos = Vector2<float>(ImRenderUtil::getScreenSize().x / 2, ImRenderUtil::getScreenSize().y / 2);

		Vector4<float> baseRect = Vector4<float>(guiPos.x - guiWidth, guiPos.y - guiHeight, guiPos.x + guiWidth, guiPos.y + guiHeight);
		Vector4<float> catRect = Vector4<float>(baseRect.x, baseRect.y, baseRect.x + catOffset, baseRect.w);
		Vector4<float> mainRect = Vector4<float>(baseRect.x + catOffset, baseRect.y, baseRect.z, baseRect.w);

		static float scrollEased = 0;
		static float scroll = 0;
		bool isEnabled = getModuleByName("clickgui")->isEnabled();
		scrollEased = Math::animate(scroll, scrollEased, 0.016f * 10.f);

		if (ImRenderUtil::isMouseOver(mainRect)) {
			scroll += scrollDirection * 20;

			if (scrollDirection < 0 && scroll < 0)
				scroll = 0;

			scrollDirection = 0;
		}

		std::string ClientName = "Shadow";

		static std::string currentCategory = "Visual";
		static std::vector<Module*> modList = ClickGUIManager::getModulesInCategory(currentCategory, modules);

		ImRenderUtil::fillRectangle(catRect, UIColor(18, 20, 25), animation * 0.98, 0);
		ImRenderUtil::fillRectangle(mainRect, UIColor(23, 26, 33), animation, 0);

		ImRenderUtil::fillShadowCircle(Vector2<float>(catRect.x + 25.f + ImRenderUtil::getTextWidth(&ClientName, 2.5f) / 2, catRect.y + 15.f + ImRenderUtil::getTextHeight(2.5) / 2), 1.f, UIColor(255, 255, 255), animation * 0.8F, 200, 0);
		ImRenderUtil::drawText(Vector2<float>(catRect.x + 25.f, catRect.y + 15.f), &ClientName, UIColor(255, 255, 255), 2.5f, animation, true);
		std::string vers = "0.5";
		ImRenderUtil::drawText(Vector2<float>(catRect.x + 25.f + ImRenderUtil::getTextWidth(&ClientName, 2.5f), catRect.y + 14.f), &vers, ColorUtils::Rainbow(5, 1, 1, 0), 1.f, animation, true);

		int ind = 0;
		std::string symbols[] = { "a", "b", "c", "d", "e", "f" };

		for (std::string category : categories) {
			Vector2<float> textPosition(catRect.x + 25.f, catRect.y + 90.f + ind);

			Vector4<float> catTab(textPosition.x - 5.f, textPosition.y - 5, textPosition.x - 5.f + ImRenderUtil::getTextWidth(&category, 1.f) + 12.5f + 25.f, textPosition.y + ImRenderUtil::getTextHeight(1.f) + 5);

			if (ImRenderUtil::isMouseOver(catTab) && Utils::leftClick) {
				Utils::leftClick = false;
				currentCategory = category;
				modList.clear();
				scroll = 0;
				modList = ClickGUIManager::getModulesInCategory(currentCategory, modules);
			}

			if (category == currentCategory) {
				ImRenderUtil::fillShadowRectangle(catTab.scale(2), ColorUtils::Rainbow(5, 1, 1, 0), animation * 0.5f, 40.f, 0, 10);
				ImRenderUtil::fillRectangle(catTab, ColorUtils::Rainbow(5, 1, 1, 0), animation, 8.f);
			}
			int symbolIndex = std::distance(categories.begin(), std::find(categories.begin(), categories.end(), category));

			if (symbolIndex >= 0 && symbolIndex < sizeof(symbols) / sizeof(symbols[0])) {
				std::string symbol = symbols[symbolIndex];

				ImRenderUtil::drawText(Vector2<float>(textPosition.x, textPosition.y), &symbol, UIColor(255, 255, 255), 1.f, animation, true, 1);
			}
			ImRenderUtil::drawText(Vector2<float>(textPosition.x + 25.f, textPosition.y), &category, UIColor(255, 255, 255), 1.f, animation, true);
			ind += (ImRenderUtil::getTextHeight(1.2) + 20);
		}
		float modInd = -scrollEased;

		Vector4<float> clipRect = Vector4<float>(baseRect.x, baseRect.y, baseRect.z, baseRect.w).scaleToCenter(inScale);
		//ImRect clip_rect(clipRect.x, clipRect.y, clipRect.z, clipRect.w);
		ImDrawList* list = ImGui::GetBackgroundDrawList();
		//list->PushClipRect(clip_rect.Min, clip_rect.Max, true);
		for (auto mod : modList) {
			UIColor rgb = ColorUtils::Rainbow(5, 1, 1, modInd * 2);
			Vector2<float> modulePosition(mainRect.x + 15.f, mainRect.y + 10.f + modInd);
			Vector4<float> moduleRect(modulePosition.x, modulePosition.y, mainRect.z - 15.f, modulePosition.y + 65.f);

			ImRenderUtil::fillRectangle(moduleRect, UIColor(18, 20, 25), animation * 0.5f, 10);

			std::string modName = mod->getName();
			std::string desc = mod->getDescription();

			// Calculate the text positions
			Vector2<float> modNamePos = Vector2<float>(moduleRect.x + 10.f, moduleRect.y + 8.f);
			Vector2<float> descPos = Vector2<float>(moduleRect.x + 10.f, moduleRect.y + 38.f);

			if (ImRenderUtil::isMouseOver(moduleRect) && isEnabled) {
				if (Utils::leftClick) {
					mod->toggle();
					Utils::leftClick = false;
				}
				else if (Utils::rightClick) {
					mod->showSettings = !mod->showSettings;
					Utils::rightClick = false;
				}
			}

			// Draw module name
			ImRenderUtil::drawText(modNamePos, &modName, mod->isEnabled() ? rgb : UIColor(255, 255, 255), 1.4f, animation, true);

			// Draw module description
			ImRenderUtil::drawText(descPos, &desc, UIColor(160, 160, 160), 1.f, animation, true);

			modInd += 75.f;
		}
		//list->PopClipRect();

		//ImScaleEnd(inScale, inScale, ImVec2(baseRect.getCenter().x, baseRect.getCenter().y));
		if (isEnabled) {
			Global::getClientInstance()->grabMouse();
		}
	}
};
