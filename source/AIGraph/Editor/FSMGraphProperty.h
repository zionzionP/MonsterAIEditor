#pragma once

class FSMGraphProperty
{
public:
	FSMGraphProperty() {};
	virtual ~FSMGraphProperty() = default;

	virtual void Update() {}
	virtual void Draw(){}
};
