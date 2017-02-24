#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

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
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

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
    
    timer = 0.0;

    
    auto descriptionLabel = Label::createWithTTF("PLAY mode가 되면 자동으로 edit 된 내용이 저장됩니다!", "fonts/arial.ttf", 30.0f);
    
    modeLabel = Label::createWithTTF("PLAY mode", "fonts/arial.ttf", 30.0f);
    modeLabel->setTag(eModeType::PLAYING);
    
    descriptionLabel->setPosition(visibleSize.width*0.5, visibleSize.height*0.5+200);
    
    addChild(descriptionLabel);
    
    modeLabel->setPosition(descriptionLabel->getContentSize().width/2, -descriptionLabel->getContentSize().height);
    descriptionLabel->addChild(modeLabel);
    
    auto modeChangeButton = Button::create("CloseNormal.png");
    
    modeChangeButton->setPosition(Vec2(modeChangeButton->getContentSize().width,modeChangeButton->getContentSize().height));
    modeChangeButton->addClickEventListener([&](Ref* sender){
        modeChange();
    });
    addChild(modeChangeButton);
    
    auto saveBtn = Button::create("CloseNormal.png");
    
    saveBtn->setPosition(Vec2(visibleSize.width*0.5,saveBtn->getContentSize().height));
    
    saveBtn->addClickEventListener([&](Ref* sender) {
        saveCatInfoCsv();
    });
    addChild(saveBtn);
    
    loadCatInfoCsv();
    
    return true;
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
    // writeTest
    Data data;
    
    std::vector<CatInfo> newInfos;
    
    for(const auto& info : catInfosForSaving)
        newInfos.push_back(info);

    data.copy((unsigned char*) newInfos.data(), catInfosForSaving.size() * sizeof(CatInfo));
    
    FileUtils::getInstance()->writeDataToFile(data, fullPath);
    
    catInfosForSaving.clear();
}
void HelloWorld::loadCatInfoCsv()
{
    catInfosForLoading.clear();
    std::string writablePath = FileUtils::getInstance()->getWritablePath();
    std::string fileName = "write_cat_info.csv";
    
    std::string fullPath = writablePath + fileName;
    
    std::string readDataStr = FileUtils::getInstance()->getStringFromFile(fullPath);
    
    Data datas = FileUtils::getInstance()->getDataFromFile(fullPath);
    
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
        auto visibleSize = Director::getInstance()->getVisibleSize();
        
        makeCatInRoad(visibleSize.height*0.5, (eTouchType)frontCatInfo.type);
    
        catInfosForLoading.pop_front();
        timer = 0.0;
    }
    else
        timer += dt;
    
    
}
void HelloWorld::makeCatInRoad(float initPosY, eTouchType type)
{
    auto spr = Sprite::create("cat.png");
    
    if(type == eTouchType::RED)
        spr->setColor(Color3B::RED);
    else if(type == eTouchType::BLUE)
        spr->setColor(Color3B::BLUE);
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto sprSize = spr->getContentSize();
    
    spr->setPosition(visibleSize.width + sprSize.width*0.5,initPosY);
    
    auto movAction = MoveBy::create(2.0f, Vec2(-(visibleSize.width + sprSize.width),0));
    
    auto seq = Sequence::create(movAction,RemoveSelf::create(),NULL);
    
    spr->runAction(seq);
    
    addChild(spr);
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
void HelloWorld::touchScreen(eTouchType type)
{
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
