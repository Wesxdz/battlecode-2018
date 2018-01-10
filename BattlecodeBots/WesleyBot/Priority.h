#pragma once

/*
Optimization of individual actions
Unit construction
Research pathways
*/
template<class T>
class Task
{
public:
	T action;
	float value;
	int roundEstimate; // The amount of rounds estimated to complete this task
public:
	float GetRoundValue();
};

template<class T>
inline float Task<T>::GetRoundValue()
{
	return value / roundEstimate;
}
