// actions.cpp
//
// Copyright 1997-1998 by David K. McAllister
// Based in part on code Copyright 1997 by Jonathan P. Leech
//
// This file implements the dynamics of particle actions.

#include "papi.h"

#ifdef MACOSX
#include "/usr/include/float.h"
#else
#include <float.h>
#endif

void PABounce::Execute(ParticleGroup *group)
{
	if(position.type == PDTriangle)
	{
		// Compute the inverse matrix of the plane basis.
		pVector &u = position.u;
		pVector &v = position.v;

		// w = u cross v
		float wx = u.y*v.z-u.z*v.y;
		float wy = u.z*v.x-u.x*v.z;
		float wz = u.x*v.y-u.y*v.x;

		float det = 1/(wz*u.x*v.y-wz*u.y*v.x-u.z*wx*v.y-u.x*v.z*wy+v.z*wx*u.y+u.z*v.x*wy);
		
		pVector s1((v.y*wz-v.z*wy), (v.z*wx-v.x*wz), (v.x*wy-v.y*wx));
		s1 *= det;
		pVector s2((u.y*wz-u.z*wy), (u.z*wx-u.x*wz), (u.x*wy-u.y*wx));
		s2 *= -det;
		
		// See which particles bounce.
		for (int i = 0; i < group->p_count; i++) {
			Particle &m = group->list[i];
			
			// See if particle's current and next positions cross plane.
			// If not, couldn't bounce, so keep going.
			pVector pnext(m.pos + m.vel * dt);
			
			// p2 stores the plane normal (the a,b,c of the plane eqn).
			// Old and new distances: dist(p,plane) = n * p + d
			// radius1 stores -n*p, which is d.
			float distold = m.pos * position.p2 + position.radius1;
			float distnew = pnext * position.p2 + position.radius1;
			
			// Opposite signs if product < 0
			// Is there a faster way to do this?
			if (distold * distnew >= 0)
				continue;
			
			// Find position at the crossing point by parameterizing
			// p(t) = pos + vel * t
			// Solve dist(p(t),plane) = 0 e.g.
			// n * p(t) + D = 0 ->
			// n * p + t (n * v) + D = 0 ->
			// t = -(n * p + D) / (n * v)
			// Could factor n*v into distnew = distold + n*v and save a bit.
			// Safe since n*v != 0 assured by quick rejection test.
			// This calc is indep. of dt because we have established that it
			// will hit before dt. We just want to know when.
			float nv = position.p2 * m.vel;
			float t = -distold / nv;
			
			// Actual intersection point p(t) = pos + vel t
			pVector phit(m.pos + m.vel * t);
			
			// Offset from origin in plane, p - origin
			pVector offset(phit - position.p1);
			
			// Dot product with basis vectors of old frame
			// in terms of new frame gives position in uv frame.
			float upos = offset * s1;
			float vpos = offset * s2;
			
			// Did it cross plane outside triangle?
			if (upos < 0 || vpos < 0 || (upos + vpos) > 1)
				continue;
			
			// A hit! A most palatable hit!
			
			// Compute tangential and normal components of velocity
			pVector vn(position.p2 * nv); // Normal Vn = (V.N)N
			pVector vt(m.vel - vn); // Tangent Vt = V - Vn
			
			// Don't apply friction if tangential velocity < cutoff
			float appliedFriction = oneMinusFriction;
			if (cutoffSqr > 0) {
				float vtmag = vt.length2();
				
				if (vtmag <= cutoffSqr)
					appliedFriction = 1;
			}
			
			// Compute new velocity heading out:
			// V' = (1-mu) Vt - resilience Vn
			m.vel = vt * appliedFriction - vn * resilience;
		}
	}
	else if(position.type == PDPlane)
	{
		// Compute the inverse matrix of the plane basis.
		pVector &u = position.u;
		pVector &v = position.v;

		// w = u cross v
		float wx = u.y*v.z-u.z*v.y;
		float wy = u.z*v.x-u.x*v.z;
		float wz = u.x*v.y-u.y*v.x;

		float det = 1/(wz*u.x*v.y-wz*u.y*v.x-u.z*wx*v.y-u.x*v.z*wy+v.z*wx*u.y+u.z*v.x*wy);
		
		pVector s1((v.y*wz-v.z*wy), (v.z*wx-v.x*wz), (v.x*wy-v.y*wx));
		s1 *= det;
		pVector s2((u.y*wz-u.z*wy), (u.z*wx-u.x*wz), (u.x*wy-u.y*wx));
		s2 *= -det;
		
		// See which particles bounce.
		for (int i = 0; i < group->p_count; i++) {
			Particle &m = group->list[i];
			
			// See if particle's current and next positions cross plane.
			// If not, couldn't bounce, so keep going.
			pVector pnext(m.pos + m.vel * dt);
			
			// p2 stores the plane normal (the a,b,c of the plane eqn).
			// Old and new distances: dist(p,plane) = n * p + d
			// radius1 stores -n*p, which is d.
			float distold = m.pos * position.p2 + position.radius1;
			float distnew = pnext * position.p2 + position.radius1;
			
			// Opposite signs if product < 0
			if (distold * distnew >= 0)
				continue;
			
			// Find position at the crossing point by parameterizing
			// p(t) = pos + vel * t
			// Solve dist(p(t),plane) = 0 e.g.
			// n * p(t) + D = 0 ->
			// n * p + t (n * v) + D = 0 ->
			// t = -(n * p + D) / (n * v)
			// Could factor n*v into distnew = distold + n*v and save a bit.
			// Safe since n*v != 0 assured by quick rejection test.
			// This calc is indep. of dt because we have established that it
			// will hit before dt. We just want to know when.
			float t = -distold / (position.p2 * m.vel);
			
			// Actual intersection point p(t) = pos + vel t
			pVector phit(m.pos + m.vel * t);
			
			// Offset from origin in plane, p - origin
			pVector offset(phit - position.p1);
			
			// Dot product with basis vectors of old frame
			// in terms of new frame gives position in uv frame.
			float upos = offset * s1;
			float vpos = offset * s2;
			
			// Crossed plane outside bounce region if !(0<=[uv]pos<=1)
			if (upos < 0 || upos > 1 ||
				vpos < 0 || vpos > 1)
				continue;
			
			// A hit! A most palatable hit!
			
			// Compute tangential and normal components of velocity
			float scale = m.vel * position.p2;
			pVector vn(position.p2 * scale); // Normal Vn = (V.N)N
			pVector vt(m.vel - vn); // Tangent Vt = V - Vn
			
			// Don't apply friction if tangential velocity < cutoff
			float appliedFriction = oneMinusFriction;
			if (cutoffSqr > 0) {
				float vtmag = vt.length2();
				
				if (vtmag <= cutoffSqr)
					appliedFriction = 1;
			}
			
			// Compute new velocity heading out:
			// V' = (1-mu) Vt - resilience Vn
			m.vel = vt * appliedFriction - vn * resilience;
		}
	}
	else if(position.type == PDSphere)
	{
		// Sphere that particles bounce off
		// The particles are always forced out of the sphere.
		for (int i = 0; i < group->p_count; i++)
		{
			Particle &m = group->list[i];
			
			// See if particle's next position is inside domain. If so,
			// bounce it.
			pVector pnext(m.pos + m.vel * dt);
			
			if (position.Within(pnext)) {
				// See if we were inside on previous timestep.
				bool pinside = position.Within(m.pos);
				
				// Normal to surface. This works for a sphere. Isn't
				// computed quite right, should extrapolate particle
				// position to surface.
				pVector n(m.pos - position.p1);
				n.normalize();
				
				// Compute tangential and normal components of velocity
				float scale = m.vel * n;
				
				pVector vn(n * scale); // Normal Vn = (V.N)N
				pVector vt = m.vel - vn; // Tangent Vt = V - Vn
				
				if (pinside) {
					// Previous position was inside. If normal component of
					// velocity points in, reverse it. This effectively
					// repels particles which would otherwise be trapped
					// in the sphere.
					if (scale < 0)
						m.vel = vt - vn;
				} else {
					// Previous position was outside -> particle crossed
					// surface boundary. Reverse normal component of velocity,
					// and apply friction (if Vt >= cutoff) and resilience.
					
					// Don't apply friction if tangential velocity < cutoff
					float appliedFriction = oneMinusFriction;
					if (cutoffSqr > 0) {
						float vtmag = vt.length2();
						
						if (vtmag <= cutoffSqr)
							appliedFriction = 1;
					}
					
					// Compute new velocity heading out:
					// V' = (1-mu) Vt - resilience Vn
					m.vel = vt * appliedFriction - vn * resilience;
				}
			}
		}
	}
}

// Set the secondary position of each particle to be its position.
void PACopyVertexB::Execute(ParticleGroup *group)
{
	int i;
	
	if(copy_pos)
		for (i = 0; i < group->p_count; i++)
		{
			Particle &m = group->list[i];
			m.posB = m.pos;
		}
		
	if(copy_vel)
		for (i = 0; i < group->p_count; i++)
		{
			Particle &m = group->list[i];
			m.velB = m.vel;
		}
}

// Dampen velocities
void PADamping::Execute(ParticleGroup *group)
{
	// This is important if dt is != 1.
	pVector one(1,1,1);
	pVector scale(one - ((one - damping) * dt));
	
	for (int i = 0; i < group->p_count; i++) {
		Particle &m = group->list[i];
		float vSqr = m.vel.length2();
		
		if (vSqr >= vlowSqr && vSqr <= vhighSqr) {
			m.vel.x *= scale.x;
			m.vel.y *= scale.y;
			m.vel.z *= scale.z;
		}
	}
}

// Exert force on each particle away from explosion center, iff
// shock wave is still going on at that particle's location.
void PAExplosion::Execute(ParticleGroup *group)
{
	if(age < 0) {
		// This special case computes center of mass of the
		// ParticleGroup to be the center of the explosion.
		
		center = pVector(0,0,0);
		for (int i = 0; i < group->p_count; i++) {
			Particle &m = group->list[i];
			center += m.pos;
		}
		
		center /= float(group->p_count);
		age = -age;
	}
	
	float mdt = magnitude * dt;
	float oneOverVelSqr = 1.0f / (velocity * velocity);
	float ageSqr = age * age;
	float agemlSqr = fsqr(age - lifetime);
	if(age - lifetime < 0)
		agemlSqr = 0;
	
	for (int i = 0; i < group->p_count; i++) {
		Particle &m = group->list[i];
		
		// Figure direction to particle.
		pVector dir(m.pos - center);
		float distSqr = dir.length2();
		
		// Time lag from start of explosion to when the
		// blast front reaches a particle dist units away.
		float delaySqr = distSqr * oneOverVelSqr;
		
		// See if the particle is in the propagating
		// shock wave of the explosion.
		if (delaySqr <= agemlSqr || delaySqr >= ageSqr)
			continue;
		
		// Force should really be more complicated than
		// a square wave, but this will do for now.
		m.vel += dir * (mdt / (sqrtf(distSqr) * (distSqr + epsilon)));
	}
	
	// Age the explosion. This is irrelevant for immediate mode.
	age += dt;
}

// Follow the next particle in the list
void PAFollow::Execute(ParticleGroup *group)
{
	float mdt = grav * dt;
	
	for (int i = 0; i < group->p_count - 1; i++) {
		Particle &m = group->list[i];
		
		// Accelerate toward the particle after me in the list.
		pVector vec01(group->list[i+1].pos - m.pos); // vec01 = p1 - p0
		float vec01lenSqr = vec01.length2();
		
		// Compute force exerted between the two bodies
		//m.vel += vec01 * (mdt / (sqrtf(vec01lenSqr) * (vec01lenSqr + epsilon)));
		m.vel += vec01 * (mdt / (sqrtf(vec01lenSqr) * (vec01lenSqr + epsilon)));
	}
}

// Inter-particle gravitation
// XXX Not fully tested.
void PAGravitate::Execute(ParticleGroup *group)
{
	float mdt = grav * dt;
	
	for (int i = 0; i < group->p_count; i++) {
		Particle &m = group->list[i];
		
		// Add interactions with other particles
		for (int j = i + 1; j < group->p_count; j++) {
			Particle &mj = group->list[j];
			
			pVector vec01(mj.pos - m.pos); // vec01 = p1 - p0
			float vec01lenSqr = vec01.length2();
			
			// Compute force exerted between the two bodies
			pVector acc(vec01 * (mdt / (sqrtf(vec01lenSqr) * (vec01lenSqr + epsilon))));
			
			m.vel += acc;
			mj.vel -= acc;
		}
	}
}

// Acceleration in a constant direction
void PAGravity::Execute(ParticleGroup *group)
{
	pVector ddir(direction * dt);
	
	for (int i = 0; i < group->p_count; i++) {
		// Step velocity with acceleration
		group->list[i].vel += ddir;
	}
}

// Accelerate particles along a line
void PAJet::Execute(ParticleGroup *group)
{
	float mdt = grav * dt;
	for (int i = 0; i < group->p_count; i++) {
		Particle &m = group->list[i];
		
		// Figure direction to particle.
		pVector dir(m.pos - center);
		
		// Distance to jet (force drops as 1/r^2)
		// Soften by epsilon to avoid tight encounters to infinity
		float rSqr = dir.length2();
		
		if(rSqr < maxRadiusSqr)
		{
			pVector accel;
			acc.Generate(accel);
			
			// Step velocity with acceleration
			m.vel += accel * (mdt / (rSqr + epsilon));
		}
	}
}

// Get rid of older particles
void PAKillOld::Execute(ParticleGroup *group)
{
	// Must traverse list in reverse order so Remove will work
	for (int i = group->p_count-1; i >= 0; i--) {
		Particle &m = group->list[i];
		
		if ((m.age < ageLimit) != kill_less_than)
			continue;
		group->Remove(i);
	}
}

// Get rid of slower particles
void PAKillSlow::Execute(ParticleGroup *group)
{
	// Must traverse list in reverse order so Remove will work
	for (int i = group->p_count-1; i >= 0; i--) {
		Particle &m = group->list[i];
		
		if ((m.vel.length2() < speedLimitSqr) == kill_less_than)
			group->Remove(i);
	}
}

void PAMove::Execute(ParticleGroup *group)
{
	// Step particle positions forward by dt, and age the particles.
	for (int i = 0; i < group->p_count; i++) {
		Particle &m = group->list[i];
		
		m.age += dt;
		m.pos += m.vel * dt;
	}
	
	// And finally, tick this particle group's clock.
	group->simTime += dt;
}

// Accelerate particles towards a line
void PAOrbitLine::Execute(ParticleGroup *group)
{
	float mdt = grav * dt;
	for (int i = 0; i < group->p_count; i++) {
		Particle &m = group->list[i];
		
		// Figure direction to particle from base of line.
		pVector f(m.pos - p);
		
		pVector w(axis * (f * axis));
		
		// Direction from particle to nearest point on line.
		pVector into = w - f;
		
		// Distance to line (force drops as 1/r^2, normalize by 1/r)
		// Soften by epsilon to avoid tight encounters to infinity
		float rSqr = into.length2();
		
		if(rSqr < maxRadiusSqr)
			// Step velocity with acceleration
			m.vel += into * (mdt / (sqrtf(rSqr) + (rSqr + epsilon)));
	}
}

// Accelerate particles towards a point
void PAOrbitPoint::Execute(ParticleGroup *group)
{
	float mdt = grav * dt;
	for (int i = 0; i < group->p_count; i++) {
		Particle &m = group->list[i];
		
		// Figure direction to particle.
		pVector dir(center - m.pos);
		
		// Distance to gravity well (force drops as 1/r^2, normalize by 1/r)
		// Soften by epsilon to avoid tight encounters to infinity
		float rSqr = dir.length2();
		
		if(rSqr < maxRadiusSqr)
			// Step velocity with acceleration
			m.vel += dir * (mdt / (sqrtf(rSqr) + (rSqr + epsilon)));
	}
}

// Accelerate in random direction each time step
void PARandomAccel::Execute(ParticleGroup *group)
{
	for (int i = 0; i < group->p_count; i++) {
		Particle &m = group->list[i];
		
		pVector acceleration;
		gen_acc.Generate(acceleration);
		
		// dt will affect this by making a higher probability of
		// being near the original velocity after unit time. Smaller
		// dt approach a normal distribution instead of a square wave.
		m.vel += acceleration * dt;
	}
}

// Immediately displace position randomly
void PARandomDisplace::Execute(ParticleGroup *group)
{
	for (int i = 0; i < group->p_count; i++) {
		Particle &m = group->list[i];
		
		pVector displacement;
		gen_disp.Generate(displacement);
		
		// dt will affect this by making a higher probability of
		// being near the original position after unit time. Smaller
		// dt approach a normal distribution instead of a square wave.
		m.pos += displacement * dt;
	}
}

// Immediately assign a random velocity
void PARandomVelocity::Execute(ParticleGroup *group)
{
	for (int i = 0; i < group->p_count; i++) {
		Particle &m = group->list[i];
		
		pVector velocity;
		gen_vel.Generate(velocity);
		
		// Shouldn't multiply by dt because velocities are
		// invariant of dt. How should dt affect this?
		m.vel = velocity;
	}
}

#if 0
// Produce coefficients of a velocity function v(t)=at^2 + bt + c
// satisfying initial x(0)=x0,v(0)=v0 and desired x(t)=xf,v(t)=vf,
// where x = x(0) + integrate(v(T),0,t)
static inline void _pconstrain(float x0, float v0, float xf, float vf,
							   float t, float *a, float *b, float *c)
{
	*c = v0;
	*b = 2 * ( -t*vf - 2*t*v0 + 3*xf - 3*x0) / (t * t);
	*a = 3 * ( t*vf + t*v0 - 2*xf + 2*x0) / (t * t * t);
}
#endif

// Over time, restore particles to initial positions
// Put all particles on the surface of a statue, explode the statue,
// and then suck the particles back to the original position. Cool!
void PARestore::Execute(ParticleGroup *group)
{
	if (timeLeft <= 0) {
		for (int i = 0; i < group->p_count; i++) {
			Particle &m = group->list[i];
			
			// Already constrained, keep it there.
			m.pos = m.posB;
			m.vel = pVector(0,0,0);
		}
	} else {
		float t = timeLeft;
		float dtSqr = dt * dt;
		float tSqrInv2dt = dt * 2.0f / (t * t);
		float tCubInv3dtSqr = dtSqr * 3.0f / (t * t * t);

		for (int i = 0; i < group->p_count; i++) {
#if 1
			Particle &m = group->list[i];
			
			// Solve for a desired-behavior velocity function in each axis
			// _pconstrain(m.pos.x, m.vel.x, m.posB.x, 0., timeLeft, &a, &b, &c);
			
			// Figure new velocity at next timestep
			// m.vel.x = a * dtSqr + b * dt + c;

			float b = (-2*t*m.vel.x + 3*m.posB.x - 3*m.pos.x) * tSqrInv2dt;
			float a = (t*m.vel.x - m.posB.x - m.posB.x + m.pos.x + m.pos.x) * tCubInv3dtSqr;
			
			// Figure new velocity at next timestep
			m.vel.x += a + b;

			b = (-2*t*m.vel.y + 3*m.posB.y - 3*m.pos.y) * tSqrInv2dt;
			a = (t*m.vel.y - m.posB.y - m.posB.y + m.pos.y + m.pos.y) * tCubInv3dtSqr;
			
			// Figure new velocity at next timestep
			m.vel.y += a + b;

			b = (-2*t*m.vel.z + 3*m.posB.z - 3*m.pos.z) * tSqrInv2dt;
			a = (t*m.vel.z - m.posB.z - m.posB.z + m.pos.z + m.pos.z) * tCubInv3dtSqr;
			
			// Figure new velocity at next timestep
			m.vel.z += a + b;
#else
			Particle &m = group->list[i];
			
			// XXX Optimize this.
			// Solve for a desired-behavior velocity function in each axis
			float a, b, c; // Coefficients of velocity function needed
			
			_pconstrain(m.pos.x, m.vel.x, m.posB.x, 0.,
				timeLeft, &a, &b, &c);
			
			// Figure new velocity at next timestep
			m.vel.x = a * dtSqr + b * dt + c;
			
			_pconstrain(m.pos.y, m.vel.y, m.posB.y, 0.,
				timeLeft, &a, &b, &c);
			
			// Figure new velocity at next timestep
			m.vel.y = a * dtSqr + b * dt + c;
			
			_pconstrain(m.pos.z, m.vel.z, m.posB.z, 0.,
				timeLeft, &a, &b, &c);
			
			// Figure new velocity at next timestep
			m.vel.z = a * dtSqr + b * dt + c;

#endif
		}
	}

	timeLeft -= dt;
}

// Kill particles on wrong side of sphere
void PASink::Execute(ParticleGroup *group)
{
	// Must traverse list in reverse order so Remove will work
	for (int i = group->p_count-1; i >= 0; i--) {
		Particle &m = group->list[i];
		
		// Remove if inside/outside flag matches object's flag
		if (position.Within(m.pos) == kill_inside) {
			group->Remove(i);
		}
	}
}

// Kill particles on wrong side of sphere
void PASinkVelocity::Execute(ParticleGroup *group)
{
	// Must traverse list in reverse order so Remove will work
	for (int i = group->p_count-1; i >= 0; i--) {
		Particle &m = group->list[i];
		
		// Remove if inside/outside flag matches object's flag
		if (velocity.Within(m.vel) == kill_inside) {
			group->Remove(i);
		}
	}
}

// Randomly add particles to the system
void PASource::Execute(ParticleGroup *group)
{
	int rate = int(floor(particleRate * dt));
	
	// Dither the fraction particle in time.
	if(drand48() < particleRate * dt - float(rate))
		rate++;
	
	// Don't emit more than it can hold.
	if(group->p_count + rate > group->max_particles)
		rate = group->max_particles - group->p_count;
	
	pVector pos, posB, vel, col;
	
	if(vertexB_tracks)
	{
		for (int i = 0; i < rate; i++)
		{
			position.Generate(pos);
			velocity.Generate(vel);
			color.Generate(col);
			
			float size;
			if(size1 == size2)
				size = size1;
			else
				size = drand48() * (size2 - size1) + size1;
			group->Add(pos, pos, vel, col, size, alpha, age);
		}
	}
	else
	{
		for (int i = 0; i < rate; i++)
		{
			position.Generate(pos);
			positionB.Generate(posB);
			velocity.Generate(vel);
			color.Generate(col);
			
			float size;
			if(size1 == size2)
				size = size1;
			else
				size = drand48() * (size2 - size1) + size1;
			group->Add(pos, posB, vel, col, size, alpha, age);
		}
	}
}

// Shift color towards a specified value
void PATargetColor::Execute(ParticleGroup *group)
{
	float scaleFac = scale * dt;
	
	for (int i = 0; i < group->p_count; i++) {
		Particle &m = group->list[i];
		m.color += (color - m.color) * scaleFac;
		m.alpha += (alpha - m.alpha) * scaleFac;
	}
}

// Shift size towards a specified value
void PATargetSize::Execute(ParticleGroup *group)
{
	float scaleFac = scale * dt;
	
	for (int i = 0; i < group->p_count; i++) {
		Particle &m = group->list[i];
		m.size += (destSize - m.size) * scaleFac;
	}
}

// Immediately displace position using vortex
// Vortex tip at center, around axis, with magnitude
// and tightness exponent
void PAVortex::Execute(ParticleGroup *group)
{
	float magdt = magnitude * dt;
	
	for (int i = 0; i < group->p_count; i++) {
		Particle &m = group->list[i];
		
		// Vector from tip of vortex
		pVector offset(m.pos - center);
		
		// Compute distance from particle to tip of vortex.
		float r = offset.length();
		
		// Don't do anything to particle if too close or too far.
		if (r < P_EPS || r > maxRadius)
			continue;
		
		// Compute normalized offset vector.
		pVector offnorm(offset / r);
		
		// Construct orthogonal vector frame in which to rotate
		// transformed point around origin
		float axisProj = offnorm * axis; // offnorm . axis
		
		// Components of offset perpendicular and parallel to axis
		pVector w(axis * axisProj); // parallel component
		pVector u(offnorm - w); // perpendicular component
		
		// Perpendicular component completing frame:
		pVector v(axis ^ u);
		
		// Figure amount of rotation
		// Resultant is (cos theta) u + (sin theta) v
		float theta = magdt / powf(r, tightness);
		float s = sinf(theta);
		float c = cosf(theta);
		
		offset = (u * c + v * s + w) * r;
		
		// Translate back to object space
		m.pos = offset + center;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Stuff for the pDomain.

// To offset [0 .. 1] vectors to [-.5 .. .5]
static pVector vHalf(0.5, 0.5, 0.5);

static inline pVector RandVec()
{
	return pVector(drand48(), drand48(), drand48());
}

// Return a random number with a normal distribution.
static inline float NRand(float sigma = 1.0f)
{
#define ONE_OVER_SIGMA_EXP (1.0f / 0.7975f)
	
	float y;
	do
	{
		y = -logf(drand48());
	}
	while(drand48() > expf(-fsqr(y - 1.0f)*0.5f));
	
	if(rand() & 0x1)
		return y * sigma * ONE_OVER_SIGMA_EXP;
	else
		return -y * sigma * ONE_OVER_SIGMA_EXP;
}

pDomain::pDomain(PDomainEnum dtype, float a0, float a1,
				 float a2, float a3, float a4, float a5,
				 float a6, float a7, float a8)
{
	type = dtype;
	switch(type)
	{
	case PDPoint:
		p1 = pVector(a0, a1, a2);
		break;
	case PDLine:
		{
			p1 = pVector(a0, a1, a2);
			pVector tmp(a3, a4, a5);
			// p2 is vector from p1 to other endpoint.
			p2 = tmp - p1;
		}
		break;
	case PDBox:
		// p1 is the min corner. p2 is the max corner.
		if(a0 < a3) {
			p1.x = a0; p2.x = a3;
		} else {
			p1.x = a3; p2.x = a0;
		}
		if(a1 < a4) {
			p1.y = a1; p2.y = a4;
		} else {
			p1.y = a4; p2.y = a1;
		}
		if(a2 < a5) {
			p1.z = a2; p2.z = a5;
		} else {
			p1.z = a5; p2.z = a2;
		}
		break;
	case PDTriangle:
		{
			p1 = pVector(a0, a1, a2);
			pVector tp2 = pVector(a3, a4, a5);
			pVector tp3 = pVector(a6, a7, a8);
			
			u = tp2 - p1;
			v = tp3 - p1;

			// The rest of this is needed for bouncing.
			radius1Sqr = u.length();
			pVector tu = u / radius1Sqr;
			radius2Sqr = v.length();
			pVector tv = v / radius2Sqr;

			p2 = tu ^ tv; // This is the non-unit normal.
			p2.normalize(); // Must normalize it.

			// radius1 stores the d of the plane eqn.
			radius1 = -(p1 * p2);
		}
		break;
	case PDPlane:
		{
			p1 = pVector(a0, a1, a2);
			u = pVector(a3, a4, a5);
			v = pVector(a6, a7, a8);

			// The rest of this is needed for bouncing.
			radius1Sqr = u.length();
			pVector tu = u / radius1Sqr;
			radius2Sqr = v.length();
			pVector tv = v / radius2Sqr;

			p2 = tu ^ tv; // This is the non-unit normal.
			p2.normalize(); // Must normalize it.

			// radius1 stores the d of the plane eqn.
			radius1 = -(p1 * p2);
		}
		break;
	case PDSphere:
		p1 = pVector(a0, a1, a2);
		if(a3 > a4) {
			radius1 = a3; radius2 = a4;
		} else {
			radius1 = a4; radius2 = a3;
		}
		radius1Sqr = radius1 * radius1;
		radius2Sqr = radius2 * radius2;
		break;
	case PDCone:
	case PDCylinder:
		{
			// p2 is a vector from p1 to the other end of cylinder.
			// p1 is apex of cone.
			
			p1 = pVector(a0, a1, a2);
			pVector tmp(a3, a4, a5);
			p2 = tmp - p1;
			
			if(a6 > a7) {
				radius1 = a6; radius2 = a7;
			} else {
				radius1 = a7; radius2 = a6;
			}
			radius1Sqr = radius1 * radius1;
			
			// Given an arbitrary nonzero vector n, form two orthonormal
			// vectors u and v forming a frame [u,v,n.normalize()].
			pVector n = p2;
			float p2l2 = n.length2(); // Optimize this.
			n.normalize();
			
			// radius2Sqr stores 1 / (p2.p2)
			if(p2l2 != 0.0f)
				radius2Sqr = 1.0f / p2l2;
			else
				radius2Sqr = 0.0f;
			
			// Find a vector with an orthogonal component to n.
			pVector basis(1.0f, 0.0f, 0.0f);
			if (1.0f - fabs(basis * n) < 1e-5f)
				basis = pVector(0.0f, 1.0f, 0.0f);
			
			// Project away N component, normalize and cross to get
			// second orthonormal vector.
			u = basis - n * (basis * n);
			u.normalize();
			v = n ^ u;
		}
		break;
	case PDBlob:
		p1 = pVector(a0, a1, a2);
		radius1 = a3;
		radius1Sqr = fsqr(radius1);
		radius2Sqr = -0.5f/radius1Sqr;
		radius2 = 1.0f * sqrtf(2.0f * float(M_PI) * radius1Sqr);
		break;
	}
}

// Determines if 'pos' is inside the box domain d
bool pDomain::Within(const pVector &pos) const
{
	switch (type) {
	case PDPoint:
		return false; // Even less likely!
	case PDLine:
		return false; // It's very hard to lie on a line.
	case PDBox:
		if ((pos.x < p1.x) || (pos.x > p2.x) ||
			(pos.y < p1.y) || (pos.y > p2.y) ||
			(pos.z < p1.z) || (pos.z > p2.z))
			return false; // outside
		
		return true; // inside
	case PDTriangle:
		return false; // XXX Is there something better?
	case PDPlane:
		// Distance from plane = n * p + d
		// Inside is the positive half-space.
		return pos * p2 >= -radius1;
	case PDSphere:
		{
			pVector rvec(pos - p1);
			float rSqr = rvec.length2();
			return rSqr <= radius1Sqr && rSqr >= radius2Sqr;
		}
	case PDCylinder:
	case PDCone:
		{
			// This is painful and slow. Might be better to do quick
			// accept/reject tests.
			// Let p2 = vector from base to tip of the cylinder
			// x = vector from base to test point
			//        x . p2
			// dist = ------ = projected distance of x along the axis
			//        p2. p2   ranging from 0 (base) to 1 (tip)
			//
			// rad = x - dist * p2 = projected vector of x along the base
			// p1 is the apex of the cone.
			
			pVector x(pos - p1);
			
			// Check axial distance
			// radius2Sqr stores 1 / (p2.p2)
			float dist = (p2 * x) * radius2Sqr;
			if (dist < 0.0f || dist > 1.0f)
				return false;
			
			// Check radial distance; scale radius along axis for cones
			pVector xrad = x - p2 * dist; // Radial component of x
			float rSqr = xrad.length2();
			
			if (type == PDCone)
				return (rSqr <= fsqr(dist * radius1) &&
				rSqr >= fsqr(dist * radius2));
			else
				return (rSqr <= radius1Sqr && rSqr >= fsqr(radius2));
		}
	case PDBlob:
		{
			pVector x(pos - p1);
			float Gx = expf(x.length2() * radius2Sqr) * radius2;
			return (drand48() < Gx);
		}
	default:
		return false;
	}
}

// Generate a random point uniformly distrbuted within the domain
void pDomain::Generate(pVector &pos) const
{
	switch (type)
	{
	case PDPoint:
		pos = p1;
		break;
	case PDLine:
		pos = p1 + p2 * drand48();
		break;
	case PDBox:
		// Scale and translate [0,1] random to fit box
		pos.x = p1.x + (p2.x - p1.x) * drand48();
		pos.y = p1.y + (p2.y - p1.y) * drand48();
		pos.z = p1.z + (p2.z - p1.z) * drand48();
		break;
	case PDTriangle:
		{
			float r1 = drand48();
			float r2 = drand48();
			if(r1 + r2 < 1.0f)
				pos = p1 + u * r1 + v * r2;
			else
				pos = p1 + u * (1.0f-r1) + v * (1.0f-r2);
		}
		break;
	case PDPlane:
		pos = p1 + u * drand48() + v * drand48();
		break;
	case PDSphere:
		// Place on [-1..1] sphere
		pos = RandVec() - vHalf;
		pos.normalize();
		
		// Scale unit sphere pos by [0..r] and translate
		// (should distribute as r^2 law)
		if(radius1 == radius2)
			pos = p1 + pos * radius1;
		else
			pos = p1 + pos * (radius2 + drand48() * (radius1 - radius2));
		break;
	case PDCylinder:
	case PDCone:
		{
			// For a cone, p2 is the apex of the cone.
			float dist = drand48(); // Distance between base and tip
			float theta = drand48() * 2.0f * float(M_PI); // Angle around base sphere
			// Distance from axis
			float r = radius2 + drand48() * (radius1 - radius2);
			
			float x = r * cosf(theta); // Weighting of each frame vector
			float y = r * sinf(theta);
			
			// Scale radius along axis for cones
			if (type == PDCone) {
				x *= dist;
				y *= dist;
			}
			
			pos = p1 + p2 * dist + u * x + v * y;
		}
		break;
	case PDBlob:
		pos.x = p1.x + NRand(radius1);
		pos.y = p1.y + NRand(radius1);
		pos.z = p1.z + NRand(radius1);
		
		break;
	default:
		pos = pVector(0,0,0);
	}
}
