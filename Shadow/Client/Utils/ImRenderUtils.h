#pragma once

class ImRenderUtil
{
public:
	inline static void drawText(Vector2<float> pos, std::string* textStr, const UIColor& color, float textSize, float alpha, bool shadow = false, int index = 0, ImDrawList* d = ImGui::GetBackgroundDrawList())
	{
		if (!ImGui::GetCurrentContext())
			return;
		ImFont* font = ImGui::GetFont();

		ImVec2 textPos = ImVec2(pos.x, pos.y);
		constexpr ImVec2 shadowOffset = ImVec2(1.f, 1.f);

		if (shadow)
		{
			d->AddText(font, (textSize * 18), textPos + shadowOffset, ImColor(color.r * 0.03f, color.g * 0.03f, color.b * 0.03f, alpha * 0.9f), textStr->c_str());
		}

		d->AddText(font, (textSize * 18), textPos, ImColor(color.r, color.g, color.b, alpha), textStr->c_str());
	};

	template <typename T>
	static void fillRectangle(Vector4<T> pos, const UIColor& color, float alpha, float radius = 0.f, ImDrawList* list = ImGui::GetBackgroundDrawList())
	{
		if (!ImGui::GetCurrentContext())
			return;

		list->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), ImColor(color.r, color.g, color.b, alpha), radius);
	}

	template <typename T>
	static void fillRectangleCustom(Vector4<T> pos, const UIColor& color, float alpha, Vector4<float>radius) {
		if (!ImGui::GetCurrentContext()) return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();
		list->AddRectFilledCustomRadius(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), ImColor(color.r, color.g, color.b, alpha), radius.x, radius.z, radius.y, radius.w);
	}

	static void fillShadowRectangle(Vector4<float> pos, const UIColor& color, float alpha, float thickness, ImDrawFlags flags, float radius = 0.f, ImDrawList* list = ImGui::GetBackgroundDrawList())
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImVec2 offset = ImVec2(0, 0);
		list->AddShadowRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), ImColor(color.r, color.g, color.b, alpha), thickness, offset, flags, radius);
	}

	static void fillShadowCircle(Vector2<float> pos, float radius, const UIColor& color, float alpha, float thickness, ImDrawFlags flags, float segments = 12.f)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();
		ImVec2 offset = ImVec2(0, 0);
		list->AddShadowCircle(ImVec2(pos.x, pos.y), radius, ImColor(color.r, color.g, color.b, alpha), thickness, offset, flags, radius);
	}

	static void drawShadowSquare(Vector2<float> center, float size, const UIColor& color, float alpha, float thickness, ImDrawFlags flags)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();
		ImVec2 offset = ImVec2(0, 0);

		// Define the four corners of the square
		ImVec2 points[4];
		points[0] = ImVec2(center.x - size / 2.f, center.y - size / 2.f);
		points[1] = ImVec2(center.x + size / 2.f, center.y - size / 2.f);
		points[2] = ImVec2(center.x + size / 2.f, center.y + size / 2.f);
		points[3] = ImVec2(center.x - size / 2.f, center.y + size / 2.f);

		list->AddShadowConvexPoly(points, 4, ImColor(color.r, color.g, color.b, alpha), thickness, offset, flags);
	}

	static void drawRoundRect(Vector4<float> pos, const ImDrawFlags& flags, float radius, const UIColor& color, float alpha, float lineWidth)
	{
		if (!ImGui::GetCurrentContext())
			return;
		const auto d = ImGui::GetBackgroundDrawList();
		d->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), ImColor(color.r, color.g, color.b, alpha), radius, flags, lineWidth);
	}

	static void fillGradientOpaqueRectangle(Vector4<float> pos, const UIColor& firstColor, const UIColor& secondColor, float firstAlpha, float secondAlpha)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();

		ImVec2 topLeft = ImVec2(pos.x, pos.y);
		ImVec2 bottomRight = ImVec2(pos.z, pos.w);

		list->AddRectFilledMultiColor(topLeft, bottomRight,
			ImColor(firstColor.r, firstColor.g, firstColor.b, secondAlpha),
			ImColor(secondColor.r, secondColor.g, secondColor.b, secondAlpha),
			ImColor(secondColor.r, secondColor.g, secondColor.b, firstAlpha),
			ImColor(firstColor.r, firstColor.g, firstColor.b, firstAlpha));
	}

	static void fillRoundedGradientRectangle(Vector4<float> pos, const UIColor& firstColor, const UIColor& secondColor, float radius, float firstAlpha, float secondAlpha)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();

		ImVec2 topLeft = ImVec2(pos.x, pos.y);
		ImVec2 bottomRight = ImVec2(pos.z, pos.w);

		list->AddRectFilledMultiColor(topLeft, bottomRight,
			ImColor(firstColor.r, firstColor.g, firstColor.b, secondAlpha),
			ImColor(secondColor.r, secondColor.g, secondColor.b, secondAlpha),
			ImColor(secondColor.r, secondColor.g, secondColor.b, firstAlpha),
			ImColor(firstColor.r, firstColor.g, firstColor.b, firstAlpha));
	}

	static void fillCircle(Vector2<float> center, float radius, const UIColor& color, float alpha, int segments)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();
		list->AddCircleFilled(ImVec2(center.x , center.y), radius, ImColor(color.r, color.g, color.b, alpha), segments);
	}

	static inline Vector2<float> getScreenSize() {
		RECT desktop;
		static HWND window = (HWND)FindWindowA(nullptr, "Minecraft");
		GetWindowRect(window, &desktop);
		int w = desktop.right - desktop.left;
		int h = desktop.bottom - desktop.top;
		// not in fullscreen
		if (desktop.top != 0 || desktop.left != 0) {
			w -= 30;
			h -= 45;
		}
		return Vector2<float>(w, h);
	}


	static inline float getTextWidth(std::string* textStr, float textSize)
	{
		return ImGui::GetFont()->CalcTextSizeA(textSize * 18, FLT_MAX, -1, textStr->c_str()).x;
	}

	static inline float getTextHeight(float textSize)
	{
		return ImGui::GetFont()->CalcTextSizeA(textSize * 18, FLT_MAX, -1, "").y;
	}

	static inline float getDeltaTime() {
		return 0.016f;
	}

	static inline Vector2<float> getMousePos() {
		if (!Global::getClientInstance()->getGuiData()) {
			return { 0, 0 };
		}
		return Global::getClientInstance()->getGuiData()->getMousePos().toFloat();
	}

	inline static bool isMouseOver(Vector4<float>(pos))
	{
		Vector2<short> mousePos = Global::getClientInstance()->getGuiData()->getMousePos();
		return mousePos.x >= pos.x && mousePos.y >= pos.y && mousePos.x < pos.z && mousePos.y < pos.w;
	}

	static __forceinline void drawBox(Vector3<float> lower1, Vector3<float> upper1, UIColor color, UIColor lineColor, float lineWidth, bool fill, bool outline) {
		Vector3<float> lower = lower1;
		Vector3<float> upper = upper1;

		Vector3<float> diff = upper.submissive(lower);
		Vector3<float> vertices[8];
		vertices[0] = Vector3<float>(lower.x, lower.y, lower.z);
		vertices[1] = Vector3<float>(lower.x + diff.x, lower.y, lower.z);
		vertices[2] = Vector3<float>(lower.x, lower.y + diff.y, lower.z);
		vertices[3] = Vector3<float>(lower.x + diff.x, lower.y + diff.y, lower.z);
		vertices[4] = Vector3<float>(lower.x, lower.y, lower.z + diff.z);
		vertices[5] = Vector3<float>(lower.x + diff.x, lower.y, lower.z + diff.z);
		vertices[6] = Vector3<float>(lower.x, lower.y + diff.y, lower.z + diff.z);
		vertices[7] = Vector3<float>(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

		auto instance = Global::getClientInstance();

		const auto d = ImGui::GetBackgroundDrawList();

		if (fill) {
			// Convert the vertices to screen coordinates
			std::vector<Vector2<float>> screenCords;
			for (int i = 0; i < 8; i++) {
				Vector2<float> screen;
				if (instance->WorldToScreen(vertices[i], screen)) {
					screenCords.push_back(screen);
				}
			}

			// Return if there are less than four points to draw quads with
			if (screenCords.size() < 8) return;

			// Define the indices of the vertices to use for each quad face
			std::vector<std::tuple<int, int, int, int>> faces = {
				{0, 1, 3, 2},  // Bottom face
				{4, 5, 7, 6},  // Top face
				{0, 1, 5, 4},  // Front face
				{2, 3, 7, 6},  // Back face
				{1, 3, 7, 5},  // Right face
				{0, 2, 6, 4}   // Left face
			};

			// Draw the quads to fill the box
			for (auto face : faces) {
				ImVec2 posScreenCords = ImVec2(screenCords[std::get<0>(face)].x, screenCords[std::get<0>(face)].y);
				ImVec2 posScreenCords1 = ImVec2(screenCords[std::get<1>(face)].x, screenCords[std::get<1>(face)].y);
				ImVec2 posScreenCords2 = ImVec2(screenCords[std::get<2>(face)].x, screenCords[std::get<2>(face)].y);
				ImVec2 posScreenCords3 = ImVec2(screenCords[std::get<3>(face)].x, screenCords[std::get<3>(face)].y);
				d->AddQuadFilled(posScreenCords, posScreenCords1, posScreenCords2, posScreenCords3, ImColor(color.r, color.g, color.b, color.a));
				//drawlist->AddQuadFilled(screenCords[std::get<3>(face)].toImVec2(), screenCords[std::get<2>(face)].toImVec2(), screenCords[std::get<1>(face)].toImVec2(), screenCords[std::get<0>(face)].toImVec2(), color.toImColor());
			}
		}

		{
			// Convert the vertices to screen coordinates
			std::vector<std::tuple<int, Vector2<float>>> screenCords;
			for (int i = 0; i < 8; i++) {
				Vector2<float> screen;
				if (instance->WorldToScreen(vertices[i], screen)) {
					screenCords.emplace_back(outline ? (int)screenCords.size() : i, screen);
				}
			}

			// Return if there are less than two points to draw lines between
			if (screenCords.size() < 2) return;

			switch (outline) {
			case false: {
				// Draw lines between all pairs of vertices
				for (auto it = screenCords.begin(); it != screenCords.end(); it++) {
					auto from = *it;
					auto fromOrig = vertices[std::get<0>(from)];

					for (auto to : screenCords) {
						auto toOrig = vertices[std::get<0>(to)];

						// Determine if the line should be drawn based on the relative positions of the vertices
						bool shouldDraw = false;
						// X direction
						shouldDraw |= fromOrig.y == toOrig.y && fromOrig.z == toOrig.z && fromOrig.x < toOrig.x;
						// Y direction
						shouldDraw |= fromOrig.x == toOrig.x && fromOrig.z == toOrig.z && fromOrig.y < toOrig.y;
						// Z direction
						shouldDraw |= fromOrig.x == toOrig.x && fromOrig.y == toOrig.y && fromOrig.z < toOrig.z;

						ImVec2 posForm = ImVec2(std::get<1>(from).x, std::get<1>(from).y);
						ImVec2 posTo = ImVec2(std::get<1>(to).x, std::get<1>(to).y);
						if (shouldDraw) d->AddLine(posForm, posTo, ImColor(lineColor.r, lineColor.g, lineColor.b, lineColor.a), lineWidth);
					}
				}
				return;
				break;
			}
			case true: {
				// Find start vertex
				auto it = screenCords.begin();
				std::tuple<int, Vector2<float>> start = *it;
				it++;
				for (; it != screenCords.end(); it++) {
					auto cur = *it;
					if (std::get<1>(cur).x < std::get<1>(start).x) {
						start = cur;
					}
				}

				// Follow outer line
				std::vector<int> indices;

				auto current = start;
				indices.push_back(std::get<0>(current));
				Vector2<float> lastDir(0, -1);
				do {
					float smallestAngle = PI * 2;
					Vector2<float> smallestDir;
					std::tuple<int, Vector2<float>> smallestE;
					auto lastDirAtan2 = atan2(lastDir.y, lastDir.x);
					for (auto cur : screenCords) {
						if (std::get<0>(current) == std::get<0>(cur))
							continue;

						// angle between vecs
						Vector2<float> dir = Vector2<float>(std::get<1>(cur)).submissive(std::get<1>(current));
						float angle = atan2(dir.y, dir.x) - lastDirAtan2;
						if (angle > PI) {
							angle -= 2 * PI;
						}
						else if (angle <= -PI) {
							angle += 2 * PI;
						}
						if (angle >= 0 && angle < smallestAngle) {
							smallestAngle = angle;
							smallestDir = dir;
							smallestE = cur;
						}
					}
					indices.push_back(std::get<0>(smallestE));
					lastDir = smallestDir;
					current = smallestE;
				} while (std::get<0>(current) != std::get<0>(start) && indices.size() < 8);

				// draw

				Vector2<float> lastVertex;
				bool hasLastVertex = false;
				for (auto& indice : indices) {
					Vector2<float> curVertex = std::get<1>(screenCords[indice]);
					if (!hasLastVertex) {
						hasLastVertex = true;
						lastVertex = curVertex;
						continue;
					}
					ImVec2 lastVertexPos = ImVec2(lastVertex.x, lastVertex.y);
					ImVec2 curVertexPos = ImVec2(curVertex.x, curVertex.y);
					d->AddLine(lastVertexPos, curVertexPos, ImColor(lineColor.r, lineColor.g, lineColor.b, lineColor.a), lineWidth);
					lastVertex = curVertex;
				}
				return;
				break;
			}
			}
		}
	}
};