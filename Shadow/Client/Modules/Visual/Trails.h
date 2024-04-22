#pragma once

class Trails : public Module
{
public:
	Trails(int keybind = 7, bool enabled = true) :
		Module("Trail", "Visual", "Draw Trail", keybind, enabled)
	{
		
	}

	int delayCount = 0;

	struct TrailLog {
		int time;
		Vector3<float> pos;
	};
	long long timeOffset = 0;
	std::vector<TrailLog> trailLog;
	Vector3<float> oldPos;

	void onEvent(RenderContextEvent* event) {
		delayCount++;
		if (delayCount <= 12)return;
		delayCount = 0;
		if (timeOffset == 0) timeOffset = TimeUtils::getCurrentMs();;
		auto localPlayer = Global::getClientInstance()->getLocalPlayer();
		if (localPlayer == nullptr) return;
		int time = TimeUtils::getCurrentMs() - timeOffset;
		auto pos = localPlayer->getStateVector()->Position;
		if (oldPos.distance(pos) > 0.01) {
			TrailLog trail = {};
			trail.time = time;
			trail.pos = localPlayer->getStateVector()->Position;

			trailLog.push_back(trail);
			oldPos = pos;
		}

		for (auto it = trailLog.begin(); it != trailLog.end(); ) {
			if ((time - it->time) > 1000) {
				it = trailLog.erase(it);
			}
			else {
				++it;
			}
		}
	}

	void onEvent(ImGuiRenderEvent* event) override
	{
		if (!Global::getClientInstance()->getMinecraftGame()->getCanUseKeys())return;
		int time = TimeUtils::getCurrentMs() - timeOffset;
		for (auto trail : trailLog) {
			int time2 = time - trail.time;
			int time3 = 1000 - time2;
			Vector2<float> screen, screen2, screen3;
			if (!Global::getClientInstance()->WorldToScreen(trail.pos, screen)) continue;
			if (!Global::getClientInstance()->WorldToScreen(Vector3<float>(trail.pos.x - 0.1f, trail.pos.y - 0.1f, trail.pos.z - 0.1f), screen2)) continue;
			if (!Global::getClientInstance()->WorldToScreen(Vector3<float>(trail.pos.x + 0.1f, trail.pos.y + 0.1f, trail.pos.z + 0.1f), screen2)) continue;

			float circleSize = std::fmin(std::fmax(abs(screen.x - screen2.x), abs(screen.y - screen2.y)), 8);
			float opacity = time3 < 300 ? time3 / 300.f : 1;
			circleSize = circleSize * (time2 < 100 ? time2 / 100.f : 1) * opacity;

			ImRenderUtil::fillCircle(screen, circleSize, UIColor(255, 255, 255), opacity, 12.f);
			if (circleSize <= 2) continue;
			ImRenderUtil::fillShadowCircle(screen, circleSize, UIColor(255, 255, 255), opacity, 40.f, 0);
		}
	}

	void onEnabled() override {
		timeOffset = TimeUtils::getCurrentMs();
	}

	void onDisabled() override {
		trailLog.clear();
	}
};