#include "GameCharacterShape.h"

const string GameCharacterShape::SHAPEDIR   =   "shape/";

GameCharacterShape::GameCharacterShape(const std::string& armatureName):
    FLOATNUMBERDIRATION(0.3f), FLOATNUMBERMOVEBYY(50)
{
    string tmpFileName  =   SHAPEDIR + armatureName;
    ArmatureDataManager::getInstance()->addArmatureFileInfo(tmpFileName + ".ExportJson");
    _armature = Armature::create(armatureName);
    _currentAnimationName = "";
    m_halo  =   nullptr;
    m_hpBar =   nullptr;
    
    // @_@ 这里是因为美术每个资源的问题，给的任务的朝向不同，只好加上这个逻辑
    if (armatureName == "xuejingling-qian" || armatureName == "Aer" || armatureName == "Theif" 
        || armatureName == "YSG")
    {
        _armature->setScaleX(-1);
    }
}

GameCharacterShape* GameCharacterShape::create(const std::string& armatureName)
{
    auto pRet   =   new GameCharacterShape(armatureName);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }

    return nullptr;
}

void GameCharacterShape::playAction(const std::string& actionName, bool loop, ActionFrameEventCallback eventCallBack)
{
    // @_@ 需要检查一下是否有指定动画
    std::vector<std::string> tmpNames = _armature->getAnimation()->getAnimationData()->movementNames;
    for (int i = 0; i < tmpNames.size(); i++)
    {
        if (tmpNames[i] == actionName)
        {
            if (isNotInAnimation() || _currentAnimationName != actionName)
            {
                _armature->getAnimation()->play(actionName, -1, loop ? 9999999 : 0); 
                _currentAnimationName = actionName;
                _frameEventCallBack = eventCallBack;

                // 设置帧事件的回调函数
                _armature->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(GameCharacterShape::onFrameEvent));
            }
            
            return;
        }
    }

    // 如果没有动画可以播放，就播放一个一定存在的动画idle
    playAction(IDLE_ACTION);
}

bool GameCharacterShape::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    // 添加动画
    this->addChild(_armature, 2);

    // 添加血条
    auto tmpHpBarBg =   Sprite::create("character/barbg.png");
    tmpHpBarBg->setScale(0.5);
    m_hpBar =   LoadingBar::create("character/hpbar.png", 100);
    m_hpBar->setAnchorPoint(Vec2(0, 0));
    m_hpBar->setPosition(Vec2(3.5, 2));
    tmpHpBarBg->addChild(m_hpBar);
    tmpHpBarBg->setAnchorPoint(Vec2(0.5, 0));
    tmpHpBarBg->setPosition(Vec2(0, _armature->getContentSize().height));
    this->addChild(tmpHpBarBg);

/**
    // @_@ 增加标签
    m_posNumLabel = Label::createWithBMFont("font/greennumber.fnt", "-1");
    m_posNumLabel->setPositionY(this->getCenterPos().y);
    this->addChild(m_posNumLabel, 4);

    // @_@ 当前目标的标签
    m_goalLabel =   Label::createWithSystemFont("Hello World","Arial", 25);
    m_goalLabel->setColor(Color3B(255, 0, 0));
    m_goalLabel->setPositionY(this->getCenterPos().y + 50);
    this->addChild(m_goalLabel, 5);

    // @_@ 驱动力的标签
    m_forceLabel =  Label::create("驱动力","Arial", 25);
    m_forceLabel->setColor(Color3B(0, 255, 0));
    m_forceLabel->setPositionY(this->getCenterPos().y - 50);
    this->addChild(m_forceLabel, 5);
*/
    return true;
}

void GameCharacterShape::faceToRight()
{
    this->setScaleX(1);
}

void GameCharacterShape::faceToLeft()
{
    this->setScaleX(-1);
}

bool GameCharacterShape::isNotInAnimation()
{
    return !_armature->getAnimation()->isPlaying();
}

int GameCharacterShape::getCurrentFrameIndex()
{
    return _armature->getAnimation()->getCurrentFrameIndex();
}

Rect GameCharacterShape::getCollisionRect()
{
    Rect tmpRect    =   _armature->getBoundingBox();
    Size tmpSize    =   tmpRect.size;
    tmpSize.width   -=  80;
    
    // 返回该节点在父节点上的坐标
    auto tmpPoint   =   this->getPosition();
    tmpRect.setRect(tmpPoint.x - tmpSize.width / 2, tmpPoint.y, tmpSize.width, tmpSize.height);
    return tmpRect;
}

void GameCharacterShape::onFrameEvent(Bone *bone, const string& evt, int originFrameIndex, int currentFrameIndex)
{
    if (_frameEventCallBack != nullptr)
    {
        _frameEventCallBack(evt);
    }
}

Vec2 GameCharacterShape::getCenterPos()
{
    auto tmpRect    =   this->getCollisionRect();
    auto tmpPos     =   Vec2(tmpRect.getMidX(), tmpRect.getMidY());
    return tmpPos;
}

void GameCharacterShape::floatNumber(int num, GameCharacterShape::FloatNumberTypeEnum type )
{
    char numStr[10];
    sprintf(numStr, "-%d", num);
    // 选择使用的文字样式
    auto tmpFntSrc  =   "";
    switch (type)
    {
    case GameCharacterShape::FLOAT_NUMBER_GREEN:
        tmpFntSrc   =   "font/greennumber.fnt";
        break;
    case GameCharacterShape::FLOAT_NUMBER_RED:
        tmpFntSrc   =   "font/rednumber.fnt";
        break;
    case GameCharacterShape::FLOAT_NUMBER_YELLOW:
        tmpFntSrc   =   "font/yellownumber.fnt";
        break;
    default:
        break;
    }

    // 创建标签
    auto tmpText    =   Label::createWithBMFont(tmpFntSrc, numStr);
    tmpText->setPositionY(_armature->getContentSize().height / 2);
    this->addChild(tmpText, 4);
    tmpText->setScaleX(this->getScaleX());

    // 绑定浮动的动画，播放动画
    auto tmpMoveBy  =   MoveBy::create(FLOATNUMBERDIRATION, Vec2(0, FLOATNUMBERMOVEBYY));
    tmpText->runAction(Sequence::create(tmpMoveBy, CallFuncN::create(std::bind(&GameCharacterShape::onFloatNumberMoveOver, this, std::placeholders::_1)), nullptr));
}

void GameCharacterShape::onFloatNumberMoveOver( Node* pNode )
{
    pNode->removeFromParentAndCleanup(true);
}

void GameCharacterShape::showHalo( HaloTypeEnum type )
{
    if (m_halo != nullptr)
    {
        m_halo->removeFromParentAndCleanup(true);
        m_halo  =   nullptr;
    }

    // 添加光圈动画
    ArmatureDataManager::getInstance()->addArmatureFileInfo("halo/greenhalo0.png", "halo/greenhalo0.plist", "halo/greenhalo.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("halo/redhalo0.png", "halo/redhalo0.plist", "halo/redhalo.ExportJson");

    switch (type)
    {
    case GameCharacterShape::HALO_GREEN:
        {
            m_halo  =   Armature::create("greenhalo");
            this->addChild(m_halo);
            break;
        }
        
    case GameCharacterShape::HALO_RED:
        {
	        m_halo  =   Armature::create("redhalo");
	        this->addChild(m_halo);
	        break;
        }
    default:
        break;
    }

    m_halo->getAnimation()->play("Animation1");
}

void GameCharacterShape::hideHalo()
{
    if (m_halo != nullptr)
    {
        m_halo->removeFromParentAndCleanup(true);
        m_halo  =   nullptr;
    }
}

void GameCharacterShape::setHpRatio( float ratio )
{
    m_hpBar->setPercent(ratio * 100);
}

std::string GameCharacterShape::getCurrentAnimationName()
{
    // 如果当前没有在播放动画，就返回空的字符串
    if (isNotInAnimation())
    {
        return "";
    }
    return _currentAnimationName;
}
