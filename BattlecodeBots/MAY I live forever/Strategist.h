#ifndef STRATEGIST_H
#define STRATEGIST_H

enum Strategy {
	/*
	Reach Mars as soon as possible and exploit its resources to shuttle more offensive units
	Essentially, we want to be killing any uncoordinated rocket landing immediately
	Snipe Earth enemy spawn sections or unoccupied/enemy spotted Mars sections
	Research rockets, rangers, and workers
	*/
	Psychonaut,
	/*
	This strategy is a counter to knight rush
	Build a few mages and lots of rangers
	*/
	WizardWin,
	/*
	Turtle with rangers until overcharge is researched
	Multiple overcharge mages from the backlines to move into range of enemies and deal devastating damage
	Research healers, mages, and rangers
	*/
	TerroristOvercharge
};

class Strategist
{
public:
	Strategist();
	~Strategist();
	static Strategy strategy;
};

#endif
