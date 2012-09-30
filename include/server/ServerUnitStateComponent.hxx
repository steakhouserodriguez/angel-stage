#ifndef _SERVER_UNIT_STATE_COMPONENT_HXX_
#define _SERVER_UNIT_STATE_COMPONENT_HXX_

#include "Position.hxx"
#include "Ids.hxx"
#include "ServerComponent.hxx"

#define STATE_NOT_MOVING		5
#define STATE_CHASING_POINT		42
#define STATE_CHASING_UNIT		43

const double UNIT_VELOCITY = 100.0;

class ServerUnitStateComponent : public ServerComponent {

public:
	ServerUnitStateComponent(ServerEntity* entity);
	virtual ~ServerUnitStateComponent() {};
	virtual void tick(double wallTime, double deltaT);
	virtual const Position & getPosition();
	virtual void setPosition(Position newpos);

	virtual void setGoalPoint(Position goal);
	virtual void setGoalEntity(EntityID target);

protected:
	// MOTION AND POSITION
	Position _pos;
	Position _orbit_pos;
	
	EntityID _target;

	double _xVel;
	double _yVel;

	Position _goal;
	double _theta;
	double _size;
	
	virtual void setGoal(Position goal);
	
	int _state;
};

#endif
