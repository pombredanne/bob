/* Tensor type */
#define TENSOR_T_(TYPE) TH##TYPE##Tensor
#define TENSOR_T(TYPE) TENSOR_T_(TYPE)
#define TENSOR TENSOR_T(CAP_TYPE)

/* Function name for a Tensor */
#define TENSOR_FUNC_TN_(TYPE,NAME) TH##TYPE##Tensor_##NAME
#define TENSOR_FUNC_TN(TYPE, NAME) TENSOR_FUNC_TN_(TYPE,NAME)
#define TENSOR_FUNC(NAME) TENSOR_FUNC_TN(CAP_TYPE, NAME)

/* Class name for a Tensor */
#define TENSOR_CLASS_TN_(TYPE,NAME) TYPE##NAME
#define TENSOR_CLASS_TN(TYPE, NAME) TENSOR_CLASS_TN_(TYPE,NAME)
#define TENSOR_CLASS(NAME) TENSOR_CLASS_TN(CAP_TYPE, NAME)

/* For the default Tensor type, we simplify the naming */
#ifdef DEFAULT_TENSOR
#undef TENSOR
#undef TENSOR_FUNC
//#undef TENSOR_CLASS
#define TENSOR THTensor
#define TENSOR_FUNC(NAME) TENSOR_FUNC_TN(, NAME)
//#define TENSOR_CLASS(NAME) TENSOR_CLASS_TN(, NAME)
#endif

class TENSOR_CLASS(Tensor) : public Tensor
{
 public:

  TENSOR *t;

  ///

  //
  TENSOR_CLASS(Tensor)();

  //
  TENSOR_CLASS(Tensor)(long dim0_);

  //
  TENSOR_CLASS(Tensor)(long dim0_, long dim1_);

  //
  TENSOR_CLASS(Tensor)(long dim0_, long dim1_, long dim2_);

  //
  TENSOR_CLASS(Tensor)(long dim0_, long dim1_, long dim2_, long dim3_);

  //
  virtual int nDimension() const { if(t != NULL) return t->nDimension; return 0; };

  //
  virtual long size(int dimension_) const { if(t != NULL && dimension_ >= 0 && dimension_ < t->nDimension) return t->size[dimension_]; return 0; };

  //
  virtual void setTensor(const Tensor *src_);

  //
  virtual void copy(const Tensor *src_);

  //
  virtual void transpose(const Tensor *src, int dimension1_, int dimension2_);

  //
  virtual void narrow(const Tensor *src, int dimension_, long firstIndex_, long size_);

  //
  virtual void select(const Tensor *src, int dimension_, long sliceIndex_);

  //
  virtual Tensor* select(int dimension_, long sliceIndex_) const;

  //
  virtual void unfold(const Tensor *src, int dimension_, long size_, long step_);

  //
  virtual void print(const char *name = NULL) const;
  
  //
  virtual void sprint(const char *name, ...) const;

  //---

  //
  void fill(TYPE value);

  //
  TYPE get(long) const;
  TYPE get(long, long) const;
  TYPE get(long, long, long) const;
  TYPE get(long, long, long, long) const;

  //
  void set(long, TYPE);
  void set(long, long, TYPE);
  void set(long, long, long, TYPE);
  void set(long, long, long, long, TYPE);

  //
  TYPE &operator()(long);
  TYPE &operator()(long, long);
  TYPE &operator()(long, long, long);
  TYPE &operator()(long, long, long, long);

  //
  virtual ~TENSOR_CLASS(Tensor)();
};

void Tprint(const TENSOR_CLASS(Tensor) *T);

#undef TENSOR_T_
#undef TENSOR_T
#undef TENSOR
#undef TENSOR_FUNC_TN_
#undef TENSOR_FUNC_TN
#undef TENSOR_FUNC
#undef TENSOR_CLASS_TN_
#undef TENSOR_CLASS_TN
#undef TENSOR_CLASS


