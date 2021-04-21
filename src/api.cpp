
#include "engine/engine.cpp"

#ifdef _WIN32
#	define FUNC extern "C" __declspec(dllexport)
#else
#	define FUNC extern "C"
#endif

using byte = unsigned char;

#warning The API is still missing many features, dont treat it as final or stable!

/// Initialize LibTile2d
FUNC bool engine_init() {
	return lib_init();
}

/// Initialize LibTile2d
FUNC void engine_exit() {
	lib_exit();
}

/// Create and open new Window object
FUNC void* window_new( const char* name, int width, int height ) {
	return (void*) new Window( name, width, height );

}

/// Check if the window was created
FUNC bool window_verify( void* window ) {
	return ((Window*) window)->isOk();
}

/// Delete and close a window object
FUNC void window_free( void* window ) {
	delete (Window*) window;
}

/// Add layer (or layer group) to window
FUNC void window_add_layer( void* window, void* layer ) {

}

/// Delete and close a window object
FUNC void window_set_resource_handle( void* resource_handle ) {

}

/// Create and open new Window object
FUNC void* resource_handle_new() {

}

/// Delete and close a window object
FUNC void resource_handle_free( void* resource_handle ) {

}

/// Create new Resource object
FUNC void* resource_create( void* resource_handle, int tile ) {

}

/// Add light component to resource
FUNC void resource_add_light( byte r, byte g, byte b ) {

}

/// Add texture component to resource
FUNC void resource_add_texture( int id, void* data ) {

}

/// Add texture animation frame
FUNC void resource_add_frame( int id ) {

}


/// Create new Layer object
FUNC void* layer_new( void* parent ) {

}

/// Delete Layer object
FUNC void layer_free( void* layer ) {

}

/// Set layer's z-index
FUNC void layer_set_index( void* layer, long index ) {

}

/// Set layer's x, y, and rotation
FUNC void layer_set_position( void* layer, float x, float y, float r ) {

}

/// Set tile map
FUNC void layer_set_map( void* layer, void* map ) {

}

/// Get layer's z-index
FUNC long layer_get_index( void* layer ) {

}

/// Get layer's position x
FUNC float layer_get_posx( void* layer ) {

}

/// Get layer's position y
FUNC float layer_get_posy( void* layer ) {

}

/// Get layer's rotation
FUNC float layer_get_rotation( void* layer ) {

}

/// Create new tile map given a map trait
FUNC void* map_new( byte trait ) {

}

/// Set tile at position, this expands the map if required
FUNC void map_set_tile( void* map, int x, int y, int tile ) {

}

/// Delete tile map
FUNC void map_free( void* map ) {

}
