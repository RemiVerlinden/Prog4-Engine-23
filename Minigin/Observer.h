#pragma once
class Observer
{
public:
	virtual ~Observer();
	virtual void OnNotify() = 0;
};

class Achievements final : public Observer
{
public:
	~Achievements();
	virtual void OnNotify();
private:

};
