#ifndef PUZZLECOMPONENT_HPP
#define PUZZLECOMPONENT_HPP

#include "Bubblewrap/Render.hpp"
using namespace Bubblewrap;
using namespace Render;
class PuzzleComponent
	: public Bubblewrap::Base::Component
{
public:
	PuzzleComponent();
	void Initialise( Json::Value Params );

	CREATE_REGISTER( PuzzleComponent);

	void OnAttach();

	PROTECTED_FIELD( int, Height );
	PROTECTED_FIELD( int, Width );
	PROTECTED_FIELD( float, TileSize );
	PROTECTED_FIELD( Colour, BackgroundColour );
	void DoCreateMap( int width, int height );


	void InputFunction( Bubblewrap::Events::Event* Event );
private:
	Events::EventHandle InputHandle_;

	enum LocationType
	{
		Empty = 0x00,
		Player = 0x01,
		Safe = 0x02,
		Death = 0x04,
		Wall = 0x08
	};
	struct TileData
	{
		TileData( )
			: Left_( 0.0f ), Right_( 0.0f ), Top_( 0.0f ), Bottom_( 0.0f ), First_( 0 )
		{

		}
		TileData( float l, float r, float t, float b )
			: Left_( l ), Right_( r ), Top_( t ), Bottom_( b ), First_( 0 )
		{

		}
		float Left_;
		float Right_;
		float Top_;
		float Bottom_;
		int First_;
	};

	struct PositionInfo
	{
		LocationType Type_;
		int StartPoint_;
	};

	Vertices* VertexList;
	static const int MaxMapSize = 10;
	void CreateMap();
	PositionInfo MapInfo_[ MaxMapSize ][ MaxMapSize ];

	PositionInfo GetPositionInfo( int X, int Y );
	std::map<LocationType, TileData> TileData_;

	void UpdateTexCoords();

	void GenerateMap();
};



#endif