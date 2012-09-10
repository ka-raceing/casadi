/*
 *    This file is part of CasADi.
 *
 *    CasADi -- A symbolic framework for dynamic optimization.
 *    Copyright (C) 2010 by Joel Andersson, Moritz Diehl, K.U.Leuven. All rights reserved.
 *
 *    CasADi is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 3 of the License, or (at your option) any later version.
 *
 *    CasADi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with CasADi; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef INTEGRATOR_INTERNAL_HPP
#define INTEGRATOR_INTERNAL_HPP

#include "integrator.hpp"
#include "fx_internal.hpp"

namespace CasADi{

/** \brief Internal storage for integrator related data

  @copydoc DAE_doc
  \author Joel Andersson 
  \date 2010
*/
class IntegratorInternal : public FXInternal{
public:
  /** \brief  Constructor */
  IntegratorInternal(const FX& f, const FX& g);

  /** \brief  Destructor */
  virtual ~IntegratorInternal()=0;

  /** \brief  Clone */
  virtual IntegratorInternal* clone() const=0;

  /** \brief  Deep copy data members */
  virtual void deepCopyMembers(std::map<SharedObjectNode*,SharedObject>& already_copied);
  
  /** \brief  Create a new integrator */
  virtual IntegratorInternal* create(const FX& f, const FX& g) const = 0;
  
  /** \brief  Set linear solver */
  virtual void setLinearSolver(const LinearSolver& linsol, const FX& jac)=0;
  
  /** \brief  Print solver statistics */
  virtual void printStats(std::ostream &stream) const = 0;

    /** \brief  Reset the solver and bring the time back to t0 */
  virtual void reset() = 0;

    /** \brief  Reset the solver of the adjoint problem and take time to tf */
  virtual void resetAdj() = 0;

  /** \brief  Integrate until a specified time point */
  virtual void integrate(double t_out) = 0;

  /** \brief  Integrate backwards in time until a specified time point */
  virtual void integrateAdj(double t_out) = 0;

  /** \brief  evaluate */
  virtual void evaluate(int nfdir, int nadir);

  /** \brief  Initialize */
  virtual void init();

  /// Is the class able to propate seeds through the algorithm?
  virtual bool spCanEvaluate(bool fwd){ return true;}
    
  /// Generate a function that calculates nfwd forward derivatives and nadj adjoint derivatives
  virtual FX getDerivative(int nfwd, int nadj);

  /// Generate a augmented DAE system with nfwd forward sensitivities and nadj adjoint sensitivities
  virtual std::pair<FX,FX> getAugmented(int nfwd, int nadj);
  
  /// Generate a augmented DAE system with nfwd forward sensitivities and nadj adjoint sensitivities (generic)
  template<class Mat,class XFunc>
  std::pair<FX,FX> getAugmentedGen(int nfwd, int nadj);
  
  /// Number of states for the forward integration
  int nx_, nz_, nq_;
  
  /// Number of states for the backward integration
  int nrx_, nrz_, nrq_;

  /// Number of forward and backward parameters
  int np_, nrp_;

  /// Integration horizon
  double t0_, tf_;
  
  /// ODE/DAE forward integration function
  FX f_;
  
  /// ODE/DAE backward integration function, if any
  FX g_;
  
  /// Number of right hand sides
  int nrhs_;
};
  
} // namespace CasADi

#endif // INTEGRATOR_INTERNAL_HPP
