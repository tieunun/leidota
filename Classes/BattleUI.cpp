#include "cocostudio/CCSGUIReader.h"
#include "BattleUI.h"
#include "UIViewIdEnum.h"
#include "EntityManager.h"

using namespace cocostudio;

bool BattleUI::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    // 加载界面部分
    auto tmpUIRoot = GUIReader::getInstance()->widgetFromJsonFile("battleui/battle_1.ExportJson");
    this->addChild(tmpUIRoot);

    // 获取对应的节点
    auto tmpRoot    =   tmpUIRoot->getChildByName("leaderhpbarbg");
    m_leaderHpBar   =   dynamic_cast<LoadingBar*>(tmpRoot->getChildByName("leaderhpbar"));
    tmpRoot         =   tmpUIRoot->getChildByName("leadericonbg");
    m_leaderIcon    =   dynamic_cast<ImageView*>(tmpRoot->getChildByName("leadericon"));

    m_enemyPanel    =   dynamic_cast<Node*>(tmpUIRoot->getChildByName("enemypanel"));
    tmpRoot         =   m_enemyPanel;
    tmpRoot         =   m_enemyPanel->getChildByName("enemyhpbarbg");
    m_enemyHpBar    =   dynamic_cast<LoadingBar*>(tmpRoot->getChildByName("enemyhpbar"));
    tmpRoot         =   m_enemyPanel->getChildByName("enemyiconbg");
    m_enemyIcon     =   dynamic_cast<ImageView*>(tmpRoot->getChildByName("enemyicon"));

    m_changeTargetBtn   =   dynamic_cast<Button*>(tmpUIRoot->getChildByName("changetargetbtn"));
    m_skillBtn          =   dynamic_cast<Button*>(tmpUIRoot->getChildByName("skillbtn"));
    m_convergeBtn       =   dynamic_cast<Button*>(tmpUIRoot->getChildByName("jihuobtn"));

    // 在节点上绑定回调
    m_changeTargetBtn->addTouchEventListener(CC_CALLBACK_2(BattleUI::onClickChangeTargetBtn, this));
    m_convergeBtn->addTouchEventListener(CC_CALLBACK_2(BattleUI::onClickConvergeBtn, this));
    m_skillBtn->addTouchEventListener(CC_CALLBACK_2(BattleUI::onClickSkillBtn, this));

    // 操纵柄
    m_jokStick    =   JoyStick::create(Sprite::create("battleui/000.png"), Sprite::create("battleui/001.png"));
    m_jokStick->setPosition(140, 140);
    tmpUIRoot->addChild(m_jokStick);
    // @_@ 这里开启每一帧回调
    this->scheduleUpdate();

    return true;
}

BattleUI::BattleUI():UIView(UIVIEW_ID_BATTLE)
{

}

void BattleUI::onWee( RefreshUIMsg& msg )
{
    switch (msg.eventType)
    {
    case REFRESH_UI_EVENT_CHARACTER:
        {
            refreshCharacter((GameCharacter*)msg.extraInfo);
            break;
        }

    case REFRESH_UI_EVENT_ATTACK_CHARACTER:
        {
            refreshTargetCharacter((GameCharacter*)msg.extraInfo);
            break;
        }

    default:
        break;
    }
}

void BattleUI::setWeeList()
{
    m_weeList.push_back(REFRESH_UI_EVENT_CHARACTER);
    m_weeList.push_back(REFRESH_UI_EVENT_ATTACK_CHARACTER);
}

void BattleUI::refreshCharacter( GameCharacter* character )
{
    if (character == EntityMgr->getmainEntity())
    {
        // 更新主角的UI
        auto tmpAttribute   =   character->getAttribute();
        m_leaderHpBar->setPercent(tmpAttribute.getHp() / tmpAttribute.getFullHp() * 100);
    }
}

void BattleUI::refreshTargetCharacter( GameCharacter* character )
{
    if (character == nullptr)
    {
        // 说明当前没有攻击目标
        m_enemyPanel->setVisible(false);
    }
    else
    {
        // 否则就显示该角色信息
        auto tmpAttribute   =   character->getAttribute();
        m_enemyPanel->setVisible(true);
        m_leaderHpBar->setPercent(tmpAttribute.getHp() / tmpAttribute.getFullHp() * 100);
        m_enemyIcon->loadTexture(character->getIconSrc());
    }
}

void BattleUI::update( float dm )
{
    // 判断操纵柄方向
    if (m_jokStick->getCtrlDirection().x > 10)
    {
        _delegate->moveToRight();
    }
    else if (m_jokStick->getCtrlDirection().x < -10)
    {
        _delegate->moveToLeft();
    }
    else
    {
        _delegate->idle();
    }
}

void BattleUI::onClickChangeTargetBtn( Ref* target,Widget::TouchEventType type )
{
    if (type == Widget::TouchEventType::ENDED)
    {
        _delegate->changeTarget();
    }
}

void BattleUI::onClickConvergeBtn( Ref* target,Widget::TouchEventType type )
{
    if (type == Widget::TouchEventType::ENDED)
    {

    }
}

void BattleUI::onClickSkillBtn( Ref* target,Widget::TouchEventType type )
{
    if (type == Widget::TouchEventType::ENDED)
    {

    }
}
