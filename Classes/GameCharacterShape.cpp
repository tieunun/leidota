#include "GameCharacterShape.h"

GameCharacterShape::GameCharacterShape(const std::string& fileName, const std::string& armatureName)
{
    ArmatureDataManager::getInstance()->addArmatureFileInfo(fileName);
    _armature = Armature::create(armatureName);
    _currentAnimationName = "";
    
    // @_@ 这里是因为美术每个资源的问题，给的任务的朝向不同，只好加上这个逻辑
    if (armatureName == "xuejingling-qian" || armatureName == "Aer")
    {
        _armature->setScaleX(-1);
    }
}

GameCharacterShape* GameCharacterShape::create(const std::string& fileName, const std::string& armatureName)
{
    auto pRet   =   new GameCharacterShape(fileName, armatureName);
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

    this->addChild(_armature);

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