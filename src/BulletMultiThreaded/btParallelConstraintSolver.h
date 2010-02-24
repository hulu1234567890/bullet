/*
   Copyright (C) 2010 Sony Computer Entertainment Inc.
   All rights reserved.

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

*/

#ifndef __BT_PARALLEL_CONSTRAINT_SOLVER_H
#define __BT_PARALLEL_CONSTRAINT_SOLVER_H

#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
								
#define BT_PARALLEL_CONSTRAINT_SOLVER_MAX_THREADS 16

struct btParallelConstraintSolverSetupTaskParams
{
	int m_numContactConstraints;
	int	m_startIndex;
	int m_currIndex;
};

class btParallelConstraintSolver; 

void kSetupContact(btParallelConstraintSolver* pSolver, 
				  btParallelConstraintSolverSetupTaskParams* pParams, 
				  btContactSolverInfo* pInfoGlobal, int threadId);

// defined in btSequentialImpulseConstraintSolver.cpp
extern void	applyAnisotropicFriction(btCollisionObject* colObj,btVector3& frictionDirection);

class btParallelConstraintSolver : public btSequentialImpulseConstraintSolver
{
protected:
	int m_numFrictonPerContact;
	btParallelConstraintSolverSetupTaskParams m_taskParams[BT_PARALLEL_CONSTRAINT_SOLVER_MAX_THREADS]; // enough for MiniCL


public:
	int m_localGroupSize;
	btPersistentManifold** m_manifoldPtr; 
	int m_numManifolds;

	btParallelConstraintSolver();
	
	virtual ~btParallelConstraintSolver();

	//virtual btScalar solveGroup(btCollisionObject** bodies,int numBodies,btPersistentManifold** manifold,int numManifolds,btTypedConstraint** constraints,int numConstraints,const btContactSolverInfo& info, btIDebugDraw* debugDrawer, btStackAlloc* stackAlloc,btDispatcher* dispatcher);
	
	virtual btScalar solveGroupCacheFriendlySetup(btCollisionObject** bodies,int numBodies,btPersistentManifold** manifoldPtr, int numManifolds,btTypedConstraint** constraints,int numConstraints,const btContactSolverInfo& infoGlobal,btIDebugDraw* debugDrawer,btStackAlloc* stackAlloc);
	virtual btScalar solveGroupCacheFriendlyIterations(btCollisionObject** bodies,int numBodies,btPersistentManifold** manifoldPtr, int numManifolds,btTypedConstraint** constraints,int numConstraints,const btContactSolverInfo& infoGlobal,btIDebugDraw* debugDrawer,btStackAlloc* stackAlloc);

	// returns number of contact constraints dispatched
	int prepareBatches(btPersistentManifold** manifold,int numManifolds, const btContactSolverInfo& infoGlobal);


	friend void kSetupContact(	btParallelConstraintSolver* pSolver, 
								btParallelConstraintSolverSetupTaskParams* pParams, 
								btContactSolverInfo* pInfoGlobal, int threadId);

	friend void kSolveContact(	btParallelConstraintSolver* pSolver, 
								btParallelConstraintSolverSetupTaskParams* pParams, 
								btContactSolverInfo* pInfoGlobal, int threadId);
};



#endif //__BT_PARALLEL_CONSTRAINT_SOLVER_H