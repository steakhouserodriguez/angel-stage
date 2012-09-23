#include "Renderer.hxx"
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

void Renderer::render() {
	al_clear_to_color(al_map_rgb(0,0,0));
	_map_renderer.render(_viewpoint.getX(), _viewpoint.getY());
	al_flip_display();
	std::vector<VisualComponent *>::iterator iter;
	for (iter = _unit_layer.begin(); iter < _unit_layer.end(); iter++) {
		(*iter)->render(_viewpoint);
	}
	al_flip_display();
}

void Renderer::setViewpoint(coordinate_t x, coordinate_t y) {
	_viewpoint.setX(x);
	_viewpoint.setY(y);
}

const Position & Renderer::getViewpoint() {
	return _viewpoint;
}

void Renderer::addToUnitLayer(VisualComponent * visual_comp) {
	_unit_layer.push_back(visual_comp);
}