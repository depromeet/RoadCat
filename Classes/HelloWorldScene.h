#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

struct CatInfo
{
    int type;
    float intervalTime;
};

class HelloWorld : public cocos2d::Layer
{
    double timer;
    
    cocos2d::Label* modeLabel;
    
    std::list<CatInfo> catInfosForSaving;
    std::list<CatInfo> catInfosForLoading;
public:
    enum eModeType{ PLAYING, SAVING };
    enum eTouchType{ RED, BLUE };
    
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void modeChange();
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void update(float dt);
    void touchScreen(eTouchType type);
    
    void makeCatInRoad(float initPosY, eTouchType type);
    void loadCatInfoCsv();
    void saveCatInfoCsv();
    
    void updateCat(float dt);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
