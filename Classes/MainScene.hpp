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

class MainScene : public cocos2d::Layer
{
    cocos2d::Size vs;
public:
    virtual bool init();
    
    static cocos2d::Scene* createScene();
    
    void startCallback(cocos2d::Ref* sender);
    void descriptionCallback(cocos2d::Ref* sender);
    
    CREATE_FUNC(MainScene);
};

//난이도 선택 레이어
class DifficultyLayer
{
public:
    
};
#endif /* MainScene_hpp */
