#ifndef _SERVER_UNIT_HXX_
#define _SERVER_UNIT_HXX_

//superclass
#include "ServerEntity.hxx"

//position
#include "Position.hxx"

const double UNIT_VELOCITY = 100.0;

const int UNIT_STATE_DEFAULT = 0;
const int UNIT_STATE_MERGING = 1;

/**
 * A Unit has a pose and size.
 */
class ServerUnit: public ServerEntity {

public:
    ServerUnit(
        EntityID id,
        ServerGameState *gamestate);
    virtual ~ServerUnit();
    virtual void react(EntityEvent *event);
    virtual void tick(double wallTime, double deltaT);

    void set_goal(Position goal);

    void sync();

    Position get_position();
    void set_position(Position pos);

    double get_orientation();

    double get_size();
    void set_size(double size);

private:
    //pose
    Position _pos;
    double _orientation;

    //attributes
    double _size;

    //motion:
    double _xVel;
    double _yVel;
    Position _goal;

    int _state;

    //merge
    EntityID _merge_partner;






    //ServerUnitStateComponent* get_unit_state_component(){ return _unit_state_component; };
    //void set_unit_state_component(ServerUnitStateComponent* comp) {_unit_state_component = comp; };

};

#endif /* _ENTITY_HXX_ */
