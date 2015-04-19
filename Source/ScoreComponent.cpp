#include "ScoreComponent.hpp"

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
	ScoreText_ = GetParentEntity()->GetComponentsByType< Text >()[ 0 ];
	char buffer[ 256 ];
	memset( buffer, 0, 256 );
	sprintf( buffer, "Score: %d", (int)Score_ );
	ScoreText_->SetText( buffer );
}
