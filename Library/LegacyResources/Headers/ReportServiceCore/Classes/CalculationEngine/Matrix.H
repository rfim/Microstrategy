#include "Base/Defines/BasicTypes.h"

typedef Int32 Index;
typedef double Real;


class Permutation
{
private:
	Index *buf;		// stores specification of the permutation

public:
	const Index dim;

	Permutation(Index);
	Permutation(Permutation &);			// copy constructor allocates a new buffer
	~Permutation();

	Index & operator [] (Index n);		// get P(n)
	Index & operator () (Index n);		// get P(n)

	Permutation & AddCycle(Index, Index);	// add a cycle
};



class Matrix
{
private:
	Real *buf;			// stores the data contained in the matrix
	bool destroiable;	// set to false if buf was provided to the constructor

public:
	const Index height, width;

	Matrix(Index, Index);			// This creator allocates memory for the Matrix
	Matrix(Index, Index, Real *);	// This creator uses the given buffer. The destroier does not 
									// deallocate the buffer
	Matrix(Matrix &);				// copy constructor allocates a new buffer
	~Matrix();

	Real & operator () (Index i, Index j);	// returns element i,j 
	Matrix & operator = (Matrix &);

	Matrix & Permute(Matrix &A, Permutation &rowP, Permutation &colP);
	Matrix & PermuteBack(Matrix &A, Permutation &rowP, Permutation &colP);
};





class Vector
{
private:
	Real *buf;			// stores the data contained in the vector
	bool destroiable;	// set to false if buf was provided to the constructor

public:
	const Index dim;

	Vector(Index);			// This creator allocates memory for the vector
	Vector(Index, Real *);	// This creator uses the given buffer. The destroier does not 
							// deallocate the buffer
	Vector(Vector &);		// copy constructor allocates a new buffer
	~Vector();


	Real & operator [] (Index);
	Real & operator () (Index);
	Vector & operator += (Vector &v) { return Sum(*this, v); }
	Vector & operator -= (Vector &v) { return Diff(*this, v); }
	Real operator * (Vector &v);
	Vector & operator =	(Vector &v);
	Vector & operator *= (Real x) {return RealMult(*this, x); }
	Vector & operator /= (Real x) {return RealDiv(*this, x);}

	Vector & Permute(Vector &v, Permutation &P);	// set this vector to P(v)(if dimensions agree)
	Vector & PermuteBack(Vector &v, Permutation &p);// set this vector to P'(v) (if dimensions agree)
	Vector & MatrixMult(Matrix &A, Vector &v);		// set this vector to Av (if dimensions agree)
	Vector & Sum(Vector &, Vector &);
	Vector & Diff(Vector &, Vector &);
	Vector & RealMult(Vector &, Real);
	Vector & RealDiv(Vector &, Real);
};





inline Permutation::Permutation(Index d): dim(d)
{
	buf = new Index[d];

	for (Index i=0; i<d; i++)
		buf[i] = i;
}

inline Permutation::Permutation(Permutation &X): dim(X.dim)
{
	buf = new Index[dim];

	for (Index i=0; i<dim; i++)
		buf[i] = X(i);
}

inline Permutation::~Permutation()
{
	delete buf;
}

inline Index & Permutation::operator [] (Index n)
{
	return buf[n];
}

inline Index & Permutation::operator () (Index n)
{
	return buf[n];
}

inline Permutation & Permutation::AddCycle(Index i, Index j)
{
	Index temp;
	
	temp=buf[i];
	buf[i]=buf[j];
	buf[j]=temp;

	return *this;
}

inline Matrix::Matrix(Index h, Index w): height(h), width(w), destroiable(true)
{
	buf = new Real[h*w];
}

inline Matrix::Matrix(Index h, Index w, Real *M): height(h), width(w), destroiable(false), buf(M)
{}

inline Matrix::Matrix(Matrix &X): height(X.height), width(X.width), destroiable(true)
 {
	 buf = new Real[height*width];
	 for (Index i=0; i < height*width; i++)
		 buf[i] = X.buf[i];
 }

inline Matrix::~Matrix()
{
	if (destroiable) delete buf;
}

inline Real & Matrix::operator () (Index i, Index j)
{
	return buf[i*width+j];
}

inline Matrix & Matrix::operator = (Matrix &A)
{
	if ( A.height == height && A.width == width )
	{
		for (Index i=0; i<height*width; i++)
		{
			buf[i] = A.buf[i];
		}
	}

	return A;
}

inline Matrix & Matrix::Permute(Matrix &A, Permutation &rowP, Permutation &colP)
{
	if ( A.height == height && A.width == width )
	{
		Index i,j;

		for (i=0; i<height; i++)
			for (j=0; j<width; j++)
				(*this)(i, j) = A(rowP(i), colP(j));
	}

	return *this;
}

inline Matrix & Matrix::PermuteBack(Matrix &A, Permutation &rowP, Permutation &colP)
{
	if ( A.height == height && A.width == width )
	{
		Index i,j;

		for (i=0; i<height; i++)
			for (j=0; j<width; j++)
				(*this)(rowP(i), colP(j)) = A(i, j);
	}

	return *this;
}



inline Vector::Vector(Index d): dim(d), destroiable(true)
{
	buf = new Real[d];
}

inline Vector::Vector(Index d, Real *v): dim(d), destroiable(false), buf(v)
{}

inline Vector::Vector(Vector &X): dim(X.dim), destroiable(true)
 {
	 buf = new Real[dim];
	 for (Index i=0; i<dim; i++)
		 buf[i] = X.buf[i];
 }

inline Vector::~Vector()
{
	if (destroiable)
		delete buf;
}

inline Real & Vector::operator [] (Index i)
{
	return buf[i];
}

inline Real & Vector::operator () (Index i)
{
	return buf[i];
}

inline Real Vector::operator * (Vector &v)
{
	Real result=0;

	if ( dim == v.dim )
	{
		for (Index i=0; i<dim; i++)
			result += buf[i] * v.buf[i];
	}

	return result;
}

inline Vector & Vector::operator = (Vector &v)
{
	if ( dim == v.dim )
	{
		for (Index i=0; i<dim; i++)
			buf[i] = v.buf[i];
	}

	return v;
}

inline Vector & Vector::Permute(Vector &v, Permutation &P)
{
	if ( dim == v.dim && dim == P.dim )
	{
		for (Index i=0; i<dim; i++)
			buf[i] = v.buf[P[i]];
	}

	return *this;
}

inline Vector & Vector::PermuteBack(Vector &v, Permutation &P)
{
	if ( dim == v.dim && dim == P.dim )
	{
		for (Index i=0; i<dim; i++)
			buf[P[i]] = v.buf[i];
	}

	return *this;
}

inline Vector & Vector::MatrixMult(Matrix &A, Vector &v)
{
	if ( dim == A.height && v.dim == A.width )
	{
		Index i,j;
		for (i=0; i<A.height; i++)
		{
			buf[i] = 0;

			for (j=0; j<A.width; j++)
				buf[i] += A(i,j) * v.buf[j];
		}
	}

	return *this;
}

inline Vector & Vector::Sum(Vector &v, Vector &w)
{
	if ( dim == v.dim && dim == w.dim )
	{
		for (Index i=0; i<dim; i++)
			buf[i] = v.buf[i] + w.buf[i];
	}

	return *this;
}

inline Vector & Vector::Diff(Vector &v, Vector &w)
{
	if ( dim == v.dim && dim == w.dim )
	{
		for (Index i=0; i<dim; i++)
			buf[i] = v.buf[i] - w.buf[i];
	}

	return *this;
}

inline Vector & Vector::RealMult(Vector &v, Real x)
{
	 	if ( dim == v.dim )
	{
		for (Index i=0; i<dim; i++)
			buf[i] = v.buf[i] * x;
	}

	return *this;
}

inline Vector & Vector::RealDiv(Vector &v, Real x)
{
	 	if ( dim == v.dim )
	{
		for (Index i=0; i<dim; i++)
			buf[i] = v.buf[i] / x;
	}

	return *this;
}