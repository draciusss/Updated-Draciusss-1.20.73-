
#pragma once

class LevelInfo : public Module
{
public:
    LevelInfo(int keybind = Keys::NONE, bool enabled = false) :
        Module("LevelInfo", "Visual", "Displays information about level", keybind, enabled)
    {

    }

    int ping = 0;

    std::vector<float> bpsQueue;
    std::string currBps = "";
    int tick = 0;

    void onEnabled() override {
        bpsQueue.clear();
        currBps = "0.0";
        tick = 0;
    }

    void onEvent(RenderContextEvent* event) override {
        auto* player = Global::getClientInstance()->getLocalPlayer();
        if (player == nullptr)
        {
            currBps = "0.0";
            if (!bpsQueue.empty())
            {
                bpsQueue.clear();
            }
            tick = 0;
            return;
        }

        if (tick >= 20)
        {
            float total = 0.f;
            if (!bpsQueue.empty())
            {
                for (int i = 0; i < bpsQueue.size(); i++)
                {
                    if (i > 20)
                        break;
                    total += bpsQueue.at(i);
                    bpsQueue.erase(bpsQueue.begin());
                }
                total /= (bpsQueue.size() > 20 ? 20 : bpsQueue.size());
                total = truncf(total * 10) / 10;
            }
            std::stringstream ss;
            ss << std::fixed << std::setprecision(1) << total;
            currBps = ss.str();
            //currBps = std::to_string(total);
            tick = 0;
            //bpsQueue.clear();

            bpsQueue.push_back(std::stof(
                std::to_string((int)player->getBlocksPerSecond()) + std::string(".") + std::to_string((int)(player->getBlocksPerSecond() * 10) - ((int)player->getBlocksPerSecond() * 10))
            ));
            return;
        }
        bpsQueue.push_back(std::stof(
            std::to_string((int)player->getBlocksPerSecond()) + std::string(".") + std::to_string((int)(player->getBlocksPerSecond() * 10) - ((int)player->getBlocksPerSecond() * 10))
        ));
        tick++;
    }

    void onEvent(ImGuiRenderEvent* event) override {
        auto player = Global::getClientInstance()->getLocalPlayer();
        if (!player || !Global::getClientInstance()->getMinecraftGame()->getCanUseKeys()) return;

        //LevelInfo
        Vector2<float> windowSize = Global::getClientInstance()->getGuiData()->mcResolution; // Windows Resolution
        Vector3<float> currPos = player->getStateVector()->Position; // User's current position

        std::string PingLevelInfo = string("Ping: " + to_string(ping));
        std::string BPSLevelInfo = string("BPS: " + std::to_string((int)player->getBlocksPerSecond()) + std::string(".") + std::to_string((int)(player->getBlocksPerSecond() * 10) - ((int)player->getBlocksPerSecond() * 10))) + std::string(" (average: " + currBps + ")");
        std::string XYZLevelInfo = "XYZ: " + std::to_string((int)floorf(currPos.x)) + " " + std::to_string((int)floorf(currPos.y)) + " " + std::to_string((int)floorf(currPos.z));

        //ImRenderUtil::drawText(Vector2(4.f, windowSize.y - 63), &PingLevelInfo, UIColor(255, 255, 255), 1.15, 1, true);
        ImRenderUtil::drawText(Vector2(4.f, windowSize.y - 42), &BPSLevelInfo, UIColor(255, 255, 255), 1.15, 1, true);
        ImRenderUtil::drawText(Vector2(4.f, windowSize.y - 21), &XYZLevelInfo, UIColor(255, 255, 255), 1.15, 1, true);
    }

    void onEvent(PacketEvent* event) override {
        if (event->Sender->NetworkSystem->RemoteConnectorComposite->RakNetConnector != nullptr) {
            if (event->Sender->NetworkSystem->RemoteConnectorComposite->RakNetConnector->JoinedIp == "") { // Checks if User is in a server
                ping = 0; // Sets the ping to 0 if user is in a world to avoid crashing
            }
            else {
                ping = event->Sender->NetworkSystem->RemoteConnectorComposite->RakNetConnector->getPeer()->getPing();

            }
        }
        //ping = event->Sender->NetworkSystem->RemoteConnectorComposite->RakNetConnector->getPeer()->getPing();
    }
};

