#include "BackgroundComponent.hpp"

using namespace Math;

BackgroundComponent::BackgroundComponent()
{
	BackgroundSize_ = 40;
}

void BackgroundComponent::Initialise( Json::Value Params )
{
	Component::Initialise( Params );
	
	REQUIRED_LOAD( Float, BackgroundSize, backgroundSize );
	REQUIRED_LOAD( Float, GapShrink, gapShrink );
	REQUIRED_LOAD( Float, Speed, speed );
}


void BackgroundComponent::Copy( BackgroundComponent* Target, BackgroundComponent* Base )
{
	Component::Copy( Target, Base );
	NAIVE_COPY( BackgroundSize );
	NAIVE_COPY( Speed );
	NAIVE_COPY( GapShrink );
}

void BackgroundComponent::OnAttach()
{
	std::vector<Sprite*> children = GetParentEntity()->GetComponentsByTypeAnyChild<Sprite>();
	for ( unsigned int Idx = 0; Idx < children.size(); ++Idx )
	{
		Entities_.push_back( children[ Idx ]->GetParentEntity() );
	}
}

void BackgroundComponent::CreateMap()
{
}

void BackgroundComponent::Update( float dt )
{
	for ( unsigned int Idx = 0; Idx < Entities_.size(); ++Idx )
	{
		auto pos = Entities_[ Idx ]->LocalPosition();
		float newX = pos.X() - dt * Speed_;

		if ( newX < -BackgroundSize_ * 0.5f )
			newX += ( BackgroundSize_ - GapShrink_ ) * 3.0f;
		pos.SetX( newX );
		Entities_[ Idx ]->SetLocalPosition( pos );
	}
}