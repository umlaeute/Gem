// papi.h
//
// Copyright 1997-1998 by David K. McAllister
// http://www.cs.unc.edu/~davemc/Particle
//
// Adapted for GEM by Guenter Geiger <geiger@xdv.org>
//
// Include this file in all applications that use the Particle System API.


#ifndef _particle_h
#define _particle_h

#include <stdlib.h>
#include "part_vector.h"

#ifdef MACOSX
#include "macosx_math.h"
#endif

// This is the major and minor version number of this release of the API.
#define P_VERSION 111

// Actually this must be < sqrt(MAXFLOAT) since we store this value squared.
#define P_MAXFLOAT 1.0e16f

#ifdef MAXINT
#define P_MAXINT MAXINT
#else
#define P_MAXINT 0x7fffffff
#endif

#define P_EPS 1e-3f

//////////////////////////////////////////////////////////////////////
// Type codes for domains
enum PDomainEnum
{
	PDPoint = 0, // Single point
	PDLine = 1, // Line segment
	PDTriangle = 2, // Triangle
	PDPlane = 3, // Arbitrarily-oriented plane
	PDBox = 4, // Axis-aligned box
	PDSphere = 5, // Sphere
	PDCylinder = 6, // Cylinder
	PDCone = 7, // Cone
	PDBlob = 8 // Gaussian blob
};

// State setting calls

void pColor(float red, float green, float blue, float alpha = 1.0f);

void pColorD(float alpha, PDomainEnum dtype,
			 float a0=0.0f, float a1=0.0f, float a2=0.0f,
			 float a3=0.0f, float a4=0.0f, float a5=0.0f,
			 float a6=0.0f, float a7=0.0f, float a8=0.0f);

void pSize(float s1, float s2 = -1.0);

void pStartingAge(float age);

void pTimeStep(float newDT);

void pVelocity(float x, float y, float z);

void pVelocityD(PDomainEnum dtype,
				float a0=0.0f, float a1=0.0f, float a2=0.0f,
				float a3=0.0f, float a4=0.0f, float a5=0.0f,
				float a6=0.0f, float a7=0.0f, float a8=0.0f);

void pVertexB(float x, float y, float z);

void pVertexBD(PDomainEnum dtype,
			   float a0=0.0f, float a1=0.0f, float a2=0.0f,
			   float a3=0.0f, float a4=0.0f, float a5=0.0f,
			   float a6=0.0f, float a7=0.0f, float a8=0.0f);

void pVertexBTracks(bool trackVertex = true);


// Action List Calls

void pCallActionList(int action_list_num);

void pDeleteActionLists(int action_list_num, int action_list_count = 1);

void pEndActionList();

int pGenActionLists(int action_list_count = 1);

void pNewActionList(int action_list_num);


// Particle Group Calls

void pCopyGroup(int p_group_num, int index = 0, int copy_count = P_MAXINT);

void pCurrentGroup(int p_group_num);

void pDeleteParticleGroups(int p_group_num, int p_group_count = 1);

void pDrawGroupl(int dlist, bool const_size = false,
				 bool const_color = false, bool const_rotation = false);

void pDrawGroupp(int primitive, bool const_size = false,
				 bool const_color = false);

int pGenParticleGroups(int p_group_count = 1, int max_particles = 0);

int pGetGroupCount();

void pGetParticles(int index, int count, float *verts, float *color = NULL,
				   float *vel = NULL, float *size = NULL);

int pSetMaxParticles(int max_count);


// Actions

void pBounce(float friction, float resilience, float cutoff,
			 PDomainEnum dtype,
			 float a0=0.0f, float a1=0.0f, float a2=0.0f,
			 float a3=0.0f, float a4=0.0f, float a5=0.0f,
			 float a6=0.0f, float a7=0.0f, float a8=0.0f);

void pCopyVertexB(bool copy_pos = true, bool copy_vel = false);

void pDamping(float damping_x, float damping_y, float damping_z,
			  float vlow = 0.0f, float vhigh = P_MAXFLOAT);

void pExplosion(float center_x, float center_y, float center_z,
				float velocity, float magnitude, float lifetime,
				float epsilon = P_EPS, float age = 0.0f);

void pFollow(float grav = 1.0f, float epsilon = P_EPS);

void pGravitate(float grav = 1.0f, float epsilon = P_EPS);

void pGravity(float dir_x, float dir_y, float dir_z);

void pGrowSize(float destSize, float scale);

void pJet(float center_x, float center_y, float center_z, float grav = 1.0f,
		  float epsilon = P_EPS, float maxRadius = P_MAXFLOAT);

void pKillOld(float ageLimit, bool kill_less_than = false);

void pKillSlow(float speedLimit, bool kill_less_than = true);

void pMove();

void pOrbitLine(float p_x, float p_y, float p_z,
				float axis_x, float axis_y, float axis_z, float grav = 1.0f,
				float epsilon = P_EPS, float maxRadius = P_MAXFLOAT);

void pOrbitPoint(float center_x, float center_y, float center_z,
				 float grav = 1.0f, float epsilon = P_EPS,
				 float maxRadius = P_MAXFLOAT);

void pRandomAccel(PDomainEnum dtype,
				  float a0=0.0f, float a1=0.0f, float a2=0.0f,
				  float a3=0.0f, float a4=0.0f, float a5=0.0f,
				  float a6=0.0f, float a7=0.0f, float a8=0.0f);

void pRandomDisplace(PDomainEnum dtype,
					 float a0=0.0f, float a1=0.0f, float a2=0.0f,
					 float a3=0.0f, float a4=0.0f, float a5=0.0f,
					 float a6=0.0f, float a7=0.0f, float a8=0.0f);

void pRandomVelocity(PDomainEnum dtype,
					 float a0=0.0f, float a1=0.0f, float a2=0.0f,
					 float a3=0.0f, float a4=0.0f, float a5=0.0f,
					 float a6=0.0f, float a7=0.0f, float a8=0.0f);

void pRestore(float time);

void pShade(float color_x, float color_y, float color_z,
			float alpha, float scale);

void pSink(bool kill_inside, PDomainEnum dtype,
		   float a0=0.0f, float a1=0.0f, float a2=0.0f,
		   float a3=0.0f, float a4=0.0f, float a5=0.0f,
		   float a6=0.0f, float a7=0.0f, float a8=0.0f);

void pSinkVelocity(bool kill_inside, PDomainEnum dtype,
				   float a0=0.0f, float a1=0.0f, float a2=0.0f,
				   float a3=0.0f, float a4=0.0f, float a5=0.0f,
				   float a6=0.0f, float a7=0.0f, float a8=0.0f);

void pSource(float particleRate, PDomainEnum dtype,
			 float a0=0.0f, float a1=0.0f, float a2=0.0f,
			 float a3=0.0f, float a4=0.0f, float a5=0.0f,
			 float a6=0.0f, float a7=0.0f, float a8=0.0f);

void pTargetColor(float color_x, float color_y, float color_z,
				  float alpha, float scale);

void pTargetSize(float destSize, float scale);

void pVertex(float x, float y, float z);

void pVortex(float center_x, float center_y, float center_z,
			 float axis_x, float axis_y, float axis_z,
			 float magnitude, float tightness=1.0f, float maxRadius = P_MAXFLOAT);



// A single particle
struct Particle
{
	pVector pos;
	pVector posB;
	pVector vel;
	pVector velB;	// Used to compute binormal, normal, etc.
	pVector color;	// Color must be next to alpha so glColor4fv works.
	float alpha;	// This is both cunning and scary.
	float size;
	float age;
};

// A group of particles - Info and an array of Particles
struct ParticleGroup
{
	int p_count; // Number of particles currently existing.
	int max_particles; // Max particles allowed in group.
	int particles_allocated; // Actual allocated size.
	float simTime; // Current time. Not really used anymore.
	Particle list[1];	// Actually, num_particles in size
	
	inline void Remove(int i)
	{
		list[i] = list[--p_count];
	}
	
	inline bool Add(const pVector &pos, const pVector &posB,
		const pVector &vel, const pVector &color,
		const float size = 1.0f, const float alpha = 1.0f,
		const float age = 0.0f)
	{
		if(p_count >= max_particles)
			return false;
		else
		{
			list[p_count].pos = pos;
			list[p_count].posB = posB;
			list[p_count].vel = vel;
			list[p_count].velB = vel;	// XXX This should be fixed.
			list[p_count].color = color;
			list[p_count].alpha = alpha;
			list[p_count].size = size;
			list[p_count].age = age;
			p_count++;
			return true;
		}
	}
};

struct pDomain
{
	PDomainEnum type;	// PABoxDomain, PASphereDomain, PAConeDomain...
	pVector p1, p2;		// Box vertices, Sphere center, Cylinder/Cone ends
	pVector u, v;		// Orthonormal basis vectors for Cylinder/Cone
	float radius1;		// Sphere/Cylinder/Cone outer radius
	float radius2;		// Sphere/Cylinder/Cone inner radius
	float radius1Sqr;	// Used for fast Within test of spheres,
	float radius2Sqr;	// and for mag. of u and v vectors for plane (not needed).
	
	bool Within(const pVector &) const;
	void Generate(pVector &) const;
	
	// This constructor is used when default constructing a
	// ParticleAction that has a pDomain.
	inline pDomain()
	{
	}
	
	// Construct a domain in the standard way.
	pDomain(PDomainEnum dtype,
		float a0=0.0f, float a1=0.0f, float a2=0.0f,
		float a3=0.0f, float a4=0.0f, float a5=0.0f,
		float a6=0.0f, float a7=0.0f, float a8=0.0f);
};

//////////////////////////////////////////////////////////////////////
// Type codes for all actions
enum PActionEnum
{
	PAHeaderID,			// The first action in each list,
	PABounceID,			// Bounce particles off a domain of space.
	PACopyVertexBID,	// Set the secondary position from current position.
	PADampingID,		// Dampen particle velocities.
	PAExplosionID,		// An Explosion.
	PAFollowID,			// Accelerate toward the previous particle in the group.
	PAGravitateID,		// Accelerate each particle toward each other particle.
	PAGravityID,		// Acceleration in the given direction.
	PAJetID,			// 
	PAKillOldID,		// 
	PAKillSlowID,		// 
	PAMoveID,			// 
	PAOrbitLineID,		// 
	PAOrbitPointID,		// 
	PARandomAccelID,	// 
	PARandomDisplaceID,	// 
	PARandomVelocityID,	// 
	PARestoreID,		// 
	PASinkID,			// 
	PASinkVelocityID,	// 
	PASourceID,			// 
	PATargetColorID,	// 
	PATargetSizeID,		// 
	PAVortexID			// 
};

// This method actually does the particle's action.
#define ExecMethod	void Execute(ParticleGroup *pg);

struct ParticleAction
{
	static float dt;	// This is copied to here from global state.
	PActionEnum type;	// Type field
};

///////////////////////////////////////////////////////////////////////////
// Data types derived from Action.


struct PAHeader : public ParticleAction
{
	int actions_allocated;
	int count;			// Total actions in the list.
	float padding[82];	// This must be the largest action.
	
	ExecMethod
};

struct PABounce : public ParticleAction
{
	pDomain position;	// Bounce region (should be plane or sphere)
	float oneMinusFriction;	// Friction tangent to surface
	float resilience;	// Resilence perpendicular to surface
	float cutoffSqr;	// cutoff velocity; friction applies iff v > cutoff
	
	ExecMethod
};

struct PACopyVertexB : public ParticleAction
{
	bool copy_pos;		// True to copy pos to posB.
	bool copy_vel;		// True to copy vel to velB.

	ExecMethod
};

struct PADamping : public ParticleAction
{
	pVector damping;	// Damping constant applied to velocity
	float vlowSqr;		// Low and high cutoff velocities
	float vhighSqr;
	
	ExecMethod
};

struct PAExplosion : public ParticleAction
{
	pVector center;		// The center of the explosion
	float velocity;		// Of shock wave
	float magnitude;	// At unit radius
	float lifetime;		// Thickness of shock wave
	float age;			// How long it's been going on
	float epsilon;		// Softening parameter
	
	ExecMethod
};

struct PAFollow : public ParticleAction
{
	float grav;			// The grav of each particle
	float epsilon;		// Softening parameter
	
	ExecMethod
};

struct PAGravitate : public ParticleAction
{
	float grav;			// The grav of each particle
	float epsilon;		// Softening parameter
	
	ExecMethod
};

struct PAGravity : public ParticleAction
{
	pVector direction;	// Amount to increment velocity
	
	ExecMethod
};

struct PAJet : public ParticleAction
{
	pVector	center;		// Center of the fan
	pDomain acc;		// Acceleration vector domain
	float grav;			// Scales acceleration by (m/(r+epsilon)^2)
	float epsilon;		// Softening parameter
	float maxRadiusSqr;	// Only influence particles within maxRadius

	ExecMethod
};

struct PAKillOld : public ParticleAction
{
	float ageLimit;		// Age at which to kill particles
	bool kill_less_than;	// True to kill particles less than limit.

	ExecMethod
};

struct PAKillSlow : public ParticleAction
{
	float speedLimitSqr;	// Speed at which to kill particles
	bool kill_less_than;	// True to kill particles slower than limit.

	ExecMethod
};

struct PAMove : public ParticleAction
{
	
	ExecMethod
};

struct PAOrbitLine : public ParticleAction
{
	pVector p, axis;	// Endpoints of line to which particles are attracted
	float grav;			// Scales acceleration by (m/(r+epsilon)^2)
	float epsilon;		// Softening parameter
	float maxRadiusSqr;	// Only influence particles within maxRadius
	
	ExecMethod
};

struct PAOrbitPoint : public ParticleAction
{
	pVector center;		// Point to which particles are attracted
	float grav;			// Scales acceleration by (m/(r+epsilon)^2)
	float epsilon;		// Softening parameter
	float maxRadiusSqr;	// Only influence particles within maxRadius
	
	ExecMethod
};

struct PARandomAccel : public ParticleAction
{
	pDomain gen_acc;	// The domain of random accelerations.
	
	ExecMethod
};

struct PARandomDisplace : public ParticleAction
{
	pDomain gen_disp;	// The domain of random displacements.
	
	ExecMethod
};

struct PARandomVelocity : public ParticleAction
{
	pDomain gen_vel;	// The domain of random velocities.
	
	ExecMethod
};

struct PARestore : public ParticleAction
{
	float timeLeft;		// Time remaining until they should be in position.
	
	ExecMethod
};

struct PASink : public ParticleAction
{
	bool kill_inside;	// True to dispose of particles *inside* domain
	pDomain position;	// Disposal region
	
	ExecMethod
};

struct PASinkVelocity : public ParticleAction
{
	bool kill_inside;	// True to dispose of particles with vel *inside* domain
	pDomain velocity;	// Disposal region
	
	ExecMethod
};

struct PASource : public ParticleAction
{
	pDomain position;	// Choose a position in this domain.
	pDomain positionB;	// Choose a positionB in this domain.
	pDomain velocity;	// Choose a velocity in this domain.
	pDomain color;		// Choose a color in this domain.
	float particleRate;	// Particles to generate per unit time
	float size1;		// Max size of particle
	float size2;		// Min size of particle
	float alpha;		// Alpha of all generated particles
	float age;			// Initial age of the particles
	bool vertexB_tracks;	// true to get positionB from position.

	ExecMethod
};

struct PATargetColor : public ParticleAction
{
	pVector color;		// Color to shift towards
	float alpha;		// Alpha value to shift towards
	float scale;		// Amount to shift by (1 == all the way)
	
	ExecMethod
};

struct PATargetSize : public ParticleAction
{
	float destSize;		// Size to shift towards
	float scale;		// Amount to shift by per frame (1 == all the way)
	
	ExecMethod
};

struct PAVortex : public ParticleAction {
	pVector center;		// Center of vortex
	pVector axis;		// Axis around which vortex is applied
	float magnitude;	// Rotation around axis scales as mag/r^tightness
	float tightness;
	float maxRadius;	// Only influence particles within maxRadius
	
	ExecMethod
};

// Global state vector
struct _ParticleState
{
	float dt;
	bool in_call_list;
	bool in_new_list;
	bool vertexB_tracks;
	
	int group_id;
	int list_id;
	ParticleGroup *pgrp;
	PAHeader *pact;
	
	ParticleGroup **group_list;
	PAHeader **alist_list;
	int group_count;
	int alist_count;

	pDomain Vel;
	pDomain VertexB;
	pDomain Color;
	float Alpha;
	float Size1;
	float Size2;
	float Age;

	_ParticleState();

	// Return an index into the list of particle groups where
	// p_group_count groups can be added.
	int GenerateGroups(int p_group_count);
	int GenerateLists(int alist_count);
	ParticleGroup *GetGroupPtr(int p_group_num);
	PAHeader *GetListPtr(int action_list_num);
};

// Just a silly little function.
static inline float fsqr(float f) { return f * f; }

#endif
