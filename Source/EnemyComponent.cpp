#include "EnemyComponent.hpp"
#include "ScoreComponent.hpp"
#include "LevelComponent.hpp"
#include "Bubblewrap/Events.hpp"
using namespace Math;

EnemyComponent::EnemyComponent()
{
	Speed_ = 40.0f;
	RenderSprite_ = nullptr;
	GoFast_ = false;
}

void EnemyComponent::Initialise( Json::Value Params )
{
	Component::Initialise( Params );
	OPTIONAL_LOAD( Float, Speed, speed );
	REQUIRED_LOAD( Float, PlayerPosition, playerPosition );
}


void EnemyComponent::Copy( EnemyComponent* Target, EnemyComponent* Base )
{
	Component::Copy( Target, Base );
	NAIVE_COPY( Speed );
	NAIVE_COPY( PlayerPosition );
}

void EnemyComponent::OnAttach()
{
	RenderSprite_ = GetParentEntity()->GetComponentsByType<Sprite>()[ 1 ];
}

void EnemyComponent::Update( float dt )
{
	float Mod = GoFast_ ? 3.0f : 1.0f;
	auto pos = GetParentEntity()->LocalPosition();
	pos.SetX( pos.X() + dt * Speed_ * Mod );

	GetParentEntity()->SetLocalPosition( pos );
	Colour myColour( 1.0f, 1.0f, 1.0f );
	if ( GoFast_ )
		myColour = Colour( 1.0f, 0.0f, 0.0f );
	if ( RenderSprite_ != nullptr )
		RenderSprite_->SetColour( myColour );

	if ( pos.X() > PlayerPosition_ )
	{
		GetParentEntity()->GetRootEntity()->Destroy();

		Base::Entity* entity = dynamic_cast< Base::Entity* >( GetRegister().LoadObject( "basics:GameOverEntity", nullptr ) );
		ScoreComponent* component = entity->GetComponentsByTypeAnyChild< ScoreComponent >()[ 0 ];
		LevelComponent* level = GetParentEntity()->GetRootEntity()->GetComponentsByTypeAnyChild< LevelComponent >()[ 0 ];
		component->SetScore( level->GetPlayerScore() );
	}
}

