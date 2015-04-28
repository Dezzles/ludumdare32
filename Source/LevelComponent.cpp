#include "LevelComponent.hpp"
#include "PuzzleComponent.hpp"
#include "EnemyComponent.hpp"
#include "Bubblewrap/Events.hpp"

#include <cstring>

using namespace Math;

LevelComponent::LevelComponent()
{
	TimeToSpawn_ = 0;
	Kills_ = 0;
	PlayerScore_ = 0;
}

void LevelComponent::Initialise( Json::Value Params )
{
	Component::Initialise( Params );
	
	REQUIRED_LOAD( Float, SpawnX, spawnX );
	REQUIRED_LOAD( Float, SpawnY, spawnY );
	REQUIRED_LOAD( Float, SpawnRate, spawnRate );
	REQUIRED_LOAD( Float, PuzzleGap, puzzleGap );
	REQUIRED_LOAD( Float, EnemySpeed, enemySpeed );
	TimeToSpawn_ = SpawnRate_;
	++Kills_;
}


void LevelComponent::Copy( LevelComponent* Target, LevelComponent* Base )
{
	Component::Copy( Target, Base );
	NAIVE_COPY( SpawnX );
	NAIVE_COPY( SpawnY );
	NAIVE_COPY( SpawnRate );
	NAIVE_COPY( TimeToSpawn );
	NAIVE_COPY( PuzzleGap );
	NAIVE_COPY( EnemySpeed );
}

void LevelComponent::OnAttach()
{
	TextComponent_ = GetParentEntity()->GetComponentsByType<Text>( )[ 0 ];
}

void LevelComponent::Update( float dt )
{
	TimeToSpawn_ -= dt;
	SetPuzzleCoordinates();
	if ( TimeToSpawn_ < 0.0f )
	{
		SpawnRate_ = SpawnRate_ * 0.99f;
		if (SpawnRate_ < 1.0f )
			SpawnRate_ = 1.0f;
		TimeToSpawn_ += SpawnRate_;
		Base::Entity* enemy = dynamic_cast<Base::Entity*>( GetRegister().LoadObject( "basics:EnemyEntity", GetParentEntity() ) );
		enemy->SetLocalPosition( Vector3f( SpawnX_, SpawnY_, 0.0f ) );

		Base::Entity* puzzle = dynamic_cast< Base::Entity* > ( GetRegister().LoadObject("basics:PuzzleEntity", GetParentEntity( ) ) );

		PuzzleComponent* puzzleComponent = puzzle->GetComponentsByType<PuzzleComponent>()[ 0 ];
		puzzleComponent->SetEnemyObject( enemy->GetComponentsByType<EnemyComponent>()[ 0 ] );
		puzzleComponent->SetLevelObject( this );
		puzzleComponent->DoCreateMap( 4 + Kills_ / 30, 5 + Kills_ / 30, Kills_ / 5 );
		puzzle->SetLocalPosition( Math::Vector3f( -puzzleComponent->GetRenderWidth() - PuzzleGap_, PuzzleGap_, 0 ) );
		Puzzles_.push_back( puzzle );
		SetPuzzleCoordinates();
	}
	PlayerScore_ += dt * 100.0f;
	char buffer[ 256 ];
	memset( buffer, 0, 256 );
	sprintf( buffer, "Score: %d", ( int ) PlayerScore_ );
	TextComponent_->SetText( buffer );

}

void LevelComponent::RemovePuzzle( Base::Entity* Puzzle )
{
	for ( int Idx = 0; Idx < Puzzles_.size(); ++Idx )
	{
		if ( Puzzles_[ Idx ] == Puzzle )
		{
			Puzzles_.erase( Puzzles_.begin() + Idx );
			++Kills_;
			return;
		}
	}
	SetPuzzleCoordinates();
}

void LevelComponent::SetPuzzleCoordinates()
{
	float right = 800 - PuzzleGap_;
	float m = 800;
	for ( int Idx = 0; Idx < Puzzles_.size(); ++Idx )
	{
		PuzzleComponent* comp = Puzzles_[ Idx ]->GetComponentsByType< PuzzleComponent >()[ 0 ];
		comp->SetMaxPosition( m - comp->GetRenderWidth() );
		float left = right - comp->GetRenderWidth();

		comp->SetTargetXCoord( left );
		m = Puzzles_[ Idx ]->LocalPosition().X() - PuzzleGap_;
		right = left - PuzzleGap_;
	}

}

void LevelComponent::AddScore( float Score )
{
	PlayerScore_ += Score;
}