#pragma once

class ESP : public Module
{
public:
    ESP(int keybind = 7, bool enabled = true) :
        Module("ESP", "Visual", "Draw basic boxes around entities", keybind, enabled)
    {

    }

    void drawEntity(Player* entity)
    {
        
    }


    void onEvent(ImGuiRenderEvent* event) override
    {
        Player* player = Global::getClientInstance()->getLocalPlayer();
        if (!player || !Global::getClientInstance()->getMinecraftGame()->getCanUseKeys())
            return;

        AABBShapeComponent* shape = player->getAABBShape();
        if (!shape || IsBadReadPtr(shape, sizeof(AABBShapeComponent))) return;

        Vector3<float> entDims = { shape->Hitbox.x, shape->Hitbox.y, shape->Hitbox.x };

        if (entDims.x != 0.6f || entDims.y != 1.8f)
            return;

        Vector3<float> entLower = shape->PosLower;
        Vector3<float> entUpper = shape->PosUpper;
        Vector3<float> entPos = player->getStateVector()->Position;

        Vector3<float> origin = Global::getClientInstance()->getLevelRender()->getOrigin();
        float dist = origin.distance(entPos);

        Vector2<float> output1, output2;
        if (!Global::getClientInstance()->WorldToScreen(entLower, output1) || !Global::getClientInstance()->WorldToScreen(entUpper, output2)) return;

        std::vector<Vector3<float>> aabbArr;
        for (float x : { entLower.x, entUpper.x })
        {
            for (float y : { entLower.y, entUpper.y })
            {
                for (float z : { entLower.z, entUpper.z })
                {
                    aabbArr.push_back({ x, y, z });
                }
            }
        }

        std::vector<Vector2<float>> scrPoints;
        for (int i = 0; i < aabbArr.size(); i++)
        {
            Vector2<float> scrPoint;
            if (Global::getClientInstance()->WorldToScreen(aabbArr[i], scrPoint))
                scrPoints.push_back(scrPoint);
        }

        if (scrPoints.empty())
            return;

        Vector4<float> boundingRect = { scrPoints[0].x, scrPoints[0].y, scrPoints[0].x, scrPoints[0].y };
        for (const auto& point : scrPoints)
        {
            boundingRect.x = std::fmin(boundingRect.x, point.x);
            boundingRect.y = std::fmin(boundingRect.y, point.y);
            boundingRect.z = std::fmax(boundingRect.z, point.x);
            boundingRect.w = std::fmax(boundingRect.w, point.y);
        }

        float thickness2 = fmax(1.f, 1.f / std::fmax(1.f, origin.distance(entPos)));
        thickness2 *= 3; // Here is the thickness level
        ImRenderUtil::drawRoundRect(boundingRect, 0, 0.f, UIColor(0, 0, 0), 0.75f, thickness2 + 2.f);
        ImRenderUtil::drawRoundRect(boundingRect, 0, 0.f, UIColor(255, 255, 255), 1.f, thickness2);
    }
};
