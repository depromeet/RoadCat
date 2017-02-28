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
    double startInitialTime;
    cocos2d::Label* modeLabel;
    
    std::list<CatInfo> catInfosForSaving;
    std::list<CatInfo> catInfosForLoading;
    
    std::list<cocos2d::Sprite*> catSprList;
    cocos2d::Sprite* panelCatSpr;
    
    //visibleSize
    cocos2d::Size vs;
public:
    enum eModeType{ PLAYING, SAVING };
    enum eTouchType{ RED, BLUE };
    enum eComboType{ NONE, GOOD, EXCELLENT, PERFECT };
    
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    virtual void onEnterTransitionDidFinish() override;
    
    void modeChange();
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void update(float dt);
    void touchScreen(eTouchType type);
    
    void makeCatInRoad(float initPosY, eTouchType type);
    void loadCatInfoCsv();
    void saveCatInfoCsv();
    
    eComboType checkComboInPanel(cocos2d::Vec2 catPos);
    
    void updateCat(float dt);
    void playCorrectEffect(eComboType type);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
