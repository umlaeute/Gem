// system.cpp
//
// Copyright 1998 by David K. McAllister.
//
// This file implements the API calls that are not particle actions.


#ifdef MACOSX
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "papi.h"

#include <memory.h>

float ParticleAction::dt;

_ParticleState _ps;

_ParticleState::_ParticleState()
{
	in_call_list = false;
	in_new_list = false;
	vertexB_tracks = true;
	
	dt = 1.0f;
	
	group_id = -1;
	list_id = -1;
	pgrp = NULL;
	pact = NULL;
	
	Vel = pDomain(PDPoint, 0.0f, 0.0f, 0.0f);
	VertexB = pDomain(PDPoint, 0.0f, 0.0f, 0.0f);
	Color = pDomain(PDPoint, 1.0f, 1.0f, 1.0f);
	Alpha = 1.0f;
	Size1 = 1.0f;
	Size2 = 1.0f;
	Age = 0.0f;
	
	// The list of groups, etc.		
	group_list = new ParticleGroup *[16];
	group_count = 16;
	alist_list = new PAHeader *[16];
	alist_count = 16;
	for(int i=0; i<16; i++)
	{
		group_list[i] = NULL;
		alist_list[i] = NULL;
	}
}

ParticleGroup *_ParticleState::GetGroupPtr(int p_group_num)
{
	if(p_group_num < 0)
		return NULL; // IERROR

	if(p_group_num >= group_count)
		return NULL; // IERROR

	return group_list[p_group_num];
}

PAHeader *_ParticleState::GetListPtr(int a_list_num)
{
	if(a_list_num < 0)
		return NULL; // IERROR

	if(a_list_num >= alist_count)
		return NULL; // IERROR

	return alist_list[a_list_num];
}

// Return an index into the list of particle groups where
// p_group_count groups can be added.
int _ParticleState::GenerateGroups(int p_group_count)
{
	int num_empty = 0;
	int first_empty = -1;
	int i;
	for(i=0; i<group_count; i++)
	{
		if(group_list[i])
		{
			num_empty = 0;
			first_empty = -1;
		}
		else
		{
			if(first_empty < 0)
				first_empty = i;
			num_empty++;
			if(num_empty >= p_group_count)
				return first_empty;
		}
	}
	
	// Couldn't find a big enough gap. Reallocate.
	int new_count = 16 + group_count + p_group_count;
	ParticleGroup **glist = new ParticleGroup *[new_count];
	memcpy(glist, group_list, group_count * sizeof(void*));
	for(i=group_count; i<new_count; i++)
		glist[i] = NULL;
	delete [] group_list;
	group_list = glist;
	group_count = new_count;
	
	return GenerateGroups(p_group_count);
}

// Return an index into the list of action lists where
// list_count lists can be added.
int _ParticleState::GenerateLists(int list_count)
{
	int num_empty = 0;
	int first_empty = -1;
	int i;
	for(i=0; i<alist_count; i++)
	{
		if(alist_list[i])
		{
			num_empty = 0;
			first_empty = -1;
		}
		else
		{
			if(first_empty < 0)
				first_empty = i;
			num_empty++;
			if(num_empty >= list_count)
				return first_empty;
		}
	}
	
	// Couldn't find a big enough gap. Reallocate.
	int new_count = 16 + alist_count + list_count;
	PAHeader **new_list = new PAHeader *[new_count];
	memcpy(new_list, alist_list, alist_count * sizeof(void*));
	for(i=list_count; i<new_count; i++)
		new_list[i] = NULL;
	delete [] alist_list;
	alist_list = new_list;
	alist_count = new_count;
	
	return GenerateLists(list_count);
}

////////////////////////////////////////////////////////
// Auxiliary calls
void _pCallActionList(ParticleAction *apa, int num_actions,
					  ParticleGroup *pg)
{
	// All these require a particle group, so check for it.
	if(pg == NULL)
		return;
	
	PAHeader *pa = (PAHeader *)apa;
	
	// Step through all the actions in the action list.
	for(int action = 0; action < num_actions; action++, pa++)
	{
		//if(_ps.simTime < pa->tlow || _ps.simTime > pa->thigh)
		//	continue;
		
		switch(pa->type)
		{
		case PABounceID:
			((PABounce *)pa)->Execute(pg);
			break;
		case PACopyVertexBID:
			((PACopyVertexB *)pa)->Execute(pg);
			break;
		case PADampingID:
			((PADamping *)pa)->Execute(pg);
			break;
		case PAExplosionID:
			((PAExplosion *)pa)->Execute(pg);
			break;
		case PAFollowID:
			((PAFollow *)pa)->Execute(pg);
			break;
		case PAGravitateID:
			((PAGravitate *)pa)->Execute(pg);
			break;
		case PAGravityID:
			((PAGravity *)pa)->Execute(pg);
			break;
		case PAJetID:
			((PAJet *)pa)->Execute(pg);
			break;
		case PAKillOldID:
			((PAKillOld *)pa)->Execute(pg);
			break;
		case PAKillSlowID:
			((PAKillSlow *)pa)->Execute(pg);
			break;
		case PAMoveID:
			((PAMove *)pa)->Execute(pg);
			break;
		case PAOrbitLineID:
			((PAOrbitLine *)pa)->Execute(pg);
			break;
		case PAOrbitPointID:
			((PAOrbitPoint *)pa)->Execute(pg);
			break;
		case PARandomAccelID:
			((PARandomAccel *)pa)->Execute(pg);
			break;
		case PARandomDisplaceID:
			((PARandomDisplace *)pa)->Execute(pg);
			break;
		case PARandomVelocityID:
			((PARandomVelocity *)pa)->Execute(pg);
			break;
		case PARestoreID:
			((PARestore *)pa)->Execute(pg);
			break;
		case PASinkID:
			((PASink *)pa)->Execute(pg);
			break;
		case PASinkVelocityID:
			((PASinkVelocity *)pa)->Execute(pg);
			break;
		case PASourceID:
			((PASource *)pa)->Execute(pg);
			break;
		case PATargetColorID:
			((PATargetColor *)pa)->Execute(pg);
			break;
		case PATargetSizeID:
			((PATargetSize *)pa)->Execute(pg);
			break;
		case PAVortexID:
			((PAVortex *)pa)->Execute(pg);
			break;
                case PAHeaderID:			//tigital
                        break;
		}
	}
}

// Add the incoming action to the end of the current action list.
void _pAddActionToList(ParticleAction *S, int size)
{
	if(!_ps.in_new_list)
		return; // ERROR
	
	if(_ps.pact == NULL)
		return; // ERROR
	
	if(_ps.list_id < 0)
		return; // ERROR
	
	PAHeader *alist = _ps.pact;
	
	if(alist->actions_allocated <= alist->count)
	{
		// Must reallocate.
		int new_alloc = 16 + alist->actions_allocated;
		PAHeader *new_alist = new PAHeader[new_alloc];
		memcpy(new_alist, alist, alist->count * sizeof(PAHeader));
		
		delete [] alist;
		_ps.alist_list[_ps.list_id] = _ps.pact = alist = new_alist;
		
		alist->actions_allocated = new_alloc;
	}
	
	// Now add it in.
	memcpy(&alist[alist->count], S, size);
	alist->count++;
}

////////////////////////////////////////////////////////
// State setting calls

void pColor(float red, float green, float blue, float alpha)
{
	_ps.Alpha = alpha;
	_ps.Color = pDomain(PDPoint, red, green, blue);
}

void pColorD(float alpha, PDomainEnum dtype,
			 float a0, float a1, float a2,
			 float a3, float a4, float a5,
			 float a6, float a7, float a8)
{
	_ps.Alpha = alpha;
	_ps.Color = pDomain(dtype, a0, a1, a2, a3, a4, a5, a6, a7, a8);
}

void pVelocity(float x, float y, float z)
{
	_ps.Vel = pDomain(PDPoint, x, y, z);
}

void pVelocityD(PDomainEnum dtype,
				float a0, float a1, float a2,
				float a3, float a4, float a5,
				float a6, float a7, float a8)
{
	_ps.Vel = pDomain(dtype, a0, a1, a2, a3, a4, a5, a6, a7, a8);
}

void pVertexB(float x, float y, float z)
{
	_ps.VertexB = pDomain(PDPoint, x, y, z);
}

void pVertexBD(PDomainEnum dtype,
			   float a0, float a1, float a2,
			   float a3, float a4, float a5,
			   float a6, float a7, float a8)
{
	_ps.VertexB = pDomain(dtype, a0, a1, a2, a3, a4, a5, a6, a7, a8);
}


void pVertexBTracks(bool trackVertex)
{
	_ps.vertexB_tracks = trackVertex;
}

void pSize(float s1, float s2)
{
	if(s2 < 0.0f) s2 = s1;
	
	if(s1 < s2)
	{
		_ps.Size1 = s1;
		_ps.Size2 = s2;
	} else {
		_ps.Size1 = s2;
		_ps.Size2 = s1;
	}
}

void pStartingAge(float age)
{
	_ps.Age = age;
}

void pTimeStep(float newDT)
{
	_ps.dt = newDT;
}

////////////////////////////////////////////////////////
// Action List Calls

int pGenActionLists(int action_list_count)
{
	if(_ps.in_new_list)
		return -1; // ERROR
	
	int ind = _ps.GenerateLists(action_list_count);
	
	for(int i=ind; i<ind+action_list_count; i++)
	{
		_ps.alist_list[i] = new PAHeader[16];
		_ps.alist_list[i]->actions_allocated = 16;
		_ps.alist_list[i]->type = PAHeaderID;
		_ps.alist_list[i]->count = 1;
	}
	
	return ind;
}

void pNewActionList(int action_list_num)
{
	if(_ps.in_new_list)
		return; // ERROR
	
	_ps.pact = _ps.GetListPtr(action_list_num);
	
	if(_ps.pact == NULL)
		return; // ERROR
	
	_ps.list_id = action_list_num;
	_ps.in_new_list = true;
}

void pEndActionList()
{
	if(!_ps.in_new_list)
		return; // ERROR
	
	_ps.in_new_list = false;
	
	_ps.pact = NULL;
	_ps.list_id = -1;
}

void pDeleteActionLists(int action_list_num, int action_list_count)
{
	if(_ps.in_new_list)
		return; // ERROR

	if(action_list_num < 0)
		return; // ERROR

	if(action_list_num + action_list_count > _ps.alist_count)
		return; // ERROR

	for(int i = action_list_num; i < action_list_num + action_list_count; i++)
	{
		if(_ps.alist_list[i])
		{
			delete [] _ps.alist_list[i];
			_ps.alist_list[i] = NULL;
		}
		else
			return; // ERROR
	}
}

void pCallActionList(int action_list_num)
{
	if(_ps.in_new_list)
		return; // ERROR
	
	PAHeader *pa = _ps.GetListPtr(action_list_num);
	
	if(pa == NULL)
		return; // ERRROR

	// XXX A temporary hack.
	pa->dt = _ps.dt;
	
	_ps.in_call_list = true;
	
	_pCallActionList(pa+1, pa->count-1, _ps.pgrp);
	
	_ps.in_call_list = false;
}

////////////////////////////////////////////////////////
// Particle Group Calls

// Create particle groups, each with max_particles allocated.
int pGenParticleGroups(int p_group_count, int max_particles)
{
	if(_ps.in_new_list)
		return -1; // ERROR
	
	int ind = _ps.GenerateGroups(p_group_count);
	
	for(int i=ind; i<ind+p_group_count; i++)
	{
		_ps.group_list[i] = (ParticleGroup *)new
			Particle[max_particles + 2];
		_ps.group_list[i]->max_particles = max_particles;
		_ps.group_list[i]->particles_allocated = max_particles;
		_ps.group_list[i]->p_count = 0;
		_ps.group_list[i]->simTime = 0;
	}
	
	return ind;
}

void pDeleteParticleGroups(int p_group_num, int p_group_count)
{
	if(p_group_num < 0)
		return; // ERROR

	if(p_group_num + p_group_count > _ps.group_count)
		return; // ERROR

	for(int i = p_group_num; i < p_group_num + p_group_count; i++)
	{
		if(_ps.group_list[i])
		{
			delete [] _ps.group_list[i];
			_ps.group_list[i] = NULL;
		}
		else
			return; // ERROR
	}
}

// Change which group is current.
void pCurrentGroup(int p_group_num)
{
	if(_ps.in_new_list)
		return; // ERROR
	
	_ps.pgrp = _ps.GetGroupPtr(p_group_num);
	if(_ps.pgrp)
		_ps.group_id = p_group_num;
	else
		_ps.group_id = -1;
}

// Change the maximum number of particles in the current group.
int pSetMaxParticles(int max_count)
{
	if(_ps.in_new_list)
		return 0; // ERROR
	
	ParticleGroup *pg = _ps.pgrp;
	if(pg == NULL)
		return 0; // ERROR
	
	if(max_count < 0)
		return 0; // ERROR

	// Reducing max.
	if(pg->particles_allocated >= max_count)
	{
		pg->max_particles = max_count;

		// May have to kill particles.
		if(pg->p_count > pg->max_particles)
			pg->p_count = pg->max_particles;

		return max_count;
	}
	
	// Allocate particles.
	ParticleGroup *pg2 =(ParticleGroup *)new Particle[max_count + 2];
	if(pg2 == NULL)
	{
		// Not enough memory. Just give all we've got.
		// ERROR
		pg->max_particles = pg->particles_allocated;
		
		return pg->max_particles;
	}
	
	memcpy(pg2, pg, (pg->p_count + 2) * sizeof(Particle));
	
	delete [] pg;
	
	_ps.group_list[_ps.group_id] = _ps.pgrp = pg2;
	pg2->max_particles = max_count;
	pg2->particles_allocated = max_count;

	return max_count;
}

// Copy from the specified group to the current group.
void pCopyGroup(int p_group_num, int index, int copy_count)
{
	if(_ps.in_new_list)
		return; // ERROR
	
	ParticleGroup *srcgrp = _ps.GetGroupPtr(p_group_num);
	if(srcgrp == NULL)
		return; // ERROR

	ParticleGroup *destgrp = _ps.pgrp;
	if(destgrp == NULL)
		return; // ERROR

	// Find out exactly how many to copy.
	int ccount = copy_count;
	if(ccount > srcgrp->p_count - index)
		ccount = srcgrp->p_count - index;
	if(ccount > destgrp->max_particles - destgrp->p_count)
		ccount = destgrp->max_particles - destgrp->p_count;
	
	// Directly copy the particles to the current list.
	for(int i=0; i<ccount; i++)
	{
		destgrp->list[destgrp->p_count+i] =
			srcgrp->list[index+i];
	}
	destgrp->p_count += ccount;
}

// Copy from the current group to application memory.
void pGetParticles(int index, int count, float *verts,
				   float *color, float *vel, float *size)
{
	// XXX I should think about whether color means color3, color4, or what.
	// For now, it means color4.

	if(_ps.in_new_list)
		return; // ERROR
		
	ParticleGroup *pg = _ps.pgrp;
	if(pg == NULL)
		return; // ERROR

	if(index + count > pg->p_count)
		return; // ERROR can't ask for more than there are.

	int vi = 0, ci = 0, li = 0, si = 0;

	// This could be optimized.
	for(int i=0; i<count; i++)
	{
		Particle &m = pg->list[index + i];

		if(verts)
		{
			verts[vi++] = m.pos.x;
			verts[vi++] = m.pos.y;
			verts[vi++] = m.pos.z;
		}

		if(color)
		{
			color[ci++] = m.color.x;
			color[ci++] = m.color.y;
			color[ci++] = m.color.z;
			color[ci++] = m.alpha;
		}

		if(vel)
		{
			vel[li++] = m.vel.x;
			vel[li++] = m.vel.y;
			vel[li++] = m.vel.z;
		}

		if(size)
			size[si++] = m.size;
	}
}

// Returns the number of particles currently in the group.
int pGetGroupCount()
{
	if(_ps.in_new_list)
		return 0; // ERROR
	
	if(_ps.pgrp == NULL)
		return 0; // ERROR
	
	return _ps.pgrp->p_count;
}


// Emit OpenGL calls to draw the particles. These are drawn with
// whatever primitive type the user specified(GL_POINTS, for
// example). The color and radius are set per primitive, by default.
// For GL_LINES, the other vertex of the line is the velocity vector.
void pDrawGroupp(int primitive, bool const_size, bool const_color)
{
	// Get a pointer to the particles in gp memory
	ParticleGroup *pg = _ps.pgrp;
	if(pg == NULL)
		return; // ERROR
		
	if(pg->p_count < 1)
		return;

	//if(const_color)
	//	glColor4fv((GLfloat *)&pg->list[0].color);
	
	glBegin((GLenum)primitive);
	
	for(int i = 0; i < pg->p_count; i++)
	{
		Particle &m = pg->list[i];
		
		// Warning: this depends on alpha following color in the Particle struct.
		if(!const_color)
			glColor4fv((GLfloat *)&m.color);
		glVertex3fv((GLfloat *)&m.pos);
		
		// For lines, make a tail with the velocity vector's direction and
		// a length of radius.
		if(primitive == GL_LINES) {
			pVector tail(-m.vel.x, -m.vel.y, -m.vel.z);
			tail *= m.size;
			tail += m.pos;
			
			glVertex3fv((GLfloat *)&tail);
		}
	}
	
	glEnd();
}

void pDrawGroupl(int dlist, bool const_size, bool const_color, bool const_rotation)
{
	// Get a pointer to the particles in gp memory
	ParticleGroup *pg = _ps.pgrp;
	if(pg == NULL)
		return; // ERROR
	
	if(pg->p_count < 1)
		return;

	//if(const_color)
	//	glColor4fv((GLfloat *)&pg->list[0].color);

	for(int i = 0; i < pg->p_count; i++)
	{
		Particle &m = pg->list[i];

		glPushMatrix();
		glTranslatef(m.pos.x, m.pos.y, m.pos.z);

		if(!const_size)
			glScalef(m.size, m.size, m.size);

		// Expensive! A sqrt, cross prod and acos. Yow.
		if(!const_rotation)
		{
			pVector vN(m.vel);
			vN.normalize();
			pVector voN(m.velB);
			voN.normalize();

			pVector biN;
			if(voN.x == vN.x && voN.y == vN.y && voN.z == vN.z)
				biN = pVector(0, 1, 0);
			else
				biN = vN ^ voN;
			biN.normalize();

			pVector N(vN ^ biN);

			double M[16];
			M[0] = vN.x;  M[4] = biN.x;  M[8] = N.x;  M[12] = 0;
			M[1] = vN.y;  M[5] = biN.y;  M[9] = N.y;  M[13] = 0;
			M[2] = vN.z;  M[6] = biN.z;  M[10] = N.z; M[14] = 0;
			M[3] = 0;     M[7] = 0;      M[11] = 0;   M[15] = 1;
			glMultMatrixd(M);
		}

		// Warning: this depends on alpha following color in the Particle struct.
		if(!const_color)
			glColor4fv((GLfloat *)&m.color);

		glCallList(dlist);

		glPopMatrix();
	}
	
	glEnd();
}
