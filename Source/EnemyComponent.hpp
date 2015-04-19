#ifndef EnemyCOMPONENT_HPP
#define EnemyCOMPONENT_HPP

#include "Bubblewrap/Render.hpp"
using namespace Bubblewrap;
using namespace Render;
class EnemyComponent
	: public Bubblewrap::Base::Component
{
public:
	EnemyComponent();
	void Initialise( Json::Value Params );

	CREATE_REGISTER( EnemyComponent);

	void OnAttach();

	PROTECTED_FIELD( float, Speed );
	PROTECTED_FIELD( float, GoFast );
	PROTECTED_FIELD( float, PlayerPosition );
	PROTECTED_FIELD( Sprite*, RenderSprite );
	void Update( float dt );
private:
};



#endif