#ifndef LEVELCOMPONENT_HPP
#define LEVELCOMPONENT_HPP

#include "Bubblewrap/Render.hpp"
using namespace Bubblewrap;
using namespace Render;
class LevelComponent
	: public Bubblewrap::Base::Component
{
public:
	LevelComponent();
	void Initialise( Json::Value Params );

	CREATE_REGISTER( LevelComponent);

	void OnAttach();
	PROTECTED_FIELD( int, SpawnX );
	PROTECTED_FIELD( int, SpawnY );
	PROTECTED_FIELD( float, SpawnRate );
	PROTECTED_FIELD( float, PuzzleGap );
	PROTECTED_FIELD( float, EnemySpeed );

	virtual void Update( float dt );

	void SetPuzzleCoordinates();
	void RemovePuzzle( Base::Entity* puzzle );
private:
	float TimeToSpawn_;
	std::vector< Base::Entity* > Puzzles_;
};



#endif