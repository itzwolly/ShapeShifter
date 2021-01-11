#include "mge/LineSegmentFlat.h"
#include "mge/PhysicsManagerFlat.h"


LineSegmentFlat::LineSegmentFlat(sf::Vector2f* start, sf::Vector2f* end):Start(start),End(end)
{

}

bool LineSegmentFlat::Intersects(LineSegmentFlat * other, sf::Vector2f& outIntersection, bool considerCollinearOverlapAsIntersect)
{
	sf::Vector2f p = *Start;
	sf::Vector2f q = *other->Start;

	sf::Vector2f r = *End - *Start;
	sf::Vector2f s = *other->End-*other->Start;
	float rxs = Cross(r,s);
	float qpxr = Cross((*other->Start - *Start),r);

	// If r x s = 0 and (q - p) x r = 0, then the two lines are collinear.
	if (rxs==0 && qpxr==0)
	{
		// 1. If either  0 <= (q - p) * r <= r * r or 0 <= (p - q) * s <= * s
		// then the two lines are overlapping,
		if (considerCollinearOverlapAsIntersect)
			if ((0 <= Multiply((q - p),r) && Multiply((q - p),r) <= Multiply(r,r)) || (0 <= Multiply((p - q),s) && Multiply((p - q),s) <= Multiply(s,s)))
				return true;

		// 2. If neither 0 <= (q - p) * r = r * r nor 0 <= (p - q) * s <= s * s
		// then the two lines are collinear but disjoint.
		// No need to implement this expression, as it follows from the expression above.
		return false;
	}

	// 3. If r x s = 0 and (q - p) x r != 0, then the two lines are parallel and non-intersecting.
	if (rxs==0 && qpxr!=0)
		return false;

	// t = (q - p) x s / (r x s)
	float t = Cross((q - p),(s)) / rxs;

	// u = (q - p) x r / (r x s)

	float u = Cross((q - p),(r)) / rxs;

	// 4. If r x s != 0 and 0 <= t <= 1 and 0 <= u <= 1
	// the two line segments meet at the point p + t r = q + u s.
	if (rxs!=0 && (0 <= t && t <= 1) && (0 <= u && u <= 1))
	{
		// We can calculate the intersection point using either t or u.
		outIntersection = p + t*r;

		// An intersection was found.
		return true;
	}

	// 5. Otherwise, the two line segments are not parallel but do not intersect.
	return false;
}

bool LineSegmentFlat::CircleTouching(sf::Vector2f circle,float radius)
{
	if (PhysicsManagerFlat::CheapLenght(circle - GetUnderneath(circle), 20) < radius)
	{
		return true;
	}
	return false;
}

LineSegmentFlat::~LineSegmentFlat()
{
}

void LineSegmentFlat::Draw(sf::RenderWindow* wind)
{
	sf::Vertex line[] =
	{
		*Start,
		*End
	};

	wind->draw(line, 2, sf::Lines);
}

sf::Vector2f LineSegmentFlat::GetNormal()
{
	sf::Vector2f onOrigin = *End - *Start;
	return sf::Vector2f(-onOrigin.x, onOrigin.y);
	//return sf::Vector2f(onOrigin.x, -onOrigin.y);//for other normal
}

sf::Vector2f LineSegmentFlat::GetUnderneath(sf::Vector2f point)
{
	sf::Vector2f onOrigin = PhysicsManagerFlat::CheapNormalize((*End - *Start),20);
	sf::Vector2f pointOnOrigin = point - *Start;
	//std::cout << End->x << " " << End->y << " || " << Start->x << " " << Start->y << " || " << pointOnOrigin.x << " " << pointOnOrigin.y << std::endl;
	float amount = PhysicsManagerFlat::Dot(pointOnOrigin,onOrigin);
	sf::Vector2f npoint = (onOrigin*amount) + *Start;
	return npoint;
}

float LineSegmentFlat::Cross(sf::Vector2f start, sf::Vector2f end)
{
	return start.x*end.y-start.y*end.x;
}

float LineSegmentFlat::Multiply(sf::Vector2f s, sf::Vector2f d)
{
	return s.x*d.x + s.y*d.y;
}
