#include <iostream> 
#include <sstream> 

#include <iterator> //STL Stuff

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1App.h"
#include "j2EntityManager.h"
#include "j1Minimap.h"
// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	want_to_save = want_to_load = false;

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	scene = new j1Scene();
	map = new j1Map();
	entity_manager = new j2EntityManager();
	minimap = new j1Minimap();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(map);
	AddModule(scene);
	AddModule(entity_manager);
	AddModule(minimap);
	// render last to swap buffer
	AddModule(render);
}

// Destructor
j1App::~j1App()
{
	//release Modules from the STL list
	std::list<j1Module*>::iterator stlItem = stlModules.begin();

	while (stlItem != stlModules.end())
	{
		RELEASE(*stlItem);
		stlItem = next(stlItem);
	}

	stlModules.clear();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();

	//STL
	stlModules.push_back(module);
}

// Called before render is available
bool j1App::Awake()
{
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;
		
	config = LoadConfig(config_file);

	if(config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title = app_config.child("title").child_value();
		organization = app_config.child("organization").child_value();
	}

	if(ret == true)
	{
		std::list<j1Module*>::iterator stl_item = stlModules.begin();

		while (stl_item != stlModules.end() && ret == true)
		{
			ret = (*stl_item)->Awake(config.child((*stl_item)->name.data()));
			stl_item++;
		}
	}

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	bool ret = true;

	std::list<j1Module*>::iterator stl_item = stlModules.begin();

	while (stl_item != stlModules.end() && ret == true)
	{
		(*stl_item)->Start();
		stl_item++;
	}

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node j1App::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if(result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	// We calculate the dt: differential time since last frame
	dt = frame_time.ReadSec();

	frame_time.Start();
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	if(want_to_save == true)
		SavegameNow();

	if(want_to_load == true)
		LoadGameNow();

	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	// We can cap and uncap FPS by pressing F11
	if (App->input->GetKey(SDL_SCANCODE_F11) == j1KeyState::KEY_DOWN) {
		cappedFPS = !cappedFPS;
		frame_count = 0;
	}

	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	static char title[256];
	char* cap;

	if (cappedFPS)
		cap = "on";
	else
		cap = "off";

	sprintf_s(title, 256, "Minimap Research ~ FPS: %d / Last Frame Ms: %02u / Cap %s",
		frames_on_last_update, last_frame_ms, cap);
	App->win->SetTitle(title);
	if ((last_frame_ms < (1000 / framerate_cap)) && cappedFPS) {
		SDL_Delay((1000 / framerate_cap) - last_frame_ms);
	}

}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	bool ret = true;

	std::list<j1Module*>::iterator stl_item = stlModules.begin();

	for (; stl_item != stlModules.end(); stl_item++)
	{
		if ((*stl_item)->active == false)
		{
			continue;
		}

		ret = (*stl_item)->PreUpdate();
	}
	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;
	std::list<j1Module*>::iterator stl_item = stlModules.begin();

	for (; stl_item != stlModules.end(); stl_item++)
	{
		if ((*stl_item)->active == false)
		{
			continue;
		}

		ret = (*stl_item)->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	bool ret = true;

	std::list<j1Module*>::iterator stl_item = stlModules.begin();

	for (; stl_item != stlModules.end() && ret == true; stl_item++)
	{
		if ((*stl_item)->active == false)
		{
			continue;
		}

		ret = (*stl_item)->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	bool ret = true;

	std::list<j1Module*>::iterator stl_item = --stlModules.end();

	while (stl_item != stlModules.end() && ret == true)
	{
		ret = (*stl_item)->CleanUp();
		stl_item--;
	}

	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.data();
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.data();
}

// Load / Save
void j1App::LoadGame(const char* file)
{
	want_to_load = true;
}

// ---------------------------------------
void j1App::SaveGame(const char* file) const
{
	want_to_save = true;
}


bool j1App::LoadGameNow()
{
	bool ret = false;

	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file(load_game.data());

	want_to_load = false;
	return ret;
}

bool j1App::SavegameNow() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", save_game.data());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;
	
	root = data.append_child("game_state");

	data.reset();
	want_to_save = false;
	return ret;
}