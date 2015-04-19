#ifndef AnimatedComponent_HPP
#define AnimatedComponent_HPP

#include "Bubblewrap/Render.hpp"
#include "Bubblewrap/Base.hpp"

using namespace Bubblewrap;
using namespace Render;
class AnimatedComponent 
	: public Bubblewrap::Base::Component
{
public:
	AnimatedComponent();
	void Initialise( Json::Value Params );

	CREATE_REGISTER( AnimatedComponent );

	void OnAttach();

	PROTECTED_FIELD( float, FramesPerSecond );
	PROTECTED_FIELD( int, Frames );
	PROTECTED_FIELD( bool, DestroyOnCompletion );
	void Update( float dt );
private:
	Text* AnimatedText_;
	int CurrentFrame_;
	std::vector<int> FrameOrder_;
	Vertices* Vertices_;

	float TimeToNextFrame_;
};



#endif