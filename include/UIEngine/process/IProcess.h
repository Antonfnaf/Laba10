#pragma once
class IProcess
{
private:
	bool finished = false;
public:
	virtual ~IProcess() = default;
	virtual void Update() = 0;
	bool IsFinished() const { return finished; }
	void Finish() { finished = true; }
};
