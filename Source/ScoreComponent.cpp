#include "ScoreComponent.hpp"
#include "Bubblewrap/Events.hpp"
using namespace Math;

ScoreComponent::ScoreComponent()
{
	Score_ = 0.0f;
}

void ScoreComponent::Initialise( Json::Value Params )
{
	Component::Initialise( Params );
}


void ScoreComponent::Copy( ScoreComponent* Target, ScoreComponent* Base )
{
	Component::Copy( Target, Base );
}

void ScoreComponent::OnAttach()
{
	InputHandle_ = GetManager().GetEventManager().RegisterEvent( Events::EventTypes::Input, std::bind( &ScoreComponent::InputFunction, this, std::placeholders::_1 ) );
	ScoreText_ = GetParentEntity()->GetComponentsByType< Text >( "Score" )[ 0 ];
	char buffer[ 256 ];
	memset( buffer, 0, 256 );
	sprintf( buffer, "Score: %d", (int)Score_ );
	ScoreText_->SetText( buffer );
}

void ScoreComponent::OnDetach()
{
	InputHandle_.Destroy();
}

void ScoreComponent::InputFunction( Bubblewrap::Events::Event* Event )
{
	Bubblewrap::Events::InputData data = *( Bubblewrap::Events::InputData* )Event->GetData();

	if ( ( data.Key_ == Events::Key::Space ) && ( data.InputType_ == Events::InputData::InputType::KeyUp ) )
	{
		GetParentEntity()->Destroy();
		GetRegister().LoadObject( "basics:MainMenu", nullptr );
	}
}