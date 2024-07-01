#include <Geode/Geode.hpp>

using namespace geode::prelude;

//ae
$on_mod(Loaded) {
	CCFileUtils::sharedFileUtils()->addPriorityPath(
		(Mod::get()->getTempDir() / "resources" / "user95401.run_mod").string().data()
	);
}

//tools
#if 1

class CCSpriteExt : public CCSprite {
public:
	static CCSprite* create(const char* pszFileName) {
		auto rtn = CCSprite::create(pszFileName);
		auto boolUserObj = dynamic_cast<CCBool*>(rtn->getUserObject("geode.texture-loader/fallback"));
		if (boolUserObj) return nullptr;
		else return rtn;
	};
	static CCSprite* createWithSpriteFrameName(const char* pszSpriteFrameName) {
		auto rtn = CCSprite::createWithSpriteFrameName(pszSpriteFrameName);
		auto boolUserObj = dynamic_cast<CCBool*>(rtn->getUserObject("geode.texture-loader/fallback"));
		if (boolUserObj) return nullptr;
		else return rtn;
	};
};

std::string framePath(CCNode* node) {
	if (auto textureProtocol = typeinfo_cast<CCTextureProtocol*>(node)) {
		if (auto texture = textureProtocol->getTexture()) {
			auto* cachedTextures = CCTextureCache::sharedTextureCache()->m_pTextures;
			for (auto [key, obj] : CCDictionaryExt<std::string, CCTexture2D*>(cachedTextures)) {
				if (obj == texture) {
					return key.c_str();
					break;
				}
			}

			if (auto spriteNode = typeinfo_cast<CCSprite*>(node)) {
				auto* cachedFrames = CCSpriteFrameCache::sharedSpriteFrameCache()->m_pSpriteFrames;
				const auto rect = spriteNode->getTextureRect();
				for (auto [key, frame] : CCDictionaryExt<std::string, CCSpriteFrame*>(cachedFrames)) {
					if (frame->getTexture() == texture && frame->getRect() == rect) {
						return key.c_str();
						break;
					}
				}
			}

		}
	}
	auto btnSpriteTry = framePath(getChild(node, 0));
	if (btnSpriteTry != "nah") return btnSpriteTry;
	return "nah";
}

std::vector<std::string> explode(std::string separator, std::string input) {
	std::vector<std::string> vec;
	for (int i{ 0 }; i < input.length(); ++i) {
		int pos = input.find(separator, i);
		if (pos < 0) { vec.push_back(input.substr(i)); break; }
		int count = pos - i;
		vec.push_back(input.substr(i, count));
		i = pos + separator.length() - 1;
	}
	if (vec.size() == 0) vec.push_back(input);/*
	std::stringstream log;
	for (auto item : vec)
		log << std::endl << item << std::endl;
	log::debug("{}(separator \"{}\", input \"{}\").rtn({})", __FUNCTION__, separator, input, log.str());*/
	return vec;
}

auto formatID(CCNode* of) {
	auto rtn = of->getID();
	if (rtn.empty()) rtn = framePath(of);
	if (string::contains(rtn, "resources/")) {
		auto slExpl = explode("/", rtn);
		rtn = slExpl[slExpl.size() - 1];
	}
	auto slExpl = explode("/", rtn);
	rtn = slExpl[slExpl.size() - 1];
	rtn = string::toUpper(rtn);
	rtn = string::replace(rtn, "-BUTTON-", "");
	rtn = string::replace(rtn, "-BUTTON", "");
	rtn = string::replace(rtn, "001", " ");
	rtn = string::replace(rtn, "GJ_", " ");
	rtn = string::replace(rtn, "-ROUND", " ");
	rtn = string::replace(rtn, "_ROUND", " ");
	rtn = string::replace(rtn, "-LOGO", " ");
	rtn = string::replace(rtn, "_LOGO", " ");
	rtn = string::replace(rtn, "-UHD", " ");
	rtn = string::replace(rtn, "-HD", " ");
	rtn = string::replace(rtn, "_ICON", " ");
	rtn = string::replace(rtn, ".PNG", " ");
	rtn = string::replace(rtn, "-", " ");
	rtn = string::replace(rtn, "_", " ");
	rtn = string::replace(rtn, "   ", "");
	rtn = string::replace(rtn, "  ", "");
	return (rtn);
}

void updateItemToLabel(CCMenuItemSpriteExtra* item) {
	if (not item) return;
	auto label = CCLabelBMFont::create(formatID(item).data(), "chatFont.fnt");
	item->setNormalImage(label);
	//item->m_animationEnabled = 0;
	//item->m_colorEnabled = 1;
}

#endif // 1

//sound effects
#if 1

#include <Geode/modify/GameManager.hpp>
class $modify(GameManagerExt, GameManager) {
	gd::string getMenuMusicFile() {
		return "Menu.ogg"_spr;
	}
};

#include <Geode/modify/FMODAudioEngine.hpp>
class $modify(FMODAudioEngineExt, FMODAudioEngine) {
	void playMusic(gd::string path, bool shouldLoop, float fadeInTime, int channel) {
		log::debug("{}(path {}, shouldLoop {}, fadeInTime {}, channel {})", __FUNCTION__, path, shouldLoop, fadeInTime, channel);
		if (string::contains(path, "StayInsideMe")) path = "Tuto.ogg"_spr;
		return FMODAudioEngine::playMusic(path, shouldLoop, fadeInTime, channel);
	}
	void playEffect(gd::string path, float speed, float p2, float volume) {
		log::debug("{}(path {}, speed {}, p2 {}, volume {})", __FUNCTION__, path, speed, p2, volume);
		if (string::contains(path, "playSound_01")) path = "sound_validation.ogg"_spr;
		if (string::contains(path, "quitSound_01")) path = "sound_wrong.ogg"_spr;
		return FMODAudioEngine::playEffect(path, speed, p2, volume);
	}
};

#endif // 1

//Sprites
#if 1

#include <Geode/modify/CCSprite.hpp>
class $modify(CCSpriteModExt, CCSprite) {
	static inline std::function<void(CCSprite*)> nextFunc = [](CCSprite* __this) {};
	void execNextFuncSch(float) {
		nextFunc(this);
		nextFunc = [](CCSprite* __this) {};
	}
	static void execNextFunc(CCNode* tar, std::function<void(CCSprite*)> nextFuncToSet = [](CCSprite* __this) {}) {
		nextFunc = nextFuncToSet;
		tar->scheduleOnce(schedule_selector(CCSpriteModExt::execNextFuncSch), 0.f);
	}
	static CCSprite* create(const char* pszFileName) {
		CCSprite* rtn = CCSprite::create(pszFileName);
		if (string::contains(pszFileName, "GJ_gradientBG")) {
			rtn->setContentSize(CCDirector::get()->getWinSize());
			//main_background.png
			if (CCSprite* main_background = CCSprite::create("main_background.png"_spr)) {
				//sprite
				main_background->setScaleY(rtn->getContentHeight() / main_background->getContentHeight());
				main_background->setScaleX(rtn->getContentWidth() / main_background->getContentWidth());
				main_background->setAnchorPoint({ 0.f, 0.f });
				rtn->addChild(main_background, 1);
			};
			//black placeholder for main_illustration
			if (CCSprite* main_illustration_pref = CCSprite::create("main_background.png"_spr)) {
				//sprite
				main_illustration_pref->setColor(ccBLACK);
				main_illustration_pref->setScaleY(rtn->getContentHeight() / main_illustration_pref->getContentHeight());
				main_illustration_pref->setScaleX((rtn->getContentWidth() - (302.f)) / main_illustration_pref->getContentWidth());
				main_illustration_pref->setAnchorPoint({ 0.f, 0.f });
				rtn->addChild(main_illustration_pref, 1);
			};
			//clouds_finish_back
			if (CCSprite* clouds_finish_back = CCSprite::createWithSpriteFrameName("clouds_finish_back-1.png"_spr)) {
				//sprite
				clouds_finish_back->setScale(0.8f);
				clouds_finish_back->setPositionX(rtn->getContentWidth());
				clouds_finish_back->setAnchorPoint({ 1.14f, -0.43f });
				rtn->addChild(clouds_finish_back, 1);
				//frames
				auto frames = new CCArray;
				for (int i = 2; i <= 17; i++)
					frames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fmt::format("clouds_finish_back-{}.png"_spr, i).data()));
				//animate
				clouds_finish_back->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.2f))));
			};
			//clouds_finish_back inverted dub
			if (CCSprite* clouds_finish_back = CCSprite::createWithSpriteFrameName("clouds_finish_back-1.png"_spr)) {
				//sprite
				clouds_finish_back->setFlipX(1);
				clouds_finish_back->setScale(0.85f);
				clouds_finish_back->setPositionX(rtn->getContentWidth());
				clouds_finish_back->setAnchorPoint({ 1.04f, -0.65f });
				rtn->addChild(clouds_finish_back, 1);
				//frames
				auto frames = new CCArray;
				for (int i = 2; i <= 17; i++)
					frames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fmt::format("clouds_finish_back-{}.png"_spr, i).data()));
				//animate
				clouds_finish_back->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.2f))));
			};
			//main_illustration
			if (CCSprite* main_illustration = CCSprite::createWithSpriteFrameName("main_illustration-1.png"_spr)) {
				//sprite
				main_illustration->setScale(rtn->getContentHeight() / main_illustration->getContentHeight());
				main_illustration->setPositionX(rtn->getContentWidth());
				main_illustration->setAnchorPoint({ 1.f, 0.f });
				rtn->addChild(main_illustration, 1);
				//frames
				auto frames = new CCArray;
				for (int i = 1; i <= 12; i++)
					frames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fmt::format("main_illustration-{}.png"_spr, i).data()));
				//animate
				main_illustration->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.2f))));
			};
			auto func = [](CCSprite* __this) {
				__this->setScale(1.f);
				__this->setPosition(CCPointZero);
				__this->setAnchorPoint(CCPointZero);
				};
			execNextFunc(rtn, func);
		}
		return rtn;
	}
	static CCSprite* createWithSpriteFrameName(const char* pszSpriteFrameName) {
		auto frameAtSprExtName = (Mod::get()->getID() + "/" + pszSpriteFrameName);
		auto frameAtSprExtExists = CCSpriteFrameCache::get()->m_pSpriteFrames->objectForKey(frameAtSprExtName);
		auto rtn = CCSprite::createWithSpriteFrameName(frameAtSprExtExists ? frameAtSprExtName.data() : pszSpriteFrameName);
		if (string::contains(pszSpriteFrameName, "GJ_logo_001")) {
			auto container = CCSprite::create();
			rtn = container;
			//typo_run_only
			CCSprite* typo_run_only1 = CCSprite::create("typo_run_only1.png");
			if (typo_run_only1) {
				container->addChild(typo_run_only1);
				//frames
				auto frames = new CCArray;
				for (int i = 1; i <= 12; i++)
					frames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fmt::format("typo_run_only{}.png"_spr, i).data()));
				//animate
				typo_run_only1->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.1f))));
			};
			//label
			auto label = CCLabelBMFont::create("MEETS GD IDK LOL", "chatFont.fnt");
			if (label) {
				label->setScale(0.8f);
				label->setPositionY(-36.f);
				container->addChild(label);
			}
		};
		return rtn;
	}
};

#endif // 1

//layers n objects
#if 1

#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayerExt, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;
		//main-menu-bg
		if (auto node = this->getChildByID("main-menu-bg")) node->setZOrder(-10);
		//main-title
		if (auto node = this->getChildByID("main-title")) node->setPositionX(136.f);
		//more-games-menu
		if (auto node = dynamic_cast<CCMenu*>(this->getChildByID("more-games-menu"))) node->setOpacity(9.f);
		//main-menu
		if (auto mainMenu = dynamic_cast<CCMenu*>(this->getChildByID("main-menu"))) {
			//hr out the 3 things
			mainMenu->addChild(CCLabelBMFont::create("----------", "chatFont.fnt"));
			//add bottom things
			if (auto bottomMenu = dynamic_cast<CCMenu*>(this->getChildByID("bottom-menu"))) {
				for (int i = 0; i < 50; i++) {
					auto inode = cocos::getChild(bottomMenu, i);
					if (inode) {
						inode->setParent(mainMenu);
						mainMenu->addChild(inode);
					}
					bottomMenu->removeFromParentAndCleanup(0);
				}
			}
			//hr out the top right items
			mainMenu->addChild(CCLabelBMFont::create("----------", "chatFont.fnt"));
			//add top right items
			if (auto closeMenu = dynamic_cast<CCMenu*>(this->getChildByID("close-menu"))) {
				for (int i = 0; i < 50; i++) {
					auto inode = cocos::getChild(closeMenu, i);
					if (inode) {
						inode->setParent(mainMenu);
						mainMenu->addChild(inode);
					}
					closeMenu->removeFromParentAndCleanup(0);
				}
			}
			//setup collected stuff
			for (int i = 0; i < mainMenu->getChildrenCount(); i++) {
				auto inode_item = cocos::getChild<CCMenuItemSpriteExtra>(mainMenu, i);
				if (inode_item) updateItemToLabel(inode_item);
			}
			//setup menu
			mainMenu->setScale(0.575f);
			mainMenu->setAnchorPoint({ 0.5f, 0.f });
			mainMenu->setPosition(132.f, 0.f);
			mainMenu->setContentHeight(this->getContentHeight() + 60.f);
			mainMenu->setLayout(ColumnLayout::create()->setAxisAlignment(AxisAlignment::End)->setAxisReverse(1));
		}
		//social-media-menu
		if (auto socialMediaMenu = dynamic_cast<CCMenu*>(this->getChildByID("social-media-menu"))) {
			//setup collected stuff
			for (int i = 0; i < socialMediaMenu->getChildrenCount(); i++) {
				auto inode_item = cocos::getChild<CCMenuItemSpriteExtra>(socialMediaMenu, i);
				if (inode_item) updateItemToLabel(inode_item);
			}
			//setup menu
			socialMediaMenu->setScale(0.5f);
			//socialMediaMenu->setAnchorPoint({ 0.5f, 0.f });
			//socialMediaMenu->setPosition(132.f, 0.f);
			socialMediaMenu->setContentWidth(280.f);
			socialMediaMenu->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start)->setGrowCrossAxis(1));
		}
		//right-side-menu
		if (auto rightSideMenu = dynamic_cast<CCMenu*>(this->getChildByID("right-side-menu"))) {
			//setup collected stuff
			for (int i = 0; i < rightSideMenu->getChildrenCount(); i++) {
				auto inode_item = cocos::getChild<CCMenuItemSpriteExtra>(rightSideMenu, i);
				if (inode_item) updateItemToLabel(inode_item);
			}
			//setup menu
			rightSideMenu->setScale(0.5f);
			rightSideMenu->setAnchorPoint({ 1.f, 1.f });
			rightSideMenu->setPosition(this->getContentSize() - 12);
			//rightSideMenu->setContentWidth(280.f);
			rightSideMenu->setLayout(ColumnLayout::create()->setAxisAlignment(AxisAlignment::End)->setCrossAxisLineAlignment(AxisAlignment::End)->setGrowCrossAxis(1));
		}
		//bg
		this->addChild(CCSprite::create("GJ_gradientBG.png"), -1);
		return true;
	}
};

#include <Geode/modify/CreatorLayer.hpp>
class $modify(CreatorLayerExt, CreatorLayer) {
	bool init() {
		if (!CreatorLayer::init()) return false;
		//creator-buttons-menu
		if (auto creatorButtonsMenu = dynamic_cast<CCMenu*>(this->getChildByID("creator-buttons-menu"))) {
			//setup collected stuff
			for (int i = 0; i < creatorButtonsMenu->getChildrenCount(); i++) {
				auto inode_item = cocos::getChild<CCMenuItemSpriteExtra>(creatorButtonsMenu, i);
				if (inode_item) updateItemToLabel(inode_item);
			}
			//setup menu
			creatorButtonsMenu->setScale(0.65f);
			//creatorButtonsMenu->setAnchorPoint({ 0.5f, 0.f });
			creatorButtonsMenu->setPosition(124.f, this->getContentHeight() / 2);
			creatorButtonsMenu->setContentHeight(this->getContentHeight() - 60.f);
			creatorButtonsMenu->setLayout(ColumnLayout::create()->setAxisAlignment(AxisAlignment::Center)->setAxisReverse(1));
		}
		return true;
	}
};

#include <Geode/modify/PlayerObject.hpp>
class $modify(PlayerObjectExt, PlayerObject) {
	struct Fields {
		float m_lastPlatformerXVelocity = 0.1;
	};
	bool isCube() {
		auto player = this;
		if (!player->m_isShip && !player->m_isBall && !player->m_isBird && !player->m_isDart && !player->m_isRobot && !player->m_isSpider && !player->m_isSwing)
			return true;
		return false;
	}
	void mySch(float) {
		m_fields->m_lastPlatformerXVelocity =
			fabs(this->m_platformerXVelocity) > 0.001f ?
			this->m_platformerXVelocity : m_fields->m_lastPlatformerXVelocity;
		auto mainLayer = this->getChildByIDRecursive("main-layer");
		auto plr_run = dynamic_cast<CCSprite*>(this->getChildByIDRecursive("plr_run"));
		auto plr_jumpup = dynamic_cast<CCSprite*>(this->getChildByIDRecursive("plr_jumpup"));
		auto plr_jumpdown = dynamic_cast<CCSprite*>(this->getChildByIDRecursive("plr_jumpdown"));
		auto plr_stand = dynamic_cast<CCSprite*>(this->getChildByIDRecursive("plr_stand"));
		if (plr_run and plr_jumpup and mainLayer) {
			auto showplr = (this->isCube() or this->m_isRobot) and not this->m_isDead;
			auto jmpup = (not this->m_isOnGround and fabs(m_yVelocity) > 0.1f) or this->m_isDashing;
			auto stand = fabs(this->m_platformerXVelocity) < 2.f and this->m_isPlatformer;
			auto goesLeft = m_fields->m_lastPlatformerXVelocity < 0.f and this->m_isPlatformer;
			mainLayer->setVisible(not showplr);
			;;;;;; plr_run->setVisible(showplr and !jmpup and !stand);
			;;; plr_jumpup->setVisible(showplr and jmpup and m_yVelocity > 0.1f);
			; plr_jumpdown->setVisible(showplr and jmpup and (m_yVelocity < -0.1f) or this->m_isDashing);
			;;;; plr_stand->setVisible(showplr and !jmpup and stand);
			plr_run->setFlipX(goesLeft);
			plr_jumpup->setFlipX(goesLeft);
			plr_jumpdown->setFlipX(goesLeft);
			plr_stand->setFlipX(goesLeft);
			plr_run->setFlipY(this->m_isUpsideDown);
			plr_jumpup->setFlipY(this->m_isUpsideDown);
			plr_jumpdown->setFlipY(this->m_isUpsideDown);
			plr_stand->setFlipY(this->m_isUpsideDown);
			plr_run->setAnchorPoint(this->m_isUpsideDown ? CCPoint(0.5f, 0.66f) : CCPoint(0.5f, 0.43f));
			plr_jumpup->setAnchorPoint(this->m_isUpsideDown ? CCPoint(0.5f, 0.66f) : CCPoint(0.5f, 0.43f));
			plr_jumpdown->setAnchorPoint(this->m_isUpsideDown ? CCPoint(0.5f, 0.66f) : CCPoint(0.5f, 0.43f));
			plr_stand->setAnchorPoint(this->m_isUpsideDown ? CCPoint(0.5f, 0.66f) : CCPoint(0.5f, 0.43f));
		}
		this->m_robotBurstParticles->setVisible(0);
		this->m_iconSprite->setScale(1.f);
		log::debug("{}->{}() m_yVelocity={}", this, __FUNCTION__, m_yVelocity);
	}
	void updateRotation(float p0) {
		if (this->isCube()) {
			this->m_isRobot = 1;
			if (this->m_isOnGround) PlayerObject::updateRotation(p0);
			this->m_isRobot = 0;
		}
		else PlayerObject::updateRotation(p0);
	}
	bool init(int p0, int p1, GJBaseGameLayer * p2, cocos2d::CCLayer * p3, bool p4) {
		if (!PlayerObject::init( p0,  p1, p2, p3, p4)) return false;
		this->schedule(schedule_selector(PlayerObjectExt::mySch));
		auto plr_run = CCSprite::createWithSpriteFrameName("plr_run1.png"_spr);
		if (plr_run) {
			plr_run->setScale(1.4f);
			plr_run->setID("plr_run");
			plr_run->setVisible(0);
			this->addChild(plr_run);
			//frames
			auto frames = new CCArray;
			for (int i = 1; i <= 4; i++)
				frames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fmt::format("plr_run{}.png"_spr, i).data()));
			//animate
			plr_run->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.04f))));
		};
		auto plr_stand = CCSprite::createWithSpriteFrameName("plr_stand1.png"_spr);
		if (plr_stand) {
			plr_stand->setScale(1.4f);
			plr_stand->setID("plr_stand");
			plr_stand->setVisible(0);
			this->addChild(plr_stand);
			//frames
			auto frames = new CCArray;
			for (int i = 1; i <= 10; i++)
				frames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fmt::format("plr_stand{}.png"_spr, i).data()));
			//animate
			plr_stand->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.06f))));
			this->m_iconSprite->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.06f))));
		};
		auto plr_jumpup = CCSprite::createWithSpriteFrameName("plr_jumpup1.png"_spr);
		if (plr_jumpup) {
			plr_jumpup->setScale(1.4f);
			plr_jumpup->setID("plr_jumpup");
			plr_jumpup->setVisible(0);
			this->addChild(plr_jumpup);
			//frames
			auto frames = new CCArray;
			for (int i = 1; i <= 3; i++)
				frames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fmt::format("plr_jumpup{}.png"_spr, i).data()));
			//animate
			plr_jumpup->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.06f))));
		};
		auto plr_jumpdown = CCSprite::createWithSpriteFrameName("plr_jumpdown1.png"_spr);
		if (plr_jumpdown) {
			plr_jumpdown->setScale(1.4f);
			plr_jumpdown->setID("plr_jumpdown");
			plr_jumpdown->setVisible(0);
			this->addChild(plr_jumpdown);
			//frames
			auto frames = new CCArray;
			for (int i = 1; i <= 3; i++)
				frames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fmt::format("plr_jumpdown{}.png"_spr, i).data()));
			//animate
			plr_jumpdown->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimation::createWithSpriteFrames(frames, 0.06f))));
		};
		updatePlayerFrame(1);
		return true;
	}
	void updatePlayerFrame(int p0) {
		PlayerObject::updatePlayerFrame(p0);
		this->m_iconSprite->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("plr_stand1.png"_spr));
		this->m_iconSpriteSecondary->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("emptyGlow.png"));
		this->m_iconSpriteWhitener->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("emptyGlow.png"));
		this->m_iconGlow->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("emptyGlow.png"));
	}
};

#endif // 1
