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
                                                 "res/main/description_menu.png"
                                                 );
    
    descriptionItem->setCallback(CC_CALLBACK_1(MainScene::descriptionCallback, this));
    
    auto startItem = MenuItemImage::create("res/main/start_menu.png",
                                                 "res/main/start_menu.png"
                                                 );
    
    startItem->setCallback(CC_CALLBACK_1(MainScene::startCallback, this));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(descriptionItem,startItem, NULL);
    
    menu->setPosition(vs.width*0.5, vs.height*0.5);
    menu->alignItemsHorizontallyWithPadding(20.0f);
    
    this->addChild(menu, 1);
    
    return true;
}

void MainScene::startCallback(Ref* sender)
{
    Director::getInstance()->replaceScene(TransitionSlideInR::create(1.0f, HelloWorld::createScene()));
}
void MainScene::descriptionCallback(Ref* sender)
{
    
}