/**
* Copyright (c) 2012-2014 CNRS
* Author: Olivier Roussel
*
* This file is part of the qserl package.
* qserl is free software: you can redistribute it
* and/or modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation, either version
* 3 of the License, or (at your option) any later version.
*
* qserl is distributed in the hope that it will be
* useful, but WITHOUT ANY WARRANTY; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Lesser Public License for more details.  You should have
* received a copy of the GNU Lesser General Public License along with
* qserl.  If not, see
* <http://www.gnu.org/licenses/>.
**/
#ifndef QSERL_2D_WORKSPACE_INTEGRATED_STATE_H_
#define QSERL_2D_WORKSPACE_INTEGRATED_STATE_H_

#include "qserl/exports.h"

#include <boost/array.hpp>
#include <boost/function.hpp>

#include "qserl/rod2d/workspace_state.h"
#include "qserl/rod2d/parameters.h"
#include "util/forward_class.h"

namespace qserl {
namespace rod2d {

DECLARE_CLASS( WorkspaceIntegratedState );

class QSERL_EXPORT WorkspaceIntegratedState : public WorkspaceState
{
public:
	
	/** Rod state types. */
	typedef boost::array<double, 9>	  state_type;						/**< Type of 2D elastic rod states q(t) at position t, where q(t) is an element of the Lie Group SE(2). */
	typedef boost::array<double, 3>		costate_type;					/**< Type of 2D elastic rod co-states mu(t) at position t, where mu(t) is an element of the dual Lie algebra se(2)*. */
	typedef boost::array<double, 18>	jacobian_state_type;	/**< Type of 2D elastic rod co-state and state derviates M(t) ( resp. J(t) ) at position t, where:
																													     - M(t) is the 3x3 Jacobian matrix of the co-state mu(t) w.r.t. initial conditions (i.e. mu(0)) (first 9 elements),
																															 - J(t) is the 3x3 Jacobian matrix of the state q(t) w.r.t. initial conditions (i.e. mu(0)) (last 9 elements). */

	/**
	* \brief Destructor.
	*/
	virtual ~WorkspaceIntegratedState();

	/**
	* \brief Constructor.
	* Rod base is independant from this as node positions are computed in local base frame.
	*/
	static WorkspaceIntegratedStateShPtr create(const Wrench2D& i_baseWrench, unsigned int i_nnodes, 
		const Displacement2D& i_basePosition, const Parameters& i_rodParams);

	/**
	* \brief Copy constructor.
	*/
	static WorkspaceIntegratedStateShPtr createCopy(const WorkspaceIntegratedStateConstShPtr& i_other);

	/**
	* \brief Returns a copy of itself.
	*/
	virtual WorkspaceStateShPtr clone() const;

	/**
	* \brief Compute rod state from its base wrench by integration.
	* \return False if could not integrate state (abnormal case).
	*/
	bool integrate();

	/**
	* \brief Returns true if rod configuration is in a stable quasi-static confiugration.
	* \pre Rod must be initialized.
	*/
	bool isStable() const;

	/**
	* \brief Returns the wrench at the rod given node.
	*/
	Wrench2D wrench(size_t i_idxNode) const;

	/**
	* \brief Const accessor to the wrenches (costate) of the rod for each node. 
	*   \warning Only accessible if the keepMuValues() has been set to true,
	*		with the exception of mu(0) (i.e. the base wrench) which is kept in any case.
	*/
	const std::vector<costate_type>& mu() const;

	/** \brief Returns the M matrix (i.e. dmu(t) / dmu(0) ).
	*   \warning Only accessible if the keepMMatrices integration option has been set to true.
	*/
	const Eigen::Matrix<double, 3, 3>& getMMatrix(size_t i_nodeIdx) const;

	/** \brief Returns the J matrix (i.e. dq(t) / dmu(0) ).
	*   \warning Only accessible if the keepJMatrices integration option has been set to true.
	*/
	const Eigen::Matrix<double, 3, 3>& getJMatrix(size_t i_nodeIdx) const;

	/**
	* \brief Returns the values for each node of the jacobian determinant.
	* \warning If the DLO is detected as unstable, the determinant will be 0 from
	* the instability point.
	* \warning Only accessible if the keepJdet() has been set to true.
	*/
	const std::vector<double>& J_det() const;

	/**
	* \brief Returns the memory usage of this instance.
	*/
	size_t memUsage() const;

	/**
	* \brief Integration computation options.
	*/
	struct IntegrationOptions
	{
		/** 
		* Constructor. 
		* Initialize to default values.
		*/
		IntegrationOptions();

		bool									stop_if_unstable;		/**< True if integration process should be stop if configuration is detected as not stable. */
		bool									keepMuValues;
		bool									keepJdet;
		bool									keepMMatrices;
		bool									keepJMatrices;
	};

	/**
	* \brief Set integration options.
	* Must be done beofre integrate() call
	*/
	void integrationOptions(const IntegrationOptions& i_integrationOptions);

	/**
	* \brief Accessor to integration options.
	*/
	const IntegrationOptions& integrationOptions() const;

protected:
	
	/**
	\brief Constructor
	*/
	WorkspaceIntegratedState(unsigned int i_nnodes, const Displacement2D& i_basePosition, 
		const Parameters& i_rodParams);

	/**
	\brief Init function
	*/
	bool init(const Wrench2D& i_wrench);

	/** \brief Returns true if could integrate state (even if it is not a stable
			state, in which case m_isStable attribute is set to false).
			Returns false if the input wrench cannot be integrated (singular configurations). */
	bool integrateFromBaseWrench(const Wrench2D& i_wrench);


	bool																										m_isStable;		/**< True if DLO state is stable. */
	std::vector<costate_type>																m_mu;					/**< Wrenches at each nodes (size N). */
	std::vector<Eigen::Matrix<double, 3, 3> >								m_M;
	std::vector<Eigen::Matrix<double, 3, 3> >								m_J;

	std::vector<double>																			m_J_det;

	IntegrationOptions																			m_integrationOptions;
};

}	// namespace rod2d
}	// namespace qserl

#endif // QSERL_2D_WORKSPACE_INTEGRATED_STATE_H_
