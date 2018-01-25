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
	This strategy is a counter to the classic 'ranger horde'
	Slowly amass an army of knights until a charge order is issued
	This strategy assumes you will take control of the Earth before sending significant forces to Mars
	Research knights, healers, rockets, then mages
	*/
	ShieldFormation,
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
	Strategy strategy;
};

#endif