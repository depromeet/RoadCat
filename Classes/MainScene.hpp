//
//  MainScene.hpp
//  RoadCat
//
//  Created by jang on 2017. 2. 28..
//
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include "cocos2d.h"

class DifficultyLayer : public cocos2d::Layer
{
public:
    bool init() override;
    
    CREATE_FUNC(DifficultyLayer);
};

class MainScene : public cocos2d::Layer
{
    cocos2d::Size vs;
public:
    bool init() override;
    
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(MainScene);
};
#endif /* MainScene_hpp */
