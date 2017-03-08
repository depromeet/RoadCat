#ifndef __GameScene_SCENE_H__
#define __GameScene_SCENE_H__

#include "cocos2d.h"

class GameOver : public cocos2d::Layer
{
    cocos2d::Label* successScore;
    float dtSum;
public:
    bool init();
    
    void update(float dt);

    CREATE_FUNC(GameOver);
};
struct CatInfo
{
    int type;
    float intervalTime;
};

class GameScene : public cocos2d::Layer
{
protected:
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

    virtual bool init();
    
    // a selector callback
    virtual void onEnterTransitionDidFinish() override;
    
    void modeChange();
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void update(float dt);
    void touchScreen(eTouchType type);
    
    void makeCatInRoad(float initPosY, eTouchType type);
    virtual void loadCatInfoCsv(std::string csvSrc);
    void saveCatInfoCsv();
    
    eComboType checkComboInPanel(cocos2d::Vec2 catPos);
    
    virtual void updateCat(float dt) = 0;
    
    void playCorrectEffect(eComboType type);
    
    void removeFrontCat();
};

class TutorialScene : public GameScene
{
    int tutorialStep;
public:
    bool init() override;
    
    static cocos2d::Scene* createScene();
    
    void updateCat(float dt) override;
    void showTutorialByStep();
    
    CREATE_FUNC(TutorialScene);
};

class EasyScene : public GameScene
{
public:
    bool init() override;

    static cocos2d::Scene* createScene();
    
    void updateCat(float dt) override;
    
    CREATE_FUNC(EasyScene);
};

#endif // __GameScene_SCENE_H__
