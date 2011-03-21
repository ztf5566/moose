#ifndef MPROBLEM_H_
#define MPROBLEM_H_

#include "SubProblem.h"
#include "Mesh.h"
#include "ImplicitSystem.h"
#include "AuxiliarySystem.h"
#include "AssemblyData.h"

namespace Moose {

class DisplacedProblem;

/**
 * Specialization of SubProblem for solving nonlinear equations plus auxiliary equations
 *
 */
class MProblem : public SubProblem
{
public:
  MProblem(Mesh & mesh, Problem * parent = NULL);
  virtual ~MProblem();

  virtual Order getQuadratureOrder() { return _quadrature_order; }
  virtual QBase * & qRule(THREAD_ID tid) { return _asm_info[tid]->qRule(); }
  virtual const std::vector<Point> & points(THREAD_ID tid) { return _asm_info[tid]->qPoints(); }
  virtual const std::vector<Real> & JxW(THREAD_ID tid) { return _asm_info[tid]->JxW(); }
  virtual QBase * & qRuleFace(THREAD_ID tid) { return _asm_info[tid]->qRuleFace(); }
  virtual const std::vector<Point> & pointsFace(THREAD_ID tid) { return _asm_info[tid]->qPointsFace(); }
  virtual const std::vector<Real> & JxWFace(THREAD_ID tid) { return _asm_info[tid]->JxWFace(); }

  virtual FEBase * & getFE(THREAD_ID tid, const FEType & fe_type) { return _asm_info[tid]->getFE(fe_type); }
  virtual FEBase * & getFEFace(THREAD_ID tid, const FEType & fe_type) { return _asm_info[tid]->getFEFace(fe_type); }
  virtual const Elem * & elem(THREAD_ID tid) { return _asm_info[tid]->elem(); }
  virtual unsigned int & side(THREAD_ID tid) { return _asm_info[tid]->side(); }
  virtual const Elem * & sideElem(THREAD_ID tid) { return _asm_info[tid]->sideElem(); }
  virtual const Node * & node(THREAD_ID tid) { return _asm_info[tid]->node(); }

  virtual void reinitElem(const Elem * elem, THREAD_ID tid);
  virtual void reinitElemFace(const Elem * elem, unsigned int side, unsigned int bnd_id, THREAD_ID tid);
  virtual void reinitNode(const Node * node, THREAD_ID tid);
  virtual void reinitNodeFace(const Node * node, unsigned int bnd_id, THREAD_ID tid);

  virtual void subdomainSetup(unsigned int subdomain, THREAD_ID tid);

  virtual void init();
  virtual void init2();
  virtual void update();
  virtual void solve();
  virtual bool converged();

  virtual void onTimestepBegin();
  virtual void onTimestepEnd();

  virtual void copySolutionsBackwards();

  // NL /////
  void addVariable(const std::string & var_name, const FEType & type, const std::set< subdomain_id_type > * const active_subdomains = NULL);

  void addKernel(const std::string & kernel_name, const std::string & name, InputParameters parameters);

  void addBoundaryCondition(const std::string & bc_name, const std::string & name, InputParameters parameters);

  ImplicitSystem & getNonlinearSystem() { return _nl; }

  // Aux /////
  void addAuxVariable(const std::string & var_name, const FEType & type, const std::set< subdomain_id_type > * const active_subdomains = NULL);

  void addAuxKernel(const std::string & kernel_name, const std::string & name, InputParameters parameters);

  void addAuxBoundaryCondition(const std::string & bc_name, const std::string & name, InputParameters parameters);

  AuxiliarySystem & getAuxiliarySystem() { return _aux; }

  // Stabilization /////
  void addStabilizer(const std::string & stabilizer_name, const std::string & name, InputParameters parameters);

  ////
  virtual void computeResidual(NonlinearImplicitSystem & sys, const NumericVector<Number> & soln, NumericVector<Number> & residual);
  virtual void computeJacobian(NonlinearImplicitSystem & sys, const NumericVector<Number> & soln, SparseMatrix<Number> &  jacobian);

  // Displaced problem /////
  virtual void initDisplacedProblem(const std::vector<std::string> & displacements);

  // Output /////
  virtual void output();

protected:
  ImplicitSystem _nl;
  AuxiliarySystem _aux;

  // quadrature
  Order _quadrature_order;                              /// Quadrature order required by all variables to integrated over them.
  std::vector<AssemblyData *> _asm_info;

  // Displaced mesh /////
  Mesh * _displaced_mesh;
  DisplacedProblem * _displaced_problem;
  bool _output_displaced;                               /// true for outputting displaced problem

public:
  static unsigned int _n;                               /// number of instances of MProblem (to distinguish Systems when coupling problems together)
};

} // namespace

#endif /* MPROBLEM_H_ */
