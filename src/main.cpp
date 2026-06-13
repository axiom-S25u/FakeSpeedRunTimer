#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

static const ccColor3B kGreen = {20, 255, 70};

struct FakeTimerFields {
    CCLabelBMFont* bigLabel  = nullptr;
    CCLabelBMFont* msLabel   = nullptr;
    CCLabelBMFont* wrLabel   = nullptr;
    CCScale9Sprite* bg       = nullptr;
    float fakeMs             = 0.0f;
    float flashTimer         = 0.0f;
    float chaosTimer         = 0.0f;
    float chaosMult          = 1.0f;
    int   chaosMode          = 0;
};

class $modify(MyPlayLayer, PlayLayer) {
    struct Fields : FakeTimerFields {};

    bool init(GJGameLevel* lvl, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(lvl, useReplay, dontCreateObjects)) return false;

        if (m_isPracticeMode || m_isPlatformer) return true;

        CCSize ws = CCDirector::sharedDirector()->getWinSize();

        int maxZ = 0;
        for (CCNode* kid : CCArrayExt<CCNode*>(this->getChildren())) {
            if (kid->getZOrder() > maxZ) maxZ = kid->getZOrder();
        }

        m_fields->bg = CCScale9Sprite::create("square02_001.png");
        m_fields->bg->setContentSize({148.0f, 38.0f});
        m_fields->bg->setColor({0, 0, 0});
        m_fields->bg->setOpacity(170);
        m_fields->bg->setPosition(ws.width - 88.0f, ws.height - 26.0f);
        this->addChild(m_fields->bg, maxZ + 1);

        m_fields->bigLabel = CCLabelBMFont::create("00:00", "bigFont.fnt");
        m_fields->bigLabel->setScale(0.68f);
        m_fields->bigLabel->setColor(kGreen);
        m_fields->bigLabel->setAnchorPoint({1.0f, 0.5f});
        m_fields->bigLabel->setPosition(ws.width - 50.0f, ws.height - 26.0f);
        this->addChild(m_fields->bigLabel, maxZ + 2);

        m_fields->msLabel = CCLabelBMFont::create(".000", "bigFont.fnt");
        m_fields->msLabel->setScale(0.38f);
        m_fields->msLabel->setColor(kGreen);
        m_fields->msLabel->setAnchorPoint({0.0f, 0.38f});
        m_fields->msLabel->setPosition(ws.width - 48.0f, ws.height - 26.0f);
        this->addChild(m_fields->msLabel, maxZ + 2);

        m_fields->wrLabel = CCLabelBMFont::create("WR PACE", "bigFont.fnt");
        m_fields->wrLabel->setScale(0.55f);
        m_fields->wrLabel->setColor({20, 255, 70});
        m_fields->wrLabel->setPosition(ws.width - 88.0f, ws.height - 46.0f);
        m_fields->wrLabel->setOpacity(220);
        this->addChild(m_fields->wrLabel, maxZ + 2);

        this->schedule(schedule_selector(MyPlayLayer::tickFakeTimer), 1.0f / 60.0f);

        return true;
    }

    void tickFakeTimer(float dt) {
        if (m_isPracticeMode || m_isPlatformer) {
            if (m_fields->bg) {
                m_fields->bg->removeFromParent();
                m_fields->bg = nullptr;
            }
            if (m_fields->bigLabel) {
                m_fields->bigLabel->removeFromParent();
                m_fields->bigLabel = nullptr;
            }
            if (m_fields->msLabel) {
                m_fields->msLabel->removeFromParent();
                m_fields->msLabel = nullptr;
            }
            if (m_fields->wrLabel) {
                m_fields->wrLabel->removeFromParent();
                m_fields->wrLabel = nullptr;
            }
            return;
        }

        if (!m_fields->bg) {
            CCSize ws = CCDirector::sharedDirector()->getWinSize();

            int maxZ = 0;
            for (CCNode* kid : CCArrayExt<CCNode*>(this->getChildren())) {
                if (kid->getZOrder() > maxZ) maxZ = kid->getZOrder();
            }

            m_fields->bg = CCScale9Sprite::create("square02_001.png");
            m_fields->bg->setContentSize({148.0f, 38.0f});
            m_fields->bg->setColor({0, 0, 0});
            m_fields->bg->setOpacity(170);
            m_fields->bg->setPosition(ws.width - 88.0f, ws.height - 26.0f);
            this->addChild(m_fields->bg, maxZ + 1);

            m_fields->bigLabel = CCLabelBMFont::create("00:00", "bigFont.fnt");
            m_fields->bigLabel->setScale(0.68f);
            m_fields->bigLabel->setColor(kGreen);
            m_fields->bigLabel->setAnchorPoint({1.0f, 0.5f});
            m_fields->bigLabel->setPosition(ws.width - 50.0f, ws.height - 26.0f);
            this->addChild(m_fields->bigLabel, maxZ + 2);

            m_fields->msLabel = CCLabelBMFont::create(".000", "bigFont.fnt");
            m_fields->msLabel->setScale(0.38f);
            m_fields->msLabel->setColor(kGreen);
            m_fields->msLabel->setAnchorPoint({0.0f, 0.38f});
            m_fields->msLabel->setPosition(ws.width - 48.0f, ws.height - 26.0f);
            this->addChild(m_fields->msLabel, maxZ + 2);

            m_fields->wrLabel = CCLabelBMFont::create("WR PACE", "bigFont.fnt");
            m_fields->wrLabel->setScale(0.55f);
            m_fields->wrLabel->setColor({20, 255, 70});
            m_fields->wrLabel->setPosition(ws.width - 88.0f, ws.height - 46.0f);
            m_fields->wrLabel->setOpacity(220);
            this->addChild(m_fields->wrLabel, maxZ + 2);
        }

        m_fields->chaosTimer -= dt;
        if (m_fields->chaosTimer <= 0.0f) {
            int val = rand() % 5;
            m_fields->chaosMode  = val;
            m_fields->chaosTimer = 0.4f + static_cast<float>(rand() % 180) / 100.0f;

            if      (val == 0) m_fields->chaosMult = 0.0f;
            else if (val == 1) m_fields->chaosMult = -1.8f;
            else if (val == 2) m_fields->chaosMult = 0.3f + static_cast<float>(rand() % 80) / 100.0f;
            else if (val == 3) m_fields->chaosMult = 3.5f + static_cast<float>(rand() % 500) / 100.0f;
            else               m_fields->chaosMult = 1.0f;
        }

        m_fields->fakeMs += dt * 1000.0f * m_fields->chaosMult;
        if (m_fields->fakeMs < 0.0f) m_fields->fakeMs = 0.0f;

        int totalMs = static_cast<int>(m_fields->fakeMs);
        int ms      = totalMs % 1000;
        int secs    = (totalMs / 1000) % 60;
        int mins    = (totalMs / 60000) % 60;

        char bigBuf[16];
        char msBuf[8];
        snprintf(bigBuf, sizeof(bigBuf), "%02d:%02d", mins, secs);
        snprintf(msBuf,  sizeof(msBuf),  ".%02d", ms / 10);

        m_fields->bigLabel->setString(bigBuf);
        m_fields->msLabel->setString(msBuf);

        m_fields->flashTimer -= dt;
        if (m_fields->flashTimer <= 0.0f) {
            m_fields->flashTimer = 0.15f + static_cast<float>(rand() % 60) / 100.0f;
            bool bruh = rand() % 2 == 0;
            if (bruh) {
                m_fields->wrLabel->setColor({20, 255, 70});
                m_fields->wrLabel->setString("WR PACE");
            } else {
                m_fields->wrLabel->setColor({255, 55, 55});
                m_fields->wrLabel->setString("LOSING PACE");
            }
        }
    }

    void resetLevel() {
        m_fields->fakeMs = 0.0f;
        PlayLayer::resetLevel();
    }

    void onQuit() {
        this->unscheduleAllSelectors();
        PlayLayer::onQuit();
    }
};