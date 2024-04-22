#pragma once

class TestFly : public Module
{
public:
    TestFly(int keybind = 70, bool enabled = false) :
        Module("TestFly", "Motion", "Fly like a superhero.", keybind, enabled)
    {

    }

    int CurrentPosition = 0;

    void onEnabled() override {
        Player* player = Global::getClientInstance()->getLocalPlayer();
        StateVectorComponent* state = player->getStateVector();

        if (!player || !state) { return; }
    }

    void onEvent(RenderContextEvent* event) override {
        Player* player = Global::getClientInstance()->getLocalPlayer();
        StateVectorComponent* state = player->getStateVector();

        if (!player || !state) { return; }

        player->getActorCollision()->setOnGround(true);
        
    }

    void onEvent(PacketEvent* event) override {
    }

    void onDisabled() override {
        if (!Global::getClientInstance())
            return;

        if (!Global::getClientInstance()->getTimerClass())
            return;

        Global::getClientInstance()->getTimerClass()->setMainTimerSpeed(20);
    }

    std::string getModeName() override {
        return "";
    }
};