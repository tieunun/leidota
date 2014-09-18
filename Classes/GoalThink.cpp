#include "GoalThink.h"

GoalThink::GoalThink( GameCharacter* owner ):GoalComposite<GameCharacter>(owner)
{

}

GoalThink::~GoalThink()
{

}

GoalStateEnum GoalThink::process()
{
    return GoalComposite<GameCharacter>::process();
}

void GoalThink::activate()
{

}
