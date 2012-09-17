#include "Entity.hxx"

EntityID Entity::get_id() const {
	return _id;
}

void Entity::load_component(ComponentType component_type,
							Component * component) {
	_components[component_type] = component;
}

void Entity::react(Event * event) {
	printf("Entity at %p received event!\n", this);
	/* TODO: Do things. */
}