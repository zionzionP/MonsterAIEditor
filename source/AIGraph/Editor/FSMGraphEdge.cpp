#include "AIGraph/Editor/FSMGraphEdge.h"
#include "AIGraph/Editor/Decision/FSMGraphDecision.h"

double pointToSegmentDistance(const Vec2& p, const Vec2& start, const Vec2& end)
{
	Vec2 line = end - start;
	double lenSq = line.lengthSq();
	double t = Dot(line, p - start) / lenSq;

	if (t < 0.0) return p.distanceFrom(start);
	if (t > 1.0) return p.distanceFrom(end);

	Vec2 q = start + t * line;
	return p.distanceFrom(q);
}

FSMGraphEdge::FSMGraphEdge(FSMGraphNode* pFrom, FSMGraphNode* pTo) :
	pFromNode(pFrom), pToNode(pTo), property_(this)
{
	edgeLine_.set(pFromNode->GetPosition(), pToNode->GetPosition());
}

void FSMGraphEdge::Update()
{
	edgeLine_.set(pFromNode->GetPosition(), pToNode->GetPosition());
	//両方向の遷移が重ならないように、法線方向に少しずらす
	auto offset = edgeLine_.normal() * 5;
	edgeLine_.set(pFromNode->GetPosition() + offset, pToNode->GetPosition() + offset);
	if (isSelected_)
	{
		property_.Update();
	}
}

void FSMGraphEdge::Draw()
{
	auto intersectResult = pToNode->GetDisplayNode().intersectsAt(edgeLine_).value();
	if (intersectResult.size() > 0)
	{
		Vec2 arrowPos = intersectResult[0];

		Line arrow = { edgeLine_.begin, arrowPos };
		if (isSelected_)
		{
			arrow.drawArrow(lineThickness + 4, { 14,14 }, Palette::Yellow);
			property_.Draw();
			if (pAttachedDecision_)
			{
				pAttachedDecision_->Draw();
			}

		}
		arrow.drawArrow(lineThickness, { 10,10 }, Palette::White);
	}
}

void FSMGraphEdge::DebugDraw(Actor* pActor)
{
	if (pAttachedDecision_)
	{
		pAttachedDecision_->DebugDraw(pActor);
	}
}


bool FSMGraphEdge::IsClicked() const
{
	return pointToSegmentDistance(Cursor::PosF(), edgeLine_.begin, edgeLine_.end) < 2*lineThickness;
}

nlohmann::json FSMGraphEdge::Serialize() const
{
	return { "edge",
		{
			{"index", edgeIndex_},
			{"priority", transitionPriority_},
			{"fromNodeIndex", pFromNode->GetNodeIndex()},
			{"toNodeIndex", pToNode->GetNodeIndex()},
			{"attachedDecisionIndex", pAttachedDecision_->GetDecisionIndex()}
		}
	};	
}
