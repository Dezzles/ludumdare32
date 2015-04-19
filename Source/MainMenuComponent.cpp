#include "MainMenuComponent.hpp"
#include "Bubblewrap/Events.hpp"
using namespace Math;

MainMenuComponent::MainMenuComponent()
{

}

void MainMenuComponent::Initialise( Json::Value Params )
{
	Component::Initialise( Params );
}


void MainMenuComponent::Copy( MainMenuComponent* Target, MainMenuComponent* Base )
{
	Component::Copy( Target, Base );
}

void MainMenuComponent::OnAttach()
{
	InputHandle_ = GetManager().GetEventManager().RegisterEvent( Events::EventTypes::Input, std::bind( &MainMenuComponent::InputFunction, this, std::placeholders::_1 ) );
}

void MainMenuComponent::OnDetach()
{
	InputHandle_.Destroy();
}

void MainMenuComponent::InputFunction( Bubblewrap::Events::Event* Event )
{
	Bubblewrap::Events::InputData data = *( Bubblewrap::Events::InputData* )Event->GetData();

	if ( ( data.Key_ == Events::Key::Space ) && ( data.InputType_ == Events::InputData::InputType::KeyUp ) )
	{
		GetParentEntity()->Destroy();
		GetRegister().LoadObject( "basics:LevelEntity", nullptr );
	}
}