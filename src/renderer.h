#ifndef OSRENDERER_H
#define OSRENDERER_H

#include "light.h"
#include "display.h"
#include "drawing.h"
#include "gui.h"
#include "keyboard.h"
#include "vector.h"
#include "shapes.h"
#include "mesh.h"
#include "macros.h"
#include "cimgui.h"
#include "scene_controller.h"
#include "graphic_pipeline.h"

class OSRenderer
{
	public:
		OSRenderer();
		~OSRenderer();
		bool game_loop;
		int position_mouse_x;
		int position_mouse_y;
		vect2<float> center_screen;

		SDL_Event keyboard_event;		
		Drawing draw;		
		GraphicPipeline pipeline;

		GlobalLight* light = GlobalLight::get_instance();
		OSGuiController* GUI = OSGuiController::get_instance();
		OSImgui* OSIMGUI = OSImgui::get_instance();
		OSceneController* SCENE = OSceneController::get_instance();
		// Cria a janela
		void create_window(const char* title, int width, int height, GUI_MODE mode);
		// Limpa o buffer de cor
		void clear_screen(); // this->display.clear_buffer();
		// desenha o buffer
		void draw_buffer();
		// Atualiza o render
		void update_render();	

		void frame_rate_control();
		float delta_time();
		// Ler os eventos do teclado
		inline void read_event() { SDL_PollEvent(&this->keyboard_event); };
		// retorna o tipo de evento que foi disparado
		inline Uint32 keyboard_event_type() { return this->keyboard_event.type; };
		inline Uint32 keyboard_event_key() { return this->keyboard_event.key.keysym.sym; };

		void set_frame_rate(int frame_rate);
		void set_background_color(uint32_t color);
		void run();

		int window_width();
		int window_height();
		Display* get_display();
		void create_camera(
			vect3<float> position,
			vect3<float> direction,
			float yaw,			
			float fov_y,
			float znear,
			float zfar
		);

		void set_camera_fov(float fov_x, float fov_y);
		// Metodos que ser�o sobrescritos pela classe filha
		virtual void engine_main() {}; // a cabe�a do jogo
		virtual void update() {}; // o pr� draw
		virtual void render() {}; // o draw		
		virtual void process_input() {}; // processamento do teclado e etc
		bool show_demo_window = true;
	private:
		Display display;
};

OSRenderer::OSRenderer(){

	game_loop = false;
	keyboard_event = SDL_Event();
	position_mouse_x = 0;
	position_mouse_y = 0;

	OSGui::guidraw = &this->draw;
	OSGui::default_pallet = Pallets::sky_blue_rutvi;
	Shape::shapedraw = &this->draw;		
	SDL_StartTextInput();
}

OSRenderer::~OSRenderer(){
	SDL_StopTextInput();
	SDL_DestroyRenderer(this->display.get_renderer());
	SDL_DestroyWindow(this->display.get_window());
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void OSRenderer::create_window(const char* title, int width, int height, GUI_MODE mode)
{
	this->game_loop = this->display.init_window(title, width, height, width, height, mode);
	this->display.setup_window();	
	
	// set the font
	OSGui::global_font = new OSFont("F:\\Projects\\cpp\\OSRenderer\\fonts\\aAbsoluteEmpire.ttf", 14);
	
	draw.set_display(&display);
	pipeline.set_display(&display);
	pipeline.set_drawing(&draw);

	draw.set_light(&light->light);
	center_screen = { width / 2.0f, height / 2.0f };
	Shape::center_screen = &center_screen;

	if (display.gui_mode == GUI_MODE::IMGUI) {
		// imgui
		OSIMGUI->set_window(display.get_window());
		OSIMGUI->set_renderer(display.get_renderer());
		OSIMGUI->set_texture(display.get_texture());
		OSIMGUI->set_event(&keyboard_event);
		OSIMGUI->set_scene(SCENE->get_scene());
		OSIMGUI->set_scene_controller(SCENE);
		OSIMGUI->set_display(&display);
		OSIMGUI->set_mesh(pipeline.get_mesh("nave"));
		OSIMGUI->setup(); // setup imgui
	}


}

void OSRenderer::clear_screen()
{
	this->display.clear_buffer();
	this->display.clear_z_buffer();
}

void OSRenderer::draw_buffer()
{
	this->display.draw_buffer();
}

void OSRenderer::update_render()
{	
	display.draw_buffer();
	if (display.gui_mode == GUI_MODE::IMGUI) OSIMGUI->draw();
	GUI->draw();
	SCENE->draw();
	display.sdl_render_present();
}

void OSRenderer::frame_rate_control()
{
	this->display.frame_rate_control();
}

float OSRenderer::delta_time()
{
	return display.delta_time;
}

void OSRenderer::set_frame_rate(int frame_rate)
{
	this->display.set_frame_rate(frame_rate);
}

void OSRenderer::set_background_color(uint32_t color)
{
	this->display.set_clear_color_screen(color);
}

void OSRenderer::run()
{
	engine_main();
	while (this->game_loop) {	
		process_input();
		clear_screen();
		update();		
		render();		
		// update screen
		update_render();
	}
}

int OSRenderer::window_width()
{
	return this->display.width;
}

int OSRenderer::window_height()
{
	return this->display.height;
}

Display* OSRenderer::get_display()
{
	return &this->display;
}

void OSRenderer::create_camera(
	vect3<float> position,
	vect3<float> direction,
	float yaw,
	float fov_y,
	float znear,
	float zfar
)
{

	float aspect_x = (float)display.width / (float)display.height;
	float aspect_y = (float)display.height / (float)display.width;
	float _fov_y = RAD(fov_y);
	float _fov_x = atan(tan(RAD(fov_y)/2) * aspect_x) * 2.0f;
	display.create_camera(
		position,
		direction,
		yaw,
		_fov_x,
		_fov_y,
		znear,
		zfar,
		aspect_x,
		aspect_y
	);
}

void OSRenderer::set_camera_fov(float fov_x, float fov_y)
{
	this->display.set_camera_fov(fov_x, fov_y);
}

#endif // !OSENGINE_H
