 
//*************   symmetry ***************************//

#include <gecode/int/branch.hh>
#include <gecode/int.hh>
#include <gecode/ReSBDS/head.hpp>

namespace Gecode {

 
  IntArgs myval; 
  int s_val;
 

  //symmetry class
  class ReSBDS_Sym  {
    private:
        Space& _home;
        int _nSym;
      	IntArgs _nSymR;
		int _dosize;
		int _length;
		int _depth;
	public:
		
		BoolVarArray _symvars;
		~ReSBDS_Sym() {} ;
		Space& getManager() {return _home;};
        int getDepth() {return _depth;};
		int getLength() {return _length;};
		int getDosize() {return _dosize;};
		int getNSym() {return _nSym;}; 
		   

		void symGoal(ViewArray<Int::IntView>& vars, int index, int val );//-----------------------------this is the normal ReSBDS adds conditional contrains
		ReSBDS_Sym(Space& home, int nSym,	int var_n,int dosize) ;
		void SymAdjust(  ViewArray<Int::IntView>& vars, int index, int val) ;
          
		ReSBDS_Sym(Space& home, ReSBDS_Sym OldSym) ;
  };



  void ReSBDS_Sym::symGoal(ViewArray<Int::IntView>& vars, int index, int val) {

		// special for IntVars but could be generalised
        if (_nSym == 0) { return ;} // i.e. no goal 	
			       
        else { 
            int i = 0; 
			Index_class k;
 
			  
            for (; i < _nSym; i++) 
			{  	
				if(_symvars[i].zero())      continue;
				k=(*_symmetries)(_nSymR[i],index,val); 
				if((k.index!=index||k.val!=val)&&vars[k.index].in(k.val))	{
					rel(_home,(!_symvars[i]||(vars[k.index]!=k.val)));
					//record this assignment into myval
					int r=k.index*_dosize+k.val;
    				int s;
					for(s=0;s<_length && r!=myval[_depth*s_val+s];s++);
					if(s==_length)
						myval[_depth*s_val+_length++]=r;
				}
            }
					 
        }
  };                          
  ReSBDS_Sym::ReSBDS_Sym(Space& home, int nSym, int var_n,int dosize0) :
                _home(home),
                _nSym(nSym), 
				_symvars(_home,nSym,0,1),
				_dosize(dosize0)
	{
	
		for(int i=0;i<_nSym;i++)
			rel(home,_symvars[i]==1);

		_nSymR=IntArgs(_nSym); 

		for(int i=0;i<_nSym;i++)
			_nSymR[i]=i; 
		s_val=var_n*_dosize;
		
		myval=IntArgs(s_val*(var_n+1));

		_length=0;
		_depth=0;

	};
	ReSBDS_Sym::ReSBDS_Sym(Space& home,ReSBDS_Sym OldSym) :
                _nSym(OldSym.getNSym()),
                _home(home),
				_symvars(_home,OldSym.getNSym(),0,1),
                _dosize(OldSym.getDosize()),_length(OldSym.getLength()),
				_depth(OldSym.getDepth()),
				_nSymR(OldSym.getNSym())
	{
		for(int i=0;i<_nSym;i++)	
			_nSymR[i]=OldSym._nSymR[i];
		//incrase the depth
		OldSym._depth++; 
		int v1=OldSym._depth*s_val;
		int v2=_depth*s_val;
		//copy from parent node to child node
		for(int i=0;i<_length;i++)
			myval[v1+i]=myval[v2+i];
		 
		

	};
  void ReSBDS_Sym::SymAdjust(ViewArray<Int::IntView>& vars, 
				int index, int val) 
	{
		int oldNSym = _nSym; 

		_nSym = 0;
		for (int i = 0 ; i < oldNSym ; i++) 
			if (!_symvars[i].zero()) 
			{
				Index_class k=(*_symmetries)(_nSymR[i],index,val);
			 
				if(k.index==index&&k.val==val)  _symvars[_nSym]=_symvars[i]; else
				{ 
					if(vars[k.index].in(k.val))
					{	//record this assignment into myval
						int r=k.index*_dosize+k.val;
						int s;
						for(s=0;s<_length && r!=myval[_depth*s_val+s];s++);
						if(s==_length)
							myval[_depth*s_val+_length++]=r;
					}
					_symvars[_nSym]=expr(_home,_symvars[i]&&(vars[k.index]==k.val));					 
				}
				_nSymR[_nSym++]=_nSymR[i];
			}
	};



//**************************************************//
//**************************************************//
//**************************************************//
//**************************************************//
//**************************************************//
//*************   branch ***************************//



  template<class View, int n, class Val, unsigned int a>
  class ReSBDSBrancher : public ViewValBrancher<View,n,Val,a> {
    typedef typename ViewBrancher<View,n>::BranchFilter BranchFilter;
  public:
    /// Array of symmetry implementations
    ReSBDS_Sym SymObject;
	int dosize; 
  protected:
    /// Constructor for cloning \a b
    ReSBDSBrancher(Space& home, bool share, ReSBDSBrancher& b);
    /// Constructor for creation
	
	
    ReSBDSBrancher(Home home, 
                 ViewArray<View>& x,
                 ViewSel<View>* vs[n], 
                 ValSelCommitBase<View,Val>* vsc,
                 int nSym, 	int dosize0,
                 BranchFilter bf,IntVarValPrint vvp);
  public:
    /// Return choice
    virtual const Choice* choice(Space& home);
    /// Return choice
    virtual const Choice* choice(const Space& home, Archive& e);
    /// Perform commit for choice \a c and alternative \a b
    virtual ExecStatus commit(Space& home, const Choice& c, unsigned int b);
    /// Perform cloning
    virtual Actor* copy(Space& home, bool share);
    /// Perform dispose
    virtual size_t dispose(Space& home);
    /// Delete brancher and return its size
    static BrancherHandle post(Home home,
                               ViewArray<View>& x,
                               ViewSel<View>* vs[n],
                               ValSelCommitBase<View,Val>* vsc,
                               int nSym, 	int dosize0,
                               BranchFilter bf,IntVarValPrint vvp);
  };

  template<class View, int n, class Val, unsigned int a>
  ReSBDSBrancher<View,n,Val,a>
  ::ReSBDSBrancher(Home home, ViewArray<View>& x,
                 ViewSel<View>* vs[n],
                 ValSelCommitBase<View,Val>* vsc,
                 int nSym, 	int dosize0,
                 BranchFilter bf,IntVarValPrint vvp)
    : ViewValBrancher<View,n,Val,a>(home, x, vs, vsc, bf,vvp),
      SymObject(home,nSym, x.size(),dosize0),
	  dosize(dosize0)
  {   
	home.notice(*this, AP_DISPOSE);
	     
  }

  template<class View, int n, class Val, unsigned int a>
  forceinline BrancherHandle
  ReSBDSBrancher<View,n,Val,a>::
  post(Home home, ViewArray<View>& x,
       ViewSel<View>* vs[n], ValSelCommitBase<View,Val>* vsc,
       int nSym, 	int dosize0,
       BranchFilter bf,IntVarValPrint vvp) {
	return *new (home) ReSBDSBrancher<View,n,Val,a>(home,x,vs,vsc,nSym, dosize0,bf,vvp);
  }

  template<class View, int n, class Val, unsigned int a>
  forceinline
  ReSBDSBrancher<View,n,Val,a>::
  ReSBDSBrancher(Space& home, bool shared, ReSBDSBrancher<View,n,Val,a>& b)
    : ViewValBrancher<View,n,Val,a>(home,shared,b), 
      SymObject(home,b.SymObject),
      dosize(b.dosize) { 
	SymObject._symvars.update(home,shared,b.SymObject._symvars);

  }
  
  template<class View, int n, class Val, unsigned int a>
  Actor*
  ReSBDSBrancher<View,n,Val,a>::copy(Space& home, bool shared) {
	return new (home) ReSBDSBrancher<View,n,Val,a>(home,shared,*this);
  }


  // Compute choice
  template<class View, int n, class Val, unsigned int a>
  const Choice*
  ReSBDSBrancher<View,n,Val,a>::choice(Space& home) { 
	return ViewValBrancher<View,n,Val,a>::choice(home);
 
  }

 template<class View, int n, class Val, unsigned int a>
  const Choice*
  ReSBDSBrancher<View,n,Val,a>::choice(const Space& home, Archive& e) {
	return ViewValBrancher<View,n,Val,a>::choice(home,e);
  }
  template<class View, int n, class Val, unsigned int a>
  size_t
  ReSBDSBrancher<View,n,Val,a>::dispose(Space& home) {
	home.ignore(*this,AP_DISPOSE);
	(void) ViewValBrancher<View,n,Val,a>::dispose(home);
	return sizeof(ReSBDSBrancher<View,n,Val,a>);
  }
  template<class View, int n, class Val, unsigned int a>
  ExecStatus
  ReSBDSBrancher<View,n,Val,a>
  ::commit(Space& home, const Choice& c, unsigned int b) {
  
 
 

		const PosValChoice<Val>& pvc
		  = static_cast<const PosValChoice<Val>&>(c);
		int pos = pvc.pos().pos;
		int val = pvc.val();
	 
		 
		if(b==0)
		{ 
			SymObject.SymAdjust(this->x,pos,val);//ajust symmetries
			GECODE_ME_CHECK(this->x[pos].eq(home,val)); 
		}
		else
		{
			
			SymObject.symGoal(this->x,pos, val); //post symmetry breaking constraint upon backtracking
			GECODE_ME_CHECK(this->x[pos].nq(home,val));         
		}
		StatusStatistics cc;
		home.status(cc); 
		
		
		//add additional constraints 
		int re_index=1;
		while(re_index!=0)
		{  
			 
			re_index=0;
			int length=SymObject.getLength();
			int depth=SymObject.getDepth();
			for(int i=0;i<length;i++)
			if(myval[depth*s_val+i]!=-1)
			{ 
				int p=myval[depth*s_val+i]/dosize;
				int q=myval[depth*s_val+i]%dosize;
				
				if(!this->x[p].in(q))//check wherther recorded assignment is pruned   
				{
					//post extra symmetry breaking constraint 
					SymObject.symGoal(this->x,p, q);
					myval[depth*s_val+i]=-1;
					re_index++;        
				}
			}
	 
			
			if(re_index!=0){ 
				//enforce consistency
				StatusStatistics cc;
				home.status(cc); 
			}
			
		}


	  


		return ES_OK;
  }
  
//**************************************************//
//**************************************************//
//**************************************************//
//**************************************************//
//**************************************************//
//*************   post ***************************//

  


  BrancherHandle
  branch(Home home, const IntVarArgs& x,
         IntVarBranch vars, IntValBranch vals,
         int nSym, SymmetryFunction symfunctions,  int dosize0,IntBranchFilter bf=NULL) {
    using namespace Int;
    if (home.failed()) return BrancherHandle();
    ViewArray<IntView> xv(home,x);
    ViewSel<IntView>* vs[1] = { 
      Branch::viewselint(home,vars) 
    };

	_symmetries=symfunctions;
    return ReSBDSBrancher<IntView,1,int,2>::post
    (home,xv,vs,Branch::valselcommitint(home,x.size(),vals),nSym, dosize0,bf,NULL);
    }
 
}


  

