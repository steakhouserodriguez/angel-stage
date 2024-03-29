#include <iostream>
#include <cstdio>
#include "ClientGameState.hxx"
#include "Event.hxx"
#include "Debug.hxx"
#include <math.h>
#include "Client.hxx"
#include "ClientUnit.hxx"

ClientGameState::~ClientGameState() {
    std::map<EntityID, ClientEntity *>::iterator iter = _entities.begin();
    while (iter != _entities.end()) {
        delete iter->second;
        ++iter;
    }
}

ClientEntity *ClientGameState::get_entity(EntityID id) {
    if ( _entities.count(id) == 0) { //if entity doens't exist
        DEBUG("Creating clientunit: id="<< id);
        //intialize new gamestate
        ClientUnit *ce = new ClientUnit(id, this);

        //components
        // UnitVisualComponent *uvc = new UnitVisualComponent(ce);
        // ClientUnitStateComponent *usc = new ClientUnitStateComponent(ce);
        // ce->set_visual_component(uvc);
        // ce->set_unit_state_component(usc);
        // _client->renderer->add_to_unit_layer(uvc);
        _client->renderer->add_to_unit_layer((Renderable *) ce);

        set_entity(id, ce);
    }
    return _entities[id];
}

void ClientGameState::delete_entity(EntityID id) {

    //TODO make this better

    // std::map<EntityID, ServerEntity*>::iterator it;
    // for(it = _entities.begin(); it != _entities.end(); it++) {
    // 	if (it->first == id){
    // 		_entities.

    // 	}

    // }


    // VisualComponent* vis = get_entity(id)->get_visual_component();
    _client
    ->renderer
    ->remove_from_unit_layer(
        (ClientUnit *) get_entity(id));
    remove_clock_listener(get_entity(id));
    delete get_entity(id);
    DEBUG("NUM ENTITIES=" << _entities.size());
    _entities.erase(id);
    DEBUG("NUM ENTITIES=" << _entities.size());
}

bool ClientGameState::entity_exists(EntityID id) {
    return _entities.count(id) == 1;
}

void ClientGameState::set_entity(EntityID id, ClientEntity *entity) {
    _entities[id] = entity;
}

void ClientGameState::react(Event *event) {
    if (is_entity_event(event)) {
        // DEBUG("entity event received on gamestate" << _entities.size());
        EntityID id = ((EntityEvent *) event)->entity_id;
        ClientEntity *e = get_entity(id);
        e->react((EntityEvent *) event);
    } else if (is_global_event(event)) {
        DEBUG("Received global event!");
        switch (event->event_type) {
            case EVENT_ENTITY_SPAWN: {
                UnitFeedbackEvent *ufe = (UnitFeedbackEvent *)(event);
                // std::cout << "Spawn an entity with id " << ufe->header.entity_id << "\n";
                // ClientEntity *ce = new ClientEntity(ufe->header.entity_id);
                // ce->set_gamestate(this);
                // UnitVisualComponent *uvc = new UnitVisualComponent(ce);
                // ClientUnitStateComponent *usc = new ClientUnitStateComponent(ce);
                // ce->set_visual_component(uvc);
                // ce->set_unit_state_component(usc);
                // globalRenderer->add_to_unit_layer(ce->get_visual_component());
                // // ClientEntity* ce = get_entity(ufe->header.entity_id);
                // // ce->react((EntityEvent*)ufe);
                // set_entity(ufe->header.entity_id, ce);
                ClientEntity *ce = get_entity(ufe->header.entity_id);
                ce->react((EntityEvent *)ufe);
                break;
            }
            case EVENT_ENTITY_DIE: {
                UnitDieEvent *ude = (UnitDieEvent * ) event;
                delete_entity(ude->entity_id);



                break;
            }
            case EVENT_TEST: {
                DEBUG("Received a test event!");
                break;
            }
        }
    }
}

void ClientGameState::tick(double wallTime, double deltaTime) {
    std::vector<ClientEntity *>::iterator it;
    for(it = clockReceivers.begin(); it<clockReceivers.end(); it++) {
        (*it)->tick(wallTime, deltaTime);
    }
}

void ClientGameState::add_clock_listener(ClientEntity *toListen) {
    clockReceivers.push_back(toListen);
    DEBUG("Added clock listener");
}

void ClientGameState::remove_clock_listener(ClientEntity *toListen) {
    //TODO test
//	clockReceivers.erase(
//		s td::remove(clockReceivers.begin(), clockReceivers.end(), toListen), clockReceivers.end() );
    std::vector<ClientEntity *>::iterator it;

    for(it = clockReceivers.begin(); it<clockReceivers.end(); it++) {
        if( (*it) == toListen) {
            clockReceivers.erase(it);
        }
    }
}

std::vector<EntityID> * ClientGameState::get_entities_in_rect(const Position &p1,
        const Position &p2) {
    std::vector<EntityID> * out = new std::vector<EntityID>;
    double x1 = p1.get_x();
    double x2 = p2.get_x();
    double left  = x1 < x2 ? x1 : x2;
    double right = x1 < x2 ? x2 : x1;
    double y1 = p1.get_y();
    double y2 = p2.get_y();
    double bottom  = y1 < y2 ? y1 : y2;
    double top = y1 < y2 ? y2 : y1;
    Position top_left(left, top);
    Position bottom_right(right, bottom);
    std::map<EntityID, ClientEntity *>::iterator iter = _entities.begin();
    while (iter != _entities.end()) {
        if (
            ((ClientUnit *)get_entity(iter->first))
            ->get_position()
            .in_rect(top_left, bottom_right)) {
            out->push_back(iter->first);
        }
        ++iter;
    }
    return out;
}
