#pragma once

class Killaura : public Module
{
public:
    Killaura(int keybind = 80, bool enabled = false) :
        Module("Killaura", "Combat", "Hits every entity around you.", keybind, enabled)
    {
        addEnum("Mode", "How many entities should be attacked", { "Single", "Multi" }, &switchMode);
        addEnum("Rotations", "The style of the rotations", { "Simple", "Flareon" }, &rotations);
        addEnum("Swing", "The way the players arm swing", { "Normal", "None" }, &Swing);
        addSlider("Distance", "The distance of attacking", &range, 3, 10);
        addSlider("APS", "The speed of attack (attacks per secound)", &APS, 3, 30);
        addBool("Strafe", "Strafe the player towards the entity", &Strafe);
    }

    int switchMode = 0;
    int rotations = 0;
    int Swing = 0; // enum
    float range = 4;
    float APS = 10;
    bool Strafe = true;

    struct Distance {
        bool operator()(Actor* target, Actor* target2) { // Sort by distance
            auto instance = Global::getClientInstance();
            auto localPlayer = instance->getLocalPlayer();
            return (target->getStateVector()->Position).distance(localPlayer->getStateVector()->Position) < (target2->getStateVector()->Position).distance(localPlayer->getStateVector()->Position);
        }
    };

    std::vector<Actor*> EntityList;
    void makeEntityList() {
        auto instance = Global::getClientInstance();
        auto localPlayer = instance->getLocalPlayer();
        auto list = localPlayer->getLevel()->getRuntimeActorList();
        auto lpPos = localPlayer->getStateVector()->Position;
        for (auto* actor : list) {
            if (actor != localPlayer && actor->isAlive()) {
                float dist = localPlayer->getStateVector()->Position.distance(actor->getStateVector()->Position);
                if (dist < 50.f) {
                    EntityList.push_back(actor);
                }
            }
        }
    }

    const static Vector2<float> CalcAngle(Vector3<float> ths, Vector3<float> dst)
    {
        Vector3 diff = dst.submissive(ths);

        diff.y = diff.y / diff.magnitude();
        Vector2<float> angles;
        angles.x = asinf(diff.y) * -(180.0f / 3.1415927f);
        angles.y = (float)-atan2f(diff.x, diff.z) * (180.0f / 3.1415927f);

        return angles;
    }

    void onEvent(ActorBaseTickEvent* event) override {
        auto player = Global::getClientInstance()->getLocalPlayer();
        if (!player)
        {
            return;
        }

        EntityList.clear();
        makeEntityList();

        //sort based on distance
        std::sort(EntityList.begin(), EntityList.end(), Distance());

        GameMode* gm = player->getGamemode();

        if (!gm)
            return;

        auto list = player->getLevel()->getRuntimeActorList();

        for (auto* actor : list) {
            if (actor != player && actor->isAlive() && actor->isPlayer()) {
                Vector3<float> localPos = player->getStateVector()->Position;
                Vector3<float> playerPos = actor->getStateVector()->Position;

                float playerDistance = localPos.distance(playerPos);

                if (playerDistance <= range) {
                    Vector2<float> angle = CalcAngle(localPos, playerPos);

                    if (rotations == 0) { // Simple

                    }
                    if (rotations == 1) { // Flareon

                    }

                    if (Strafe) {
                        player->getMovementInterpolator()->Rotations = angle;
                    }

                    if (TimeUtils::hasTimeElapsed("kaTimer", 1000 / APS, true)) {
                        if (Swing == 0) { player->swing(); }

                        if (switchMode == 0) {
                            gm->attack(actor[0]);
                        }
                        else if (switchMode == 1) {
                            gm->attack(*actor);
                        }
                    }
                }
            }
        }
    }

    void onEvent(ImGuiRenderEvent* event) override {
        if (getModuleByName("clickgui")->isEnabled()) return;
        Vector3<float> targetPos;

        if (!EntityList.empty()) {
            auto target = EntityList[0];
            if (target == nullptr) return;
            targetPos = target->getStateVector()->Position;
        }

        for (int i = 0; i < floor(16); i++) // count
        {
            float angle = (360 / floor(16)) * i; // count
            float rad = angle * (PI / 180);
            Vector3<float> pos = Vector3<float>(
                targetPos.x + (cos(rad) * 1),
                targetPos.y,
                targetPos.z + (sin(rad) * 1)
            );
            float circleSize;
            Vector2<float> screen;
            if (!Global::getClientInstance()->WorldToScreen(pos, screen)) continue;

            circleSize = 4.f;

            circleSize = circleSize;
            UIColor color = ColorUtils::Rainbow(1, 1, 1, 1);
            ImRenderUtil::fillCircle(screen, circleSize, color, 1, 12.f);
            if (circleSize <= 2) continue;
            ImRenderUtil::fillShadowCircle(screen, circleSize, color, 1, 40.f, 0);
        }
    }

    std::string getModeName() override {
        return " " + std::string("Multi");
    }
};