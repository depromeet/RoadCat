//
//  MainScene.cpp
//  RoadCat
//
//  Created by jang on 2017. 2. 28..
//
//

#include "MainScene.hpp"
#include "HelloWorldScene.h"

USING_NS_CC;

bool DifficultyLayer::init()
{
    if(!Layer::init())
        return false;
    
    auto background = Sprite::create("res/main/background.png");
    
    auto vs = Director::getInstance()->getVisibleSize();
    
    background->setPosition(vs.width*0.5,vs.height*0.5);
    
    addChild(background);
    
    auto easyItem = MenuItemImage::create("res/main/easy_button.png",
                                                 "res/main/easy_button.png",
                                                 [](Ref* sender){
                                                     Director::getInstance()->replaceScene(TransitionSlideInR::create(1.0f, HelloWorld::createScene()));
                                                 });
    
    auto mediumItem = MenuItemImage::create("res/main/medium_button.png",
                                           "res/main/medium_button.png",
                                           [](Ref* sender){
                                           });
    auto hardItem = MenuItemImage::create("res/main/hard_button.png",
                                           "res/main/hard_button.png",
                                           [](Ref* sender){
                                           });
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(easyItem,mediumItem,hardItem, NULL);
    
    menu->setPosition(vs.width*0.5, vs.height*0.5f);
    menu->alignItemsVerticallyWithPadding(26.0f);
    
    this->addChild(menu);

    return true;
}
Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool MainScene::init()
{
    if(!Layer::init())
        return false;
    
    vs = Director::getInstance()->getVisibleSize();
    auto backSpr = Sprite::create("res/main/background.png");
    
    backSpr->setPosition(vs.width*0.5,vs.height*0.5);
    
    addChild(backSpr);
    
    auto descriptionItem = MenuItemImage::create("res/main/description_menu.png",
                                                  "res/main/description_menu.png",
                                                 [](Ref* sender){
                                                 });
    
    auto startItem = MenuItemImage::create("res/main/start_menu.png",
                                           "res/main/start_menu.png",
                                           [this](Ref* sender){
                                               auto diftyLayer = DifficultyLayer::create();
                                               
                                               this->addChild(diftyLayer,2);
                                           });
    
    auto scoreItem = MenuItemImage::create("res/main/score_menu.png",
                                           "res/main/score_menu.png",
                                           [](Ref* sender){
                                           });
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(descriptionItem,startItem,scoreItem, NULL);
    
    menu->setPosition(vs.width*0.5, 44.0f + scoreItem->getContentSize().height*0.5);
    menu->alignItemsHorizontallyWithPadding(18.0f);
    
    descriptionItem->setPositionY(descriptionItem->getPositionY()+2.0f);
    startItem->setPositionY(startItem->getPositionY() + 9.0f);
    
    auto upAndDown = RepeatForever::create(
                                           Sequence::create(
                                                            MoveBy::create(1.0f,Vec2(0,10.0f)),
                                                            MoveBy::create(1.0f,Vec2(0,-10.0f)),
                                                            NULL)
                                           );
    
    menu->runAction(upAndDown);
    this->addChild(menu, 1);
    
    return true;
}