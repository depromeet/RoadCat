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
    loadTimer = 0.0;
    
    scheduleUpdate();
    
    auto saveBtn = Button::create("CloseNormal.png");
    
    saveBtn->setPosition(Vec2(visibleSize.width*0.5,saveBtn->getContentSize().height));
    
    saveBtn->addClickEventListener([&](Ref* sender) {
        saveCatInfoCsv();
    });
    addChild(saveBtn);
    
    return true;
}
void HelloWorld::update(float dt)
{
    timer += dt;
}

void HelloWorld::saveCatInfoCsv()
{
    if(catInfos.empty())
        return;
    
    std::string writablePath = FileUtils::getInstance()->getWritablePath();
    std::string fileName = "write_cat_info.csv";
    
    std::string fullPath = writablePath + fileName;
    // writeTest
    Data data;
    
    std::vector<CatInfo> newInfos;
    
    for(const auto& info : catInfos)
        newInfos.push_back(info);

    data.copy((unsigned char*) newInfos.data(), catInfos.size() * sizeof(CatInfo));
    
    FileUtils::getInstance()->writeDataToFile(data, fullPath);
}
void HelloWorld::loadCatInfoCsv()
{
    catInfos.clear();
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
        
        catInfos.push_back(ci);
    }
}
void HelloWorld::updateCat(float dt)
{
    if(catInfos.empty())
    {
        unschedule(schedule_selector(HelloWorld::updateCat));
        return;
    }
    
    CatInfo frontCatInfo = catInfos.front();
    
    
    if( loadTimer + dt >= frontCatInfo.intervalTime )
    {
        auto spr = Sprite::create("cat.png");
        
        if(frontCatInfo.type == eTouchType::BLUE)
            spr->setColor(Color3B::BLUE);
        else if(frontCatInfo.type == eTouchType::RED)
            spr->setColor(Color3B::RED);
        auto visibleSize = Director::getInstance()->getVisibleSize();
        
        auto sprSize = spr->getContentSize();
        
        spr->setPosition(visibleSize.width + sprSize.width*0.5,visibleSize.height*0.5);
        auto movAction = MoveBy::create(2.0f, Vec2(-(visibleSize.width + sprSize.width),0));
        
        auto seq = Sequence::create(movAction,RemoveSelf::create(),NULL);
        
        spr->runAction(seq);

        addChild(spr);
        
        catInfos.pop_front();
        loadTimer = 0.0;
    }
    else
        loadTimer += dt;
    
    
}
void HelloWorld::touchScreen(eTouchType type)
{
    CatInfo newCatInfo;
    newCatInfo.type = type;
    newCatInfo.intervalTime = timer;
    
    catInfos.push_back(newCatInfo);
    
    auto spr = Sprite::create("cat.png");
    
    if(type == eTouchType::RED)
        spr->setColor(Color3B::RED);
    else if(type == eTouchType::BLUE)
        spr->setColor(Color3B::BLUE);
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto sprSize = spr->getContentSize();
    
    spr->setPosition(visibleSize.width + sprSize.width*0.5,visibleSize.height*0.5);
    
    auto movAction = MoveBy::create(2.0f, Vec2(-(visibleSize.width + sprSize.width),0));
    
    auto seq = Sequence::create(movAction,RemoveSelf::create(),NULL);
    
    spr->runAction(seq);
    
    addChild(spr);

    timer = 0.0;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    saveCatInfoCsv();
    loadCatInfoCsv();
    
    schedule(schedule_selector(HelloWorld::updateCat));
    
    
    //Close the cocos2d-x game scene and quit the application
    //Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
