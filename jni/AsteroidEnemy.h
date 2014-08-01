#pragma once
#ifndef __asteroidenemy_h__
#define __asteroidenemy_h__

#include <vector>
#include "Vertex.h"
#include "GraphicObject.h"
#include "BaseEnemy.h"
#include "Position.h"

namespace asteroids
{
	// Describes behavior of enemy asteroid.
	class AsteroidEnemy : public BaseEnemy, public GraphicObject
	{
	public:
		// Constructor.
		AsteroidEnemy(float playerX, float playerY);
		// Clears resources.
		virtual ~AsteroidEnemy();
		// Draws pentagon from triangles: full pentagon if 5 triangle could be build from _vertexes array.
		// If generates less than 5 vertexes, than we have not a full pentagon.
		virtual void Draw();
		// Updates coordinates, shape, etc.
		virtual void Update(float time, float total = 0);
		// Returns outer radius of the asteroid.
		virtual float GetOuterRadius();

		// Manipulates visibility.
		bool IsVisible();
		void SetVisible(bool visibility);
		bool IsVisibleInViewport();

		// Splits big asteroid to its small pieces.
		// Splits small asteroid into dust points.
		std::vector<AsteroidEnemy*> SplitToPieces();

		// Describes if asteroid can damage ship. For example, asteroids dust cannot damage ship but
		// big ones of course can.
		bool HasEffectiveDamage();

	private:
		// Returns random numbers for calculation new asteroid.
		int _GetRandomCountOfVertexes();
		// Returns number of triangles that make up the asteroid rectangle.
		int _GetTrianglesCount();

	private:

		std::vector<Vertex> _vertexes;
		float _radius;
		bool _isVisible;

		bool _isDust;

		bool _isPiece;
		float _deltaX;
		float _deltaY;

		float _pieceAngle;

		int shapeType;
	};

}

#endif /* __asteroidenemy_h__ */
