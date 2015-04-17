#include "Bubblewrap/Bubblewrap.hpp"
#include "Bubblewrap/Registers/BubblewrapRegister.hpp"
#include "Bubblewrap/Registers/SfmlRegisters.hpp"
#include "Bubblewrap/Math/Matrix3.hpp"

int main()
{
	Bubblewrap::Game::GameSettings settings;
	settings.WindowCount_ = 1;
	settings.WindowSettings_ = new Bubblewrap::Render::Window::WindowSettings[ 1 ];
	settings.WindowSettings_[ 0 ].Width_ = 800;
	settings.WindowSettings_[ 0 ].Height_ = 600;
	settings.WindowSettings_[ 0 ].Title_ = "POTATO";
	settings.WindowSettings_[ 0 ].Name_ = "Main";
	Bubblewrap::Logs::StaticLog::Instance()->SetLogLevel( Bubblewrap::Logs::StaticLog::CRITICAL );
	settings.Registers_.push_back( Bubblewrap::Registers::SfmlRegisters::RegisterUtilities );
	settings.Registers_.push_back( Bubblewrap::Registers::SfmlRegisters::RegisterGraphics );
	settings.Registers_.push_back( Bubblewrap::Registers::SfmlRegisters::RegisterAudio );
	settings.Resources_.push_back( "textures" );
	settings.TypeRegistration_ = ( [ ]( Bubblewrap::Base::ObjectRegister* Register )
	{
	} );
	settings.Packages_.push_back( "basics.json" );
	settings.BaseObject_ = "basics:LevelEntity";
	Bubblewrap::Game::Game Game;
	Game.Run( settings );
}