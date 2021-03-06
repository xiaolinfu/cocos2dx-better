#include "ActionTest.h"
#include "../testResource.h"
#include "cocos2d.h"
#include "cocos2d-better.h"
#include "CCArmature.h"
#include "utils/CCArmatureDataManager.h"
#include "display/CCSkin.h"

TESTLAYER_CREATE_FUNC(ActionClipInOut);
TESTLAYER_CREATE_FUNC(ActionFlash);
TESTLAYER_CREATE_FUNC(ActionJumpEx);
TESTLAYER_CREATE_FUNC(ActionMissile);
TESTLAYER_CREATE_FUNC(ActionShake);
TESTLAYER_CREATE_FUNC(ActionTreeFadeInOut);

static NEWTESTFUNC createFunctions[] = {
    CF(ActionClipInOut),
    CF(ActionFlash),
    CF(ActionJumpEx),
    CF(ActionMissile),
    CF(ActionShake),
	CF(ActionTreeFadeInOut),
};

static int sceneIdx=-1;
#define MAX_LAYER (sizeof(createFunctions) / sizeof(createFunctions[0]))

static CCLayer* nextAction()
{
    sceneIdx++;
    sceneIdx = sceneIdx % MAX_LAYER;
    
    CCLayer* pLayer = (createFunctions[sceneIdx])();
    pLayer->init();
    pLayer->autorelease();
    
    return pLayer;
}

static CCLayer* backAction()
{
    sceneIdx--;
    int total = MAX_LAYER;
    if( sceneIdx < 0 )
        sceneIdx += total;
    
    CCLayer* pLayer = (createFunctions[sceneIdx])();
    pLayer->init();
    pLayer->autorelease();
    
    return pLayer;
}

static CCLayer* restartAction()
{
    CCLayer* pLayer = (createFunctions[sceneIdx])();
    pLayer->init();
    pLayer->autorelease();
    
    return pLayer;
}

void ActionTestScene::runThisTest()
{
    sceneIdx = -1;
    addChild(nextAction());

    CCDirector::sharedDirector()->replaceScene(this);
}


std::string ActionDemo::title()
{
    return "ActionTest";
}

std::string ActionDemo::subtitle()
{
    return "";
}

void ActionDemo::onEnter()
{
    CCLayer::onEnter();

    // add title and subtitle
    std::string str = title();
    const char * pTitle = str.c_str();
    CCLabelTTF* label = CCLabelTTF::create(pTitle, "Arial", 32);
    addChild(label, 1);
    label->setPosition( ccp(VisibleRect::center().x, VisibleRect::top().y - 30) );

    std::string strSubtitle = subtitle();
    if( ! strSubtitle.empty() ) 
    {
        CCLabelTTF* l = CCLabelTTF::create(strSubtitle.c_str(), "Thonburi", 16);
        addChild(l, 1);
        l->setPosition( ccp(VisibleRect::center().x, VisibleRect::top().y - 60) );
    }    

    // add menu
    CCMenuItemImage *item1 = CCMenuItemImage::create(s_pPathB1, s_pPathB2, this, menu_selector(ActionDemo::backCallback) );
    CCMenuItemImage *item2 = CCMenuItemImage::create(s_pPathR1, s_pPathR2, this, menu_selector(ActionDemo::restartCallback) );
    CCMenuItemImage *item3 = CCMenuItemImage::create(s_pPathF1, s_pPathF2, this, menu_selector(ActionDemo::nextCallback) );

    CCMenu *menu = CCMenu::create(item1, item2, item3, NULL);

    menu->setPosition(CCPointZero);
    item1->setPosition(ccp(VisibleRect::center().x - item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item2->setPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y+item2->getContentSize().height/2));
    item3->setPosition(ccp(VisibleRect::center().x + item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));

    addChild(menu, 1);
}

void ActionDemo::onExit()
{
    CCLayer::onExit();
}

void ActionDemo::restartCallback(CCObject* pSender)
{
    CCScene* s = new ActionTestScene();
    s->addChild( restartAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void ActionDemo::nextCallback(CCObject* pSender)
{
    CCScene* s = new ActionTestScene();
    s->addChild( nextAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

void ActionDemo::backCallback(CCObject* pSender)
{
    CCScene* s = new ActionTestScene();
    s->addChild( backAction() );
    CCDirector::sharedDirector()->replaceScene(s);
    s->release();
}

//------------------------------------------------------------------
//
// Flash
//
//------------------------------------------------------------------
void ActionFlash::onEnter()
{
    ActionDemo::onEnter();
 
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Files/Cowboy.ExportJson");
    CCArmature* armature = CCArmature::create("Cowboy");
    armature->getAnimation()->playWithIndex(0);
    armature->setPosition(ccp(VisibleRect::center().x, VisibleRect::center().y));
    armature->setScale(0.2f);
    addChild(armature);
    
    armature->runAction(CCRepeatForever::create(CCFlash::create(0.2f, ccWHITE)));
}

std::string ActionFlash::subtitle()
{
    return "Flash";
}

//------------------------------------------------------------------
//
// Clip In & Out
//
//------------------------------------------------------------------
void ActionClipInOut::onEnter()
{
    ActionDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCClippingNode* n = CCClippingNode::create();
    CCSprite* flag = CCSprite::create("Images/usa_flag.jpg");
    CCDrawNode* stencil = CCDrawNode::create();
    n->setStencil(stencil);
    flag->setAnchorPoint(CCPointZero);
    flag->setPosition(CCPointZero);
    n->addChild(flag);
    n->ignoreAnchorPointForPosition(false);
    n->setAnchorPoint(ccp(0.5f, 0.5f));
    n->setContentSize(flag->getContentSize());
    n->setPosition(ccp(origin.x + visibleSize.width / 2,
                       origin.y + visibleSize.height / 2));
    addChild(n, 1);
    
    CCClipIn* in1 = CCClipIn::create(2);
    CCClipOut* out1 = (CCClipOut*)in1->reverse();
    CCClipIn* in2 = CCClipIn::create(2, ccp(1, 1));
    CCClipOut* out2 = (CCClipOut*)in2->reverse();
    CCClipIn* in3 = CCClipIn::create(2, ccp(-1, 1));
    CCClipOut* out3 = (CCClipOut*)in3->reverse();
    CCClipIn* in4 = CCClipIn::create(2, ccp(-1, -1));
    CCClipOut* out4 = (CCClipOut*)in4->reverse();
    CCClipIn* in5 = CCClipIn::create(2, ccp(1, -1));
    CCClipOut* out5 = (CCClipOut*)in5->reverse();
    CCClipIn* in6 = CCClipIn::create(2, ccp(0, -1));
    CCClipOut* out6 = (CCClipOut*)in6->reverse();
    CCClipIn* in7 = CCClipIn::create(2, ccp(0, 1));
    CCClipOut* out7 = (CCClipOut*)in7->reverse();
    CCClipIn* in8 = CCClipIn::create(2, ccp(-1, 0));
    CCClipOut* out8 = (CCClipOut*)in8->reverse();
    n->runAction(CCSequence::create(in1,
                                    out1,
                                    in2,
                                    out2,
                                    in3,
                                    out3,
                                    in4,
                                    out4,
                                    in5,
                                    out5,
                                    in6,
                                    out6,
                                    in7,
                                    out7,
                                    in8,
                                    out8,
                                    NULL));
}

std::string ActionClipInOut::subtitle()
{
    return "Clip In & Out";
}

//------------------------------------------------------------------
//
// JumpBy Extended
//
//------------------------------------------------------------------
void ActionJumpEx::onEnter()
{
    ActionDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
	// grossini 1
	CCSprite* s = CCSprite::create("Images/grossini.png");
	s->setPosition(ccp(origin.x + visibleSize.width / 4,
					   origin.y + visibleSize.height / 5));
	addChild(s);
    
    // jump it
    CCPoint delta = ccp(origin.x + visibleSize.width / 4,
                        origin.y + visibleSize.height * 2 / 5);
    CCJumpByEx* jump = CCJumpByEx::create(1, delta, visibleSize.height / 2, 1, true, 90);
    CCFiniteTimeAction* rJump = (CCFiniteTimeAction*)jump->reverse();
    s->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(jump, rJump)));
    
    // grossini 2
	s = CCSprite::create("Images/grossini.png");
	s->setPosition(ccp(origin.x + visibleSize.width * 3 / 4,
					   origin.y + visibleSize.height / 5));
	addChild(s);
    
    // jump it
    CCPoint dst = ccp(origin.x + visibleSize.width / 2,
                      origin.y + visibleSize.height * 3 / 5);
    jump = CCJumpToEx::create(1, dst, visibleSize.height / 2, 1, true, 90);
    rJump = CCJumpToEx::create(1, s->getPosition(), visibleSize.height / 2, 1, true, 90);
    s->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(jump, rJump)));
}

std::string ActionJumpEx::subtitle()
{
    return "JumpBy/JumpTo Ex";
}

//------------------------------------------------------------------
//
// Missile
//
//------------------------------------------------------------------
void ActionMissile::onEnter()
{
    ActionDemo::onEnter();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // target
    CCSprite* grossini = CCSprite::create("Images/grossini.png");
    grossini->setPosition(ccp(origin.x + visibleSize.width / 8,
                              origin.y + visibleSize.height / 8));
    grossini->setTag(1);
    addChild(grossini);
    
    // move target
    CCMoveTo* m1 = CCMoveTo::create(2, ccp(origin.x + visibleSize.width * 7 / 8,
                                           origin.y + visibleSize.height / 8));
    CCMoveTo* m2 = CCMoveTo::create(2, ccp(origin.x + visibleSize.width * 7 / 8,
                                           origin.y + visibleSize.height * 7 / 8));
    CCMoveTo* m3 = CCMoveTo::create(2, ccp(origin.x + visibleSize.width / 8,
                                           origin.y + visibleSize.height * 7 / 8));
    CCMoveTo* m4 = CCMoveTo::create(2, ccp(origin.x + visibleSize.width / 8,
                                           origin.y + visibleSize.height / 8));
    CCSequence* seq = CCSequence::create(m1, m2, m3, m4, NULL);
    grossini->runAction(CCRepeatForever::create(seq));
    
    // missile
    CCSprite* m = CCSprite::create("Images/r1.png");
    m->setPosition(ccp(origin.x + visibleSize.width / 2,
                       origin.y + visibleSize.height / 2));
    addChild(m);
    
    // run missile action
    CCMissile* action = CCMissile::create(130, grossini, 0,
                                          CCCallFunc::create(this, callfunc_selector(ActionMissile::onHit)));
    m->runAction(action);
}

void ActionMissile::onHit() {
    getChildByTag(1)->stopAllActions();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCLabelTTF* label = CCLabelTTF::create("Hit!!", "Helvetica", 24);
    label->setPosition(ccp(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2));
    addChild(label);
}

std::string ActionMissile::subtitle()
{
    return "Missile";
}

//------------------------------------------------------------------
//
// Shake
//
//------------------------------------------------------------------
void ActionShake::onEnter()
{
    ActionDemo::onEnter();
	
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	CCSprite* s = CCSprite::create("Images/grossini.png");
	s->setPosition(ccp(origin.x + visibleSize.width / 2,
					   origin.y + visibleSize.height / 2));
	addChild(s);
	
	s->runAction(CCRepeatForever::create(CCShake::create(1, 5)));
}

std::string ActionShake::subtitle()
{
    return "Shake";
}

//------------------------------------------------------------------
//
// Tree Fade In/Out
//
//------------------------------------------------------------------
void ActionTreeFadeInOut::onEnter()
{
    ActionDemo::onEnter();
	
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	CCSprite* s = CCSprite::create("Images/grossini.png");
	s->setPosition(ccp(origin.x + visibleSize.width / 2,
					   origin.y + visibleSize.height / 2));
	addChild(s);
	
	CCSprite* s2 = CCSprite::create("Images/grossini.png");
	s->addChild(s2);
	
	CCSprite* s3 = CCSprite::create("Images/grossini.png");
	s2->addChild(s3);
	
	s->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(CCTreeFadeOut::create(2), CCTreeFadeIn::create(2))));
}

std::string ActionTreeFadeInOut::subtitle()
{
    return "Tree Fade In/Out";
}
