#include "GameScene.h"
#include "MainScene.hpp"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"

//#define __MAKING_MODE

USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;


void GameOver::setGameOverInfos(const GameOverInfos& gameOverInfos)
{
    score->setString(StringUtils::format("%d",gameOverInfos.score));
    combo->setString(StringUtils::format("%d",gameOverInfos.combo));
    miss->setString(StringUtils::format("%d",gameOverInfos.miss));
}
bool GameOver::init()
{
    if(!Layer::init())
        return false;
    
    auto vs = Director::getInstance()->getVisibleSize();
    
    auto background = Sprite::create("res/main/background.png");
    
    background->setPosition(vs.width*0.5f,vs.height*0.5f);
    addChild(background);
    
    auto nextBtn = Button::create("res/over/next_button.png");
    auto nextCS = nextBtn->getContentSize();
    
    nextBtn->setPosition(Vec2(vs.width-30.0f-nextCS.width*0.5, 30.0f + nextCS.height*0.5f));
    
    nextBtn->addClickEventListener([](Ref*){
        Director::getInstance()->replaceScene(MainScene::createScene());
    });
    addChild(nextBtn);
    
    auto scoreSpr = Sprite::create("res/over/score_button.png");
    auto scoreCS = scoreSpr->getContentSize();
    
    scoreSpr->setPosition(90.0f + scoreCS.width*0.5f, vs.height - 40.0f - scoreCS.height*0.5f);
    
    addChild(scoreSpr);
    
    auto scoreCatSpr = Sprite::create("res/over/score_cat.png");
    auto scoreCatCS = scoreCatSpr->getContentSize();
    
    scoreCatSpr->setPosition(307.0f + scoreCatCS.width*0.5f, vs.height-56.0f-scoreCatCS.height*0.5f);
    
    addChild(scoreCatSpr);
    
    auto totalBack = Sprite::create("res/over/total_background.png");
    auto totalBackCS = totalBack->getContentSize();
    
    totalBack->setPosition(150.0f,vs.height - 107.0f - totalBackCS.height*0.5f);
    
    addChild(totalBack);
    
    score = Label::createWithTTF("0", "fonts/main_font.ttf", 25.0f,Size(150.0f,30.0f),TextHAlignment::RIGHT);
    
    score->setPosition(110.0f,50.0f);
    score->enableOutline(Color4B::BLACK,1);
    
    miss = Label::createWithTTF("0", "fonts/main_font.ttf", 25.0f,Size(150.0f,30.0f),TextHAlignment::RIGHT);
    
    miss->setPosition(110.0f,90.0f);
    miss->enableOutline(Color4B::BLACK,1);
    
    combo = Label::createWithTTF("0", "fonts/main_font.ttf", 25.0f,Size(150.0f,30.0f),TextHAlignment::RIGHT);
    
    combo->setPosition(110.0f,130.0f);
    combo->enableOutline(Color4B::BLACK,1);
    
    totalBack->addChild(score);
    totalBack->addChild(miss);
    totalBack->addChild(combo);
    
    scheduleUpdate();
    
    return true;
}

void GameOver::update(float dt)
{
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    vs = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameScene::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + vs.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    // add "GameScene" splash screen"
    auto sprite = Sprite::create("background.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(vs.width/2 + origin.x, vs.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, -10);
    
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan,this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    startInitialTime = 3.0f;
    timer = 0.0;

    panelCatSpr = Sprite::create("res/feed.png");
    
    panelCatSpr->setPosition(panelCatSpr->getContentSize().width*0.5+50.0f,
                             vs.height*0.5-13);
    
    addChild(panelCatSpr);
    
#ifdef __MAKING_MODE
    auto descriptionLabel = Label::createWithTTF("PLAY mode가 되면 자동으로 edit 된 내용이 저장됩니다!", "fonts/kr_font.ttf", 30.0f);
    
    modeLabel = Label::createWithTTF("PLAY mode", "fonts/arial.ttf", 30.0f);
    modeLabel->setTag(eModeType::PLAYING);
    
    descriptionLabel->setPosition(vs.width*0.5, vs.height*0.5);
    
    addChild(descriptionLabel);
    
    modeLabel->setPosition(descriptionLabel->getContentSize().width/2, -descriptionLabel->getContentSize().height);
    descriptionLabel->addChild(modeLabel);
    
    auto modeChangeButton = Button::create("CloseNormal.png");

    modeChangeButton->setPosition(Vec2(modeChangeButton->getContentSize().width,modeChangeButton->getContentSize().height));
    modeChangeButton->addClickEventListener([&](Ref* sender){
        modeChange();
    });
    addChild(modeChangeButton);
#endif
    
    return true;
}
bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
    auto touchPos = touch->getLocation();
    auto visibleWidth = Director::getInstance()->getVisibleSize().width;
    
    if(visibleWidth*0.5 >= touchPos.x)
        touchScreen(eTouchType::RED);
    else
        touchScreen(eTouchType::BLUE);
    
    return true;
}
void GameScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    schedule(schedule_selector(GameScene::updateCat));
    
    SimpleAudioEngine::getInstance()->playEffect("ready_go.mp3");
    
    auto countLabel = Label::createWithTTF("READY", "fonts/main_font.ttf", 50.0f);
    
    countLabel->setPosition(vs.width+countLabel->getContentSize().width*0.5,
                            vs.height*0.5 );
    
    countLabel->enableOutline(Color4B::ORANGE,1);
    
    auto readyGoAction =
    Sequence::create(
                     MoveTo::create(0.6f, Vec2(vs.width*0.5,vs.height*0.5)),
                     DelayTime::create(0.45f),
                     CallFuncN::create([](Node* sender){
        auto lab = static_cast<Label*>(sender);
        lab->setString("GO!");
    }),
                     DelayTime::create(0.5f),
                     MoveTo::create(0.6f,
                                    Vec2(-vs.width*0.5-countLabel->getContentSize().width*0.5,vs.height*0.5)),
                     CallFuncN::create([](Node* sender){
        SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3",true);
        sender->removeFromParentAndCleanup(true);
    }),
                     nullptr);
    
    countLabel->runAction(readyGoAction);
    
    addChild(countLabel);
}
void GameScene::update(float dt)
{
    timer += dt;
}

void GameScene::saveCatInfoCsv()
{
    if(catInfosForSaving.empty())
        return;
    
    std::string writablePath = FileUtils::getInstance()->getWritablePath();
    std::string fileName = "write_cat_info.csv";
    
    std::string fullPath = writablePath + fileName;
    
    log("fullPath : %s", fullPath.c_str());
    // writeTest
    Data data;
    
    std::vector<CatInfo> newInfos;
    
    for(const auto& info : catInfosForSaving)
        newInfos.push_back(info);

    data.copy((unsigned char*) newInfos.data(), newInfos.size() * sizeof(CatInfo));
    
    FileUtils::getInstance()->writeDataToFile(data, fullPath);
    
    catInfosForSaving.clear();
}
void GameScene::loadCatInfoCsv(std::string csvSrc)
{
    catInfosForLoading.clear();

#ifdef __MAKING_MODE
    std::string writablePath = FileUtils::getInstance()->getWritablePath();
    std::string fileName = "write_cat_info.csv";
    
    std::string fullPath = writablePath + fileName;
    
    std::string readDataStr = FileUtils::getInstance()->getStringFromFile(fullPath);
    
    Data datas = FileUtils::getInstance()->getDataFromFile(fullPath);
#else
    Data datas = FileUtils::getInstance()->getDataFromFile(csvSrc);
#endif
    
    CatInfo* buffer = (CatInfo*) datas.getBytes();
    ssize_t length = datas.getSize() / sizeof(CatInfo);
    
    for (int i = 0; i < length; i++)
    {
        CatInfo ci;
        ci.type = buffer[i].type;
        ci.intervalTime = buffer[i].intervalTime;
        
        catInfosForLoading.push_back(ci);
    }
}
void GameScene::makeCatInRoad(float initPosY, eTouchType type)
{
    auto animation = Animation::create();
    for( int i=1;i<=2;i++)
    {
        char szName[100] = {0};
        std::string format;
        
        if(type == eTouchType::RED)
            format = "res/cat%02d.png";
        else
            format = "res/white_cat%02d.png";
        
        sprintf(szName, format.c_str(), i);
        animation->addSpriteFrameWithFile(szName);
    }
    // should last 2.8 seconds. And there are 14 frames.
    
    animation->setDelayPerUnit(0.1 / 2.0f);
    animation->setLoops(-1);
    
    auto spr = Sprite::createWithSpriteFrame(animation->getFrames().front()->getSpriteFrame());
    
    spr->setTag(type);
    
    auto sprSize = spr->getContentSize();
    
    spr->setPosition(vs.width + sprSize.width*0.5,initPosY);
    
    auto movAction = MoveBy::create(2.0f, Vec2(-(vs.width + sprSize.width),0));
    
    spr->runAction(RepeatForever::create(Animate::create(animation)));
    
    auto seq = Sequence::create(movAction,CallFunc::create([this](){
        this->playerCorrect(eComboType::NONE);
        this->removeFrontCat();
    }),nullptr);
    
    spr->runAction(seq);
    
    addChild(spr);
    catSprList.push_back(spr);
    
    score = 0;
    
    scoreLabel = Label::createWithTTF("0", "fonts/main_font.ttf", 25.0f,Size(400,30.0f), TextHAlignment::RIGHT);
    auto scoreCS = scoreLabel->getContentSize();
    
    scoreLabel->enableOutline(Color4B::ORANGE,1);
    scoreLabel->setPosition(vs.width - scoreCS.width*0.5f, vs.height - scoreCS.height*0.5f);
    
    addChild(scoreLabel);
    
    ////
    
    combo = 0;
    
    miss = 0;
}
void GameScene::removeFrontCat()
{
    this->removeChild(catSprList.front());
    this->catSprList.pop_front();
    
    //게임 오버.
#ifndef __MAKING_MODE
    if(catSprList.empty())
    {
        auto gameOver = GameOver::create();
        
        GameOverInfos gameOverInfos;
        
        gameOverInfos.combo = combo;
        gameOverInfos.score = score;
        gameOverInfos.miss = miss;
        
        gameOver->setGameOverInfos(gameOverInfos);
        this->addChild(gameOver);
    }
#endif
}

void GameScene::modeChange()
{
#ifdef __MAKING_MODE
    if(modeLabel->getTag() == eModeType::PLAYING)
    {
        //SAVING 모드 일 때
        scheduleUpdate();
        
        unschedule(schedule_selector(GameScene::updateCat));
        
        modeLabel->setTag(eModeType::SAVING);
        modeLabel->setString("SAVING mode");
    }
    else if(modeLabel->getTag() == eModeType::SAVING)
    {
        //PLAYING 모드 일 때
        unscheduleUpdate();
        
        saveCatInfoCsv();
        loadCatInfoCsv("write_cat_info.csv");
        
        schedule(schedule_selector(GameScene::updateCat));
        
        modeLabel->setTag(eModeType::PLAYING);
        modeLabel->setString("PLAYING mode");
    }
    
    timer = 0.0;
#endif
}
void GameScene::playerCorrect(eComboType type)
{
    playCorrectEffect(type);
    
    switch(type)
    {
        case eComboType::GOOD:
            score += 10;
            break;
        case eComboType::EXCELLENT:
            score += 20;
            break;
        case eComboType::PERFECT:
            score += 30;
            break;
    }
    
    if(type != eComboType::NONE)
    {
        combo++;
        
        auto comboFormat = StringUtils::format("Combo %d",combo);
        auto comboLabel = Label::createWithTTF(comboFormat, "fonts/arial.ttf", 50.0f);
        
        auto moveAct = MoveBy::create(0.5f, Vec2(0,30.0f));
        auto comboAct = Sequence::create(moveAct,RemoveSelf::create(), NULL);
        
        comboLabel->setColor(Color3B::MAGENTA);
        comboLabel->enableItalics();
        comboLabel->enableOutline(Color4B::BLACK,2);
        comboLabel->runAction(comboAct);
        
        comboLabel->setPosition(vs.width*0.5, vs.height*0.5);
        
        addChild(comboLabel);
    }
    else
    {
        combo = 0;
        miss++;
    }
    
    scoreLabel->setString(StringUtils::format("%d",score));
}
void GameScene::playCorrectEffect(eComboType type)
{
    Sprite* spr = nullptr;
    
    switch (type) {
        case eComboType::PERFECT:
            spr = Sprite::create("res/perfect.png");
            SimpleAudioEngine::getInstance()->playEffect("perfect.mp3");
            break;
            
        case eComboType::GOOD:
            spr = Sprite::create("res/good.png");
            SimpleAudioEngine::getInstance()->playEffect("good.mp3");
            break;
            
        case eComboType::EXCELLENT:
            spr = Sprite::create("res/excellent.png");
            SimpleAudioEngine::getInstance()->playEffect("excellent.mp3");
            break;
            
        case eComboType::NONE:
            spr = Sprite::create("res/miss.png");
            SimpleAudioEngine::getInstance()->playEffect("miss.mp3");
            break;
    }
    if(spr != nullptr)
    {
        spr->setPosition(panelCatSpr->getPosition());
        
        auto spwn = Spawn::create(MoveBy::create(1.0f, Vec2(0,100.0f)),FadeOut::create(1.0f),NULL );
        
        auto effectAction = Sequence::create(spwn,RemoveSelf::create(), NULL);
        
        spr->runAction(effectAction);
        spr->setScale(0.2f);
        
        addChild(spr);
    }
}
void GameScene::touchScreen(eTouchType type)
{
#ifdef __MAKING_MODE
    if(modeLabel->getTag() == eModeType::SAVING)
    {
        CatInfo newCatInfo;
        newCatInfo.type = type;
        newCatInfo.intervalTime = timer;
    
        catInfosForSaving.push_back(newCatInfo);
    
        auto visibleSize = Director::getInstance()->getVisibleSize();
    
        makeCatInRoad(visibleSize.height*0.5, type);

        timer = 0.0;
    }
    else if(modeLabel->getTag() == eModeType::PLAYING)
    {
#endif
        if(catSprList.empty())
            return;
        
        eComboType comboType = checkComboInPanel(catSprList.front()->getPosition());
        
        if(comboType != eComboType::NONE) //맞췄으면
        {
            if(catSprList.front()->getTag() != type)
            {
                playerCorrect(eComboType::NONE);
            }
            else
            {
                auto seq = Sequence::create(Blink::create(0.5f, 5), Show::create(), NULL);
                panelCatSpr->runAction(seq);
                
                playerCorrect((eComboType)comboType);
            }
            removeFrontCat();
        }
#ifdef __MAKING_MODE
    }
#endif
}

GameScene::eComboType GameScene::checkComboInPanel(Vec2 catPos)
{
    float dist = fabs(panelCatSpr->getPositionX() - catPos.x);
    float panelWidth = panelCatSpr->getContentSize().width*0.5 + 50;
    
    if(dist > panelWidth)
        return NONE;
    else if(dist > panelWidth*0.5)
        return GOOD;
    else if(dist > panelWidth*0.2)
        return EXCELLENT;
    else
        return PERFECT;
}

void GameScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}

Scene* EasyScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = EasyScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool EasyScene::init()
{
    if(!GameScene::init())
        return false;
    
    loadCatInfoCsv("res/stage/easy.csv");
    
    return true;
}
void EasyScene::updateCat(float dt)
{
    if(catInfosForLoading.empty())
    {
        unschedule(schedule_selector(GameScene::updateCat));
        
        return;
    }
    
    CatInfo frontCatInfo = catInfosForLoading.front();
    
    if( timer + dt >= frontCatInfo.intervalTime )
    {
        makeCatInRoad(vs.height*0.5, (eTouchType)frontCatInfo.type);
        
        catInfosForLoading.pop_front();
        timer = 0.0;
    }
    else
        timer += dt;
}


Scene* TutorialScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TutorialScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool TutorialScene::init()
{
    if(!GameScene::init())
        return false;
    
    loadCatInfoCsv("res/stage/easy.csv");
    
    tutorialStep = 1;
    
    schedule(schedule_selector(TutorialScene::checkTutotialLayer));
    
    return true;
}
void TutorialScene::checkTutotialLayer(float dt)
{
    if(!catSprList.empty())
        if(checkComboInPanel(catSprList.front()->getPosition()))
            showTutorialByStep();
}
void TutorialScene::showTutorialByStep()
{
    Director::getInstance()->pause();
    
    auto back = Sprite::create("res/tutorial/filter.png");
    
    back->setPosition(vs.width*0.5f,vs.height*0.5f);
    back->setName("tutorial_layer");
    
    addChild(back,-8);
    
    Label* descriptLabel;
    auto arrowSpr = Sprite::create("res/tutorial/pointer.png");
    
    if(tutorialStep == 1)
    {
        descriptLabel = Label::createWithTTF("고양이가 물고기와 만나면 화면을 터치해서 먹이를 주세요",
                                                  "fonts/kr_font.ttf",
                                                  16.0f);
        
        arrowSpr->setPosition(400,vs.height - 112.0f);
    }
    else if(tutorialStep == 2)
    {
        descriptLabel = Label::createWithTTF("무늬가 '있는' 고양이가 나오면 오른쪽을 터치해 주세요",
                                             "fonts/kr_font.ttf",
                                             16.0f);
        
        arrowSpr->setPosition(400,vs.height - 112.0f);
    }
    else if( tutorialStep == 3 )
    {
        descriptLabel = Label::createWithTTF("무늬가 '없는' 고양이가 나오면 왼쪽을 터치해 주세요",
                                             "fonts/kr_font.ttf",
                                             16.0f);
        arrowSpr->setFlippedX(true);
        arrowSpr->setPosition(79.0f,vs.height - 112.0f);
    }
    
    back->addChild(arrowSpr);
    
    //descriptLabel->setAdditionalKerning(5.0f);
    descriptLabel->setPosition(vs.width*0.5f, vs.height - 60.0f);
    back->addChild(descriptLabel);
}

bool TutorialScene::onTouchBegan(Touch* touch, Event* event)
{
    if(!GameScene::onTouchBegan(touch, event))
        return false;
    
    auto tutorialLayer = getChildByName("tutorial_layer");
    
    if(tutorialLayer != nullptr) //튜토리얼을 보고 있는 상태면
    {
        removeChild(tutorialLayer);
        Director::getInstance()->resume();
        tutorialStep++;
    }
    
    return true;
}
void TutorialScene::updateCat(float dt)
{
    if(catInfosForLoading.empty())
    {
        unschedule(schedule_selector(GameScene::updateCat));
        
        return;
    }
    
    CatInfo frontCatInfo = catInfosForLoading.front();
    
    if( timer + dt >= frontCatInfo.intervalTime )
    {
        makeCatInRoad(vs.height*0.5, (eTouchType)frontCatInfo.type);
        
        catInfosForLoading.pop_front();
        timer = 0.0;
    }
    else
        timer += dt;

}