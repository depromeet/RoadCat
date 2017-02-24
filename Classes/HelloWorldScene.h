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
    double loadTimer;
    int mode; //play,edit mode
    std::list<CatInfo> catInfos;
public:
    enum eModeType{ PLAY, EDIT };
    enum eTouchType{ RED, BLUE };
    
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void update(float dt);
    void touchScreen(eTouchType type);
    
    void loadCatInfoCsv();
    void saveCatInfoCsv();
    
    void updateCat(float dt);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
