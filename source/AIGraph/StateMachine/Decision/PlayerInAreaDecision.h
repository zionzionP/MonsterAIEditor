#pragma once
#include "AIGraph/StateMachine/Decision/Decision.h"
class PlayerInAreaDecision : public Decision
{
public:
	PlayerInAreaDecision() {}
	PlayerInAreaDecision(const Vec2& i_posTL, const Vec2& i_posBR) :
		areaTL(i_posTL),
		areaBR(i_posBR) {}
	PlayerInAreaDecision(const Vec2& i_posTL, const Vec2& i_posBR, bool i_forOut) :
	    forPlayerOut(i_forOut),
		areaTL(i_posTL),
		areaBR(i_posBR) {}
	~PlayerInAreaDecision() = default;

	bool Decide(StateController* io_pController) override
	{
		if(forPlayerOut)
		{
			return !InArea(io_pController);
		}
		else
		{
			return InArea(io_pController);
		}		
	}
	//プレイヤーがパトロール範囲内を判断
	bool InArea(StateController* io_pController)
	{
		Vec2 pos = io_pController->GetOwnerEnemy().GetTarget()->GetPosition();
		return (pos.x >= areaTL.x && pos.x <= areaBR.x && pos.y >= areaTL.y && pos.y <= areaBR.y);
	}
private:
	Vec2 areaTL = { 0,0 };
	Vec2 areaBR = { 0,0 };
	//trueの場合、逆の結果を返す
	bool forPlayerOut = false;
};
