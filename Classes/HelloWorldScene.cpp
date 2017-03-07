#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"

//#define __MAKING_MODE

USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
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
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + vs.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("background.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(vs.width/2 + origin.x, vs.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = [&](Touch* touch, Event* event){
        auto touchPos = touch->getLocation();
        auto visibleWidth = Director::getInstance()->getVisibleSize().width;
        
        if(visibleWidth*0.5 >= touchPos.x)
            touchScreen(eTouchType::RED);
        else
            touchScreen(eTouchType::BLUE);
        
        return true;
    };
    
    //std::string readDataStr = FileUtils::getInstance()->getStringFromFile("cat_pattern.csv");
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    startInitialTime = 3.0f;
    timer = 0.0;

    panelCatSpr = Sprite::create("res/feed.png");
    
    panelCatSpr->setPosition(panelCatSpr->getContentSize().width*0.5+50.0f,
                             vs.height*0.5-13);
    
    addChild(panelCatSpr);
    
#ifdef __MAKING_MODE
    auto descriptionLabel = Label::createWithTTF("PLAY mode가 되면 자동으로 edit 된 내용이 저장됩니다!", "fonts/main_font.ttf", 30.0f);
    
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
    
    loadCatInfoCsv();
    
    return true;
}

void HelloWorld::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    schedule(schedule_selector(HelloWorld::updateCat));
    
    
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
void HelloWorld::update(float dt)
{
    timer += dt;
}

void HelloWorld::saveCatInfoCsv()
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
void HelloWorld::loadCatInfoCsv()
{
    catInfosForLoading.clear();

#ifdef __MAKING_MODE
    std::string writablePath = FileUtils::getInstance()->getWritablePath();
    std::string fileName = "write_cat_info.csv";
    
    std::string fullPath = writablePath + fileName;
    
    std::string readDataStr = FileUtils::getInstance()->getStringFromFile(fullPath);
    
    Data datas = FileUtils::getInstance()->getDataFromFile(fullPath);
#else
    Data datas = FileUtils::getInstance()->getDataFromFile("res/stage/stage_1.csv");
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
void HelloWorld::updateCat(float dt)
{
    if(catInfosForLoading.empty())
    {
        unschedule(schedule_selector(HelloWorld::updateCat));
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
void HelloWorld::makeCatInRoad(float initPosY, eTouchType type)
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
        this->playCorrectEffect(eComboType::NONE);
        this->removeChild(catSprList.front());
        this->catSprList.pop_front();
    }),nullptr);
    
    spr->runAction(seq);
    
    addChild(spr);
    catSprList.push_back(spr);
}
void HelloWorld::modeChange()
{
    if(modeLabel->getTag() == eModeType::PLAYING)
    {
        //SAVING 모드 일 때
        scheduleUpdate();
        
        unschedule(schedule_selector(HelloWorld::updateCat));
        
        modeLabel->setTag(eModeType::SAVING);
        modeLabel->setString("SAVING mode");
    }
    else if(modeLabel->getTag() == eModeType::SAVING)
    {
        //PLAYING 모드 일 때
        unscheduleUpdate();
        
        saveCatInfoCsv();
        loadCatInfoCsv();
        
        schedule(schedule_selector(HelloWorld::updateCat));
        
        modeLabel->setTag(eModeType::PLAYING);
        modeLabel->setString("PLAYING mode");
    }
    
    timer = 0.0;
}
void HelloWorld::playCorrectEffect(eComboType type)
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
void HelloWorld::touchScreen(eTouchType type)
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
                playCorrectEffect(eComboType::NONE);
            }
            else
            {
                auto seq = Sequence::create(Blink::create(0.5f, 5), Show::create(), NULL);
                panelCatSpr->runAction(seq);
                
                playCorrectEffect((eComboType)comboType);
            }
            removeChild(catSprList.front());
            catSprList.pop_front();
            
        }
#ifdef __MAKING_MODE
    }
#endif
}

HelloWorld::eComboType HelloWorld::checkComboInPanel(Vec2 catPos)
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

void HelloWorld::menuCloseCallback(Ref* pSender)
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
