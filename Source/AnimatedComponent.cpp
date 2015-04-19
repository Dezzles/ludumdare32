#include "AnimatedComponent.hpp"

using namespace Math;

AnimatedComponent::AnimatedComponent()
{
	CurrentFrame_ = 0;
	DestroyOnCompletion_ = false;
}

void AnimatedComponent::Initialise( Json::Value Params )
{
	Component::Initialise( Params );
	REQUIRED_LOAD( Float, FramesPerSecond, framesPerSecond );
	REQUIRED_LOAD( Int, Frames, frames );
	OPTIONAL_LOAD( Bool, DestroyOnCompletion, destroyOnCompletion );
	for ( int i = 0; i < Params[ "frameOrder" ].size(); ++i )
	{
		int u = Params[ "frameOrder" ][ i ].asInt();
		FrameOrder_.push_back( u );
	}

	TimeToNextFrame_ = 1.0f / FramesPerSecond_;
}


void AnimatedComponent::Copy( AnimatedComponent* Target, AnimatedComponent* Base )
{
	Component::Copy( Target, Base );
	NAIVE_COPY( FramesPerSecond );
	NAIVE_COPY( Frames );
	NAIVE_COPY( FrameOrder );
	NAIVE_COPY( TimeToNextFrame );
	NAIVE_COPY( DestroyOnCompletion );
}

void AnimatedComponent::OnAttach()
{
	Vertices_ = GetParentEntity()->GetComponentsByType<Vertices>()[ 0 ];
}

void AnimatedComponent::Update( float dt )
{
	TimeToNextFrame_ -= dt;
	if ( TimeToNextFrame_ <= 0.0f )
	{
		TimeToNextFrame_ += 1.0f / FramesPerSecond_;
		++CurrentFrame_;

		if ( CurrentFrame_ >= FrameOrder_.size() )
		{
			CurrentFrame_ = 0;/**/
			if ( DestroyOnCompletion_ )
				GetParentEntity()->Destroy();
		}
	}
	float width = 1.0f / (float)Frames_;
	float left = FrameOrder_[ CurrentFrame_ ] * width;

	Vertices_->EditVertex( 0 ).TexCoords_ = Math::Vector2f( left, 0.0f );
	Vertices_->EditVertex( 1 ).TexCoords_ = Math::Vector2f( left + width, 0.0f );
	Vertices_->EditVertex( 2 ).TexCoords_ = Math::Vector2f( left, 1.0f );
	Vertices_->EditVertex( 3 ).TexCoords_ = Math::Vector2f( left + width, 1.0f );
	Vertices_->Refresh();
}