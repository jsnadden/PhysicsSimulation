#include "Collision.hpp"

Collision* Collision::instance = nullptr;

Collision::Collision()
{

}

Collision::~Collision()
{

}

Collision* Collision::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Collision();
	}

	return instance;
}

void Collision::Release()
{
	delete instance;
	instance = nullptr;
}

bool Collision::Disks(Disk dA, Disk dB)
{
	Vector2D separation = dA.centre - dB.centre;
	return (separation.NormSquared() <= (dA.radius + dB.radius) * (dA.radius + dB.radius));
}

bool Collision::RayRect(const Vector2D& rayOrigin, const Vector2D& rayDirection, const Rect& target,
	Vector2D& contactPoint, Vector2D& contactNormal, float& contactTime)
{

	contactNormal = { 0, 0 };
	contactPoint = { 0, 0 };

	// Cache division
	Vector2D invdir = { 1.0f / rayDirection.x, 1.0f / rayDirection.y };

	// Calculate intersections with rectangle bounding axes
	Vector2D t_near = (target.GetPosition() - rayOrigin) * invdir;
	Vector2D t_far = (target.GetPosition() + target.GetSize() - rayOrigin) * invdir;

	if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false;
	if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;

	// Sort distances
	if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
	if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

	// Early rejection		
	if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

	// Closest 'time' will be the first contact
	contactTime = std::max(t_near.x, t_near.y);

	// Furthest 'time' is contact on opposite side of target
	float t_hit_far = std::min(t_far.x, t_far.y);

	// Reject if ray direction is pointing away from object
	if (t_hit_far < 0)
		return false;

	// Contact point of collision from parametric line equation
	contactPoint = rayOrigin + contactTime * rayDirection;

	if (t_near.x > t_near.y)
		if (invdir.x < 0)
			contactNormal = VEC_RIGHT;
		else
			contactNormal = VEC_LEFT;
	else if (t_near.x < t_near.y)
		if (invdir.y < 0)
			contactNormal = VEC_DOWN;
		else
			contactNormal = VEC_UP;

	// Note if t_near == t_far, collision is principly in a diagonal
	// By returning a CN={0,0} even though its
	// considered a hit, the resolver wont change anything.
	return true;
}

bool Collision::SweptAABB(const Rect& colliderA, const Rect& colliderB, float dt,
	Vector2D& contactPos, Vector2D& contactNormal, float& contactTime)
{

	// Check if dynamic rectangle is actually moving - we assume rectangles are NOT in collision to start
	if (colliderA.vx == 0.0f && colliderA.vy == 0.0f)
		return false;

	// Expand target rectangle by source dimensions
	Rect expandedB = colliderB + colliderA;

	if (RayRect(colliderA.Centre(), (colliderA.GetVelocity() - colliderB.GetVelocity()) * dt, expandedB, contactPos, contactNormal, contactTime))
		return (contactTime >= 0.0f && contactTime < 1.0f);
	else
		return false;

}

bool Collision::AABB(const Rect rectA, const Rect rectB)
{
	return (rectA.x + rectA.w >= rectB.x)
		&& (rectB.x + rectB.w >= rectA.x)
		&& (rectA.y + rectA.h >= rectB.y)
		&& (rectB.y + rectB.h >= rectA.y);

}

bool Collision::ResolveSAT_Static(Polygon& p1, Polygon& p2)
{
	Polygon* poly1 = &p1;
	Polygon* poly2 = &p2;

	float overlap = INFINITY;

	for (int shape = 0; shape < 2; shape++)
	{
		if (shape == 1)
		{
			poly1 = &p2;
			poly2 = &p1;
		}

		for (int a = 0; a < poly1->Size(); a++)
		{
			Vector2D axisProj = (poly1->vertices[a + 1] - poly1->vertices[a]).Orth();
			axisProj.Normalise();

			// Work out min and max 1D points for p1
			float min_p1 = INFINITY, max_p1 = -INFINITY;
			for (int p = 0; p < poly1->Size(); p++)
			{
				float q = (poly1->centre + poly1->vertices[p]).Dot(axisProj);
				min_p1 = std::min(min_p1, q);
				max_p1 = std::max(max_p1, q);
			}

			// Work out min and max 1D points for r2
			float min_p2 = INFINITY, max_p2 = -INFINITY;
			for (int p = 0; p < poly2->Size(); p++)
			{
				float q = (poly2->centre + poly2->vertices[p]).Dot(axisProj);
				min_p2 = std::min(min_p2, q);
				max_p2 = std::max(max_p2, q);
			}

			// Calculate actual overlap along projected axis, and store the minimum
			overlap = std::min(std::min(max_p1, max_p2) - std::max(min_p1, min_p2), overlap);

			if (!(max_p2 >= min_p1 && max_p1 >= min_p2))
				return false;
		}
	}

	// If we got here, the objects have collided, we will displace r1
	// by overlap along the vector between the two object centers
	Vector2D d = poly2->centre - poly1->centre;
	float s = d.Norm();
	poly1->centre -= 0.5f * overlap * d / s;
	poly2->centre += 0.5f * overlap * d / s;
	return false;
}


// TODO: try implementing the following
/*---------------------------------------------------------------------------
                                                                                                                                            
"Ron Levine" <ron@dorianresearch.com>
Subject:     Re: [Algorithms] Collisions of moving objects
11/14/2000 02:20 PM

For convex polyhedra moving with uniform rectilinear motion (i.e. constant
velocity, no spin), determining whether or not collision occurs and its
exact time in case collision does occur is really no more difficult than
determining whether or not two stationary convex polyhedra intersect.

The idea uses the separating axis theorem.  Recall that this theorem gives
you a finite set of axes such that if the projections of the two bodies onto
on every axis of the set intersect, then you know that the two bodies
intersect in space.  In other words, if the two bodies are separated in
space, then their two projections onto at least one of the separating axes
are separated.

The algorithm goes like this.  You work with the relative velocity vector of
the two convex bodies.  Projecting each of the two bodies and the relative
velocity vector onto a particular separating axis at t0 gives two 1-D
intervals and a 1-D velocity, such that it is easy to tell whether the two
intervals intersect, and if not,  whether they are moving apart or moving
together.  If they are separated and moving apart on any of the separating
axes (or, in fact, on any axis whatever), then you know that there is no
future collision.  If on any separating axis the two projected intervals
intersect at t0 or are separated and are moving together, then it is easy to
compute (by two  simple 1D linear expressions) the earliest future time at
which the two intervals will first intersect and (assuming continuing
rectilinear motion) the latest future time at which the two intervals will
last intersect and begin moving apart.  (If they are intersecting at t0 then
the earliest future intersection time is t0).  Do this for at most all the
separating axes.  If the maximum over all the axes of the earliest future
intersection time is less than the minimum over all the axes of the latest
future intersection time then that maximum earliest future intersection time
is the exact time of first collision of the two 3D polyhedra, otherwise
there is no collision in the future.

The algorithm has the possibility of early outs as you loop over the
separating axes, for if ever the maximum earliest future intersection time
for the axes tested so far is greater than the minimum latest future
intersection time for the axes tested so far, then you know there is no
future collision and are done.


---------------------------------------------------------------------------

Ron Levine <ron@dorianresearch.com>
Subject:     Re: [Algorithms] collision detection and movement
05/04/2002 01:06 PM

Continuing my previous post, here is the elegant solution for finding
the exact time of collision of two axis-aligned boxes in uniform
linear 3D motion.  Actually the method also works for non-axis-aligned
boxes (OBBs) and even for arbitrary convex polyhedra, when you combine
it with the method of separatig axes.  But not to assume you know the
method of separating axes, and to keep the elucidation of the idea
simple, I'll describe the restricted version that applies to the case
of axis-aligned boxes.

Suppose your two boxes are given at time t = 0 by
([x1min, x1max], [y1min, y1max], [z1min,z1max]) and
([x2min, x2max], [y2min, y2max], [z2min,z2max])

and suppose that V1 and V2 are their respective 3D velocity vectors,

Let W = V2 - V1 be their relative velocity vector.

The overview of the algorithm goes like this: We treat the three axes
(x,y,z) in turn.  At any time, the projection of each box onto an axis
is an interval of constant length that is moving uniformly in time
along the axis, and the relative speed of the two projection intervals
is the projection of W onto that axis, thus Wx for the x-axis, etc.
Note that the two projection intervals on one axis can overlap without
the boxes intersecting in 3D space.  In fact, the boxes intersect in
3D space only when the projection intervals overlap SIMULTANEOUSLY on
all three axes. From this consideration, the validity of the following
algorithm should be clear:

For each axis, find the earliest future time (including possibly the
present) t >= 0   such that the two moving projection intervals
overlap and find the latest future time at which they overlap,
assuming continued constant relative speed. (see below)  If on any
axis there is no future time at which the projection intervals
overlap, then there is no future collision.  Now consider the maximum
over all three axes of the earliest future overlap time and the
minimum over all three axes of the latest future overlap time.  If the
maximum earliest future overlap time is greater than the minimum
latest future overlap time, then there is no collision in the future.
If the maximum earliest future overlap time is less than or equal to
the minimum latest future overlap time, then that maximum earliest
future overlap time is the exact time of first collision.

Now, let's look in a little more detail at the determination of
earliest future overlap time and latest future overlap time on the
x-axis.

First suppose that Wx > 0

If x1max < x2min, then the projection of box 2 lies to the right of
the projection of box2 and is moving further to the right, so there is
no future overlap time and so no future collision of the 3D boxes.

If x1min <= x2min <= x1max  or x2min < x1min <= x2max
then the boxes are overlapping at t=0 so the earliest future (or
present) overlap time is 0. But box 2 is still moving to the right
(with respect to box1) and the projection intervals come apart at time
(x1max -- x2min)/Wx, so that is the latest future overlap time.

If x2max < x1min, then the earliest future overlap time is
(x1min - x2max)/Wx and the latest future overlap time is
again (x1max - x2min)/Wx

Now suppose that Wx < 0.  Then you use exactly the same procedure, but
with the roles of box1 and box2 reversed, to get the earliest and
latest overlap times.

If Wx = 0, then we see a divide by zero problem.  In this case there
is no relative motion on the x-axis so it comes down to whether the
two intervals [x1min, x1max] and [x2min, x2max] overlap.  If yes then
both the earilest and latest future overlap times are 0. If no, then
there is no future intersection of the 3D boxes.

QED.

For OBBs, according to the separating axis theorem, there are 15 axes
to check instead of three, and the per axis computation is pretty much
the same except that you have to use some dot products to get the
projections. As a practical matter, you get accurate enough results
for game situations by checking only six of the 15  axes that you need
for an exact result.   And similarly, it extends to arbitrary convex
polyhedra, with the number of axes to be checked growing with the
number of faces and edges of the polyhedra.

Note that this algorithm has frequent early outs in the case of
non-collision, so if you know something about the distribution of
positions and velocities of your colliding bodies, you can speed it up
by appropriately ordering the separating axes.

I discovered this very elegant algorithm for exact collision time and
used it with OBBs in a published game about five years ago, but my
client at the time would not allow me to publiciZe it (competitive
advantage, non-disclosure agreements, etc.)  I did describe it tersely
in a post to this list a couple of years ago.  No one seemed to notice
or appreciate it except Dave Eberly. You can find code implementing it
on his web site, www.magic-software.com.

---------------------------------------------------------------------------*/