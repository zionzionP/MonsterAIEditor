#pragma once
class FSMGraphObject
{
public:
	FSMGraphObject() {};
	virtual ~FSMGraphObject() = default;

public:
	bool GetIsSelected() const { return isSelected_; }
	void SetIsSelected(bool selected) { isSelected_ = selected; }

protected:
	virtual void Update();
	virtual void Draw();

	bool isSelected_ = false;	

};
