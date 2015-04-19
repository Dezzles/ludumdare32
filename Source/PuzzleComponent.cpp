#include "PuzzleComponent.hpp"

#include "Bubblewrap/Events.hpp"
using namespace Math;

PuzzleComponent::PuzzleComponent()
{
	TileSize_ = 40;
	for ( int i = 0; i < MaxMapSize; ++i )
	{
		for ( int j = 0; j < MaxMapSize; ++j )
		{
			MapInfo_[ i ][ j ].Type_ = LocationType::Wall;
		}
	}

	for ( int i = 0; i < MaxMapSize; ++i )
	{
		for ( int j = 0; j < MaxMapSize; ++j )
		{
			MapInfo_[ i ][ j ].Type_ = LocationType::Empty;
		}
	}

	TileData_[ LocationType::Empty ] = TileData( 0.00f, 0.25f, 0.50f, 0.75f );
	TileData_[ LocationType::Player ] = TileData( 0.25f, 0.50f, 0.25f, 0.50f );
	TileData_[ LocationType::Safe ] =	TileData( 0.75f, 1.00f, 0.00f, 0.25f );
	TileData_[ LocationType::Death ] =	TileData( 0.50f, 0.75f, 0.00f, 0.25f );
	TileData_[ LocationType::Wall ] =	TileData( 0.00f, 0.25f, 0.00f, 0.25f );
}

void PuzzleComponent::Initialise( Json::Value Params )
{
	Component::Initialise( Params );
	
	REQUIRED_LOAD( Float, TileSize, tileSize );
	REQUIRED_LOAD( Int, Height, height );
	REQUIRED_LOAD( Int, Width, width );
	if ( !Params[ "backgroundColour" ].isNull() )
	{
		BackgroundColour_ = Colour( Params[ "backgroundColour" ].asString() );
	}
}


void PuzzleComponent::Copy( PuzzleComponent* Target, PuzzleComponent* Base )
{
	Component::Copy( Target, Base );
	NAIVE_COPY( TileSize );
	NAIVE_COPY( Width );
	NAIVE_COPY( Height );
	NAIVE_COPY( BackgroundColour );
}

void PuzzleComponent::OnAttach()
{
	DoCreateMap( 4, 5);
	InputHandle_ = GetManager().GetEventManager().RegisterEvent( Events::EventTypes::Input, std::bind( &PuzzleComponent::InputFunction, this, std::placeholders::_1 ) );
}

void PuzzleComponent::DoCreateMap( int width, int height )
{
	Width_ = width;
	Height_ = height;
	GenerateMap();
	CreateMap();
}

void PuzzleComponent::CreateMap()
{
	Vertices* vertices = GetParentEntity()->GetComponentsByType<Vertices>( "BorderVertices" )[ 0 ];
	float height = TileSize_ * Height_;
	float width = TileSize_ * Width_;
	for ( int i = 0; i < Width_ + 1; ++i )
	{
		Vertex v;
		v.Position_ = Math::Vector3f( i * TileSize_, 0, 0 );
		v.Colour_ = Colour( ( unsigned char ) 255, 255, 255 );
		vertices->AddVertex( v );

		v.Position_ = Math::Vector3f( i * TileSize_, height, 0 );
		vertices->AddVertex( v );
	}
	for ( int i = 0; i < Height_ + 1; ++i )
	{
		Vertex v;
		v.Position_ = Math::Vector3f( 0, i * TileSize_, 0 );
		v.Colour_ = Colour( ( unsigned char ) 255, 255, 255 );
		vertices->AddVertex( v );

		v.Position_ = Math::Vector3f( width, i * TileSize_, 0 );
		vertices->AddVertex( v );
	}
	vertices->Refresh();

	vertices = GetParentEntity()->GetComponentsByType<Vertices>( "FillVertices" )[ 0 ];
	{
		Vertex v;
		v.Position_ = Math::Vector3f( 0, 0, 0 );
		v.Colour_ = BackgroundColour_;
		vertices->AddVertex( v );

		v.Position_ = Math::Vector3f( 0, height, 0 );
		v.Colour_ = BackgroundColour_;
		vertices->AddVertex( v );

		v.Position_ = Math::Vector3f( width, 0, 0 );
		v.Colour_ = BackgroundColour_;
		vertices->AddVertex( v );

		v.Position_ = Math::Vector3f( width, height, 0 );
		v.Colour_ = BackgroundColour_;
		vertices->AddVertex( v );
	}
	vertices = GetParentEntity()->GetComponentsByType<Vertices>( "TileVertices" )[ 0 ];
	for ( int i = 0; i < Width_; ++i )
	{
		for ( int j = 0; j < Height_; ++j )
		{
			Vertex v1, v2;
			v1.Colour_ = Colour( 1.0f, 1.0f, 1.0f );
			v2.Colour_ = Colour( 1.0f, 1.0f, 1.0f );
			v1.Position_ = Math::Vector3f( i * TileSize_, j * TileSize_, 0.0f );
			MapInfo_[ i ][ j ].StartPoint_ = vertices->AddVertex( v1 );
			v1.Position_ = Math::Vector3f( ( i + 1 ) * TileSize_, ( j + 0 ) * TileSize_, 0.0f );
			v2.Position_ = Math::Vector3f( ( i + 0 ) * TileSize_, ( j + 1 ) * TileSize_, 0.0f );
			vertices->AddVertex( v1 );
			vertices->AddVertex( v2 );
			vertices->AddVertex( v1 );
			vertices->AddVertex( v2 );
			v2.Position_ = Math::Vector3f( ( i + 1 ) * TileSize_, ( j + 1 ) * TileSize_, 0.0f );
			vertices->AddVertex( v2 );
		}
	}
	UpdateTexCoords();
	vertices->Refresh();
}

void PuzzleComponent::UpdateTexCoords()
{
	Vertices* vertices = GetParentEntity()->GetComponentsByType<Vertices>( "TileVertices" )[ 0 ];
	for ( int i = 0; i < Width_; ++i )
	{
		for ( int j = 0; j < Height_; ++j )
		{
			TileData data = TileData_[ MapInfo_[ i ][ j ].Type_ ];
			vertices->EditVertex( MapInfo_[ i ][ j ].StartPoint_ + 0 ).TexCoords_ = Math::Vector2f( data.Left_, data.Top_ );
			vertices->EditVertex( MapInfo_[ i ][ j ].StartPoint_ + 1 ).TexCoords_ = Math::Vector2f( data.Right_, data.Top_ );
			vertices->EditVertex( MapInfo_[ i ][ j ].StartPoint_ + 2 ).TexCoords_ = Math::Vector2f( data.Left_, data.Bottom_ );
			vertices->EditVertex( MapInfo_[ i ][ j ].StartPoint_ + 3 ).TexCoords_ = Math::Vector2f( data.Right_, data.Top_ );
			vertices->EditVertex( MapInfo_[ i ][ j ].StartPoint_ + 4 ).TexCoords_ = Math::Vector2f( data.Left_, data.Bottom_);
			vertices->EditVertex( MapInfo_[ i ][ j ].StartPoint_ + 5 ).TexCoords_ = Math::Vector2f( data.Right_, data.Bottom_);
		}
	}
	vertices->Refresh();
}

PuzzleComponent::PositionInfo PuzzleComponent::GetPositionInfo( int X, int Y )
{
	if ( ( X < 0 ) || ( X >= Width_ ) || ( Y < 0 ) || ( Height_ <= Y ) )
	{
		PositionInfo ret;
		ret.Type_ = LocationType::Wall;
		return ret;
	}
	return MapInfo_[ X ][ Y ];
}

void PuzzleComponent::GenerateMap()
{
	int posx = rand() % Width_;
	int posy = rand() % Height_;
	MapInfo_[ posx ][ posy ].Type_ = LocationType::Player;

	posx = rand() % Width_;
	posy = rand() % Height_;
	while ( MapInfo_[ posx ][ posy ].Type_ != LocationType::Empty )
	{
		posx = rand() % Width_;
		posy = rand() % Height_;
	}
	MapInfo_[ posx ][ posy ].Type_ = LocationType::Safe;

	posx = rand() % Width_;
	posy = rand() % Height_;
	while ( MapInfo_[ posx ][ posy ].Type_ != LocationType::Empty )
	{
		posx = rand() % Width_;
		posy = rand() % Height_;
	}
	MapInfo_[ posx ][ posy ].Type_ = LocationType::Death;

}

void PuzzleComponent::InputFunction( Bubblewrap::Events::Event* Event )
{
	Bubblewrap::Events::InputData data = *( Bubblewrap::Events::InputData* )Event->GetData();
	int xMod = 0;
	int yMod = 0;
	if (data.InputType_ == Events::InputData::InputType::KeyDown )
	switch ( data.Key_ )
	{
	case Events::Key::W:
		yMod = -1;
		break;
	case Events::Key::S:
		yMod = 1;
		break;
	case Events::Key::A:
		xMod = -1;
		break;
	case Events::Key::D:
		xMod = 1;
		break;
	}

	int playerX = 0;
	int playerY = 0;
	bool playerFound = false;
	for ( playerX = 0; playerX < Width_; ++playerX )
	{
		for ( playerY = 0; playerY < Height_; ++playerY )
		{
			if ( MapInfo_[ playerX ][ playerY ].Type_ & LocationType::Player )
			{
				playerFound = true;
				break;
			}
		}
		if ( playerFound )
			break;
	}

	if ( GetPositionInfo( playerX + xMod, playerY + yMod ).Type_ != LocationType::Wall )
	{
		MapInfo_[ playerX ][ playerY ].Type_ = (LocationType)( MapInfo_[ playerX ][ playerY ].Type_ - ( LocationType::Player ));
		MapInfo_[ playerX + xMod ][ playerY + yMod ].Type_ = (LocationType)(LocationType::Player | MapInfo_[ playerX + xMod ][ playerY + yMod ].Type_);
	}
	UpdateTexCoords();
	
}