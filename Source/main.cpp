#include "Bubblewrap/Bubblewrap.hpp"
#include "PuzzleComponent.hpp"
#include "LevelComponent.hpp"
#include "EnemyComponent.hpp"
#include "ScoreComponent.hpp"
#include "MainMenuComponent.hpp"
#include "AnimatedComponent.hpp"
#include "Bubblewrap/Registers/BubblewrapRegister.hpp"
#include "Bubblewrap/Registers/SdlRegisters.hpp"
#include "Bubblewrap/Registers/BgfxRegisters.hpp"
#include "Bubblewrap/Math/Matrix3.hpp"
#include "BackgroundComponent.hpp"

int main()
{
	Bubblewrap::Game::GameSettings settings;
	settings.WindowCount_ = 1;
	settings.WindowSettings_ = new Bubblewrap::Render::Window::WindowSettings[ 1 ];
	settings.WindowSettings_[ 0 ].Width_ = 800;
	settings.WindowSettings_[ 0 ].Height_ = 600;
	settings.WindowSettings_[ 0 ].Title_ = "Ludum Dare #32 - Mind Like A Bot";
	settings.WindowSettings_[ 0 ].Name_ = "Main";
	settings.WindowSettings_[ 0 ].Colour_ = Bubblewrap::Render::Colour( 0.8f, 0.8f, 0.8f );
	settings.WindowSettings_[ 0 ].Colour_ = Bubblewrap::Render::Colour( 1.0f, 0.0f, 0.0f );
	Bubblewrap::Logs::StaticLog::Instance()->SetLogLevel( Bubblewrap::Logs::StaticLog::WARNING );
	settings.Registers_.push_back( Bubblewrap::Registers::SdlRegisters::RegisterUtilities );
	settings.Registers_.push_back( Bubblewrap::Registers::SdlRegisters::RegisterGraphics );
	settings.Registers_.push_back( Bubblewrap::Registers::BgfxRegisters::RegisterGraphics );
	settings.Resources_.push_back( "textures" );
	settings.Resources_.push_back( "shaders" );
	//settings.Paths_.push_back("MindLikeABot.exe");
	settings.Paths_.push_back("assets");
	settings.TypeRegistration_ = ( [ ]( Bubblewrap::Base::ObjectRegister* Register )
	{
		Register->RegisterCreator( "BackgroundComponent", BackgroundComponent::Create, BackgroundComponent::CopyDef );
		Register->RegisterCreator( "PuzzleComponent", PuzzleComponent::Create, PuzzleComponent::CopyDef );
		Register->RegisterCreator( "LevelComponent", LevelComponent::Create, LevelComponent::CopyDef );
		Register->RegisterCreator( "EnemyComponent", EnemyComponent::Create, EnemyComponent::CopyDef );
		Register->RegisterCreator( "ScoreComponent", ScoreComponent::Create, ScoreComponent::CopyDef );
		Register->RegisterCreator( "AnimatedComponent", AnimatedComponent::Create, AnimatedComponent::CopyDef );
		Register->RegisterCreator( "MainMenuComponent", MainMenuComponent::Create, MainMenuComponent::CopyDef );
	} );
	settings.Packages_.push_back( "basics.json" );
	settings.BaseObject_ = "basics:MainMenu";
	Bubblewrap::Game::Game Game;
	Game.Run( settings );
}