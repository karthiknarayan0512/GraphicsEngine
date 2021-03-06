/*
	This class represents a position or direction
*/

#ifndef EAE6320_MATH_CVECTOR_H
#define EAE6320_MATH_CVECTOR_H

// Class Declaration
//==================

namespace eae6320
{
	namespace Math
	{
		class cVector
		{
			// Interface
			//==========

		public:

			// Addition
			cVector operator +( const cVector& i_rhs ) const;
			cVector& operator +=( const cVector& i_rhs );

			// Subtraction / Negation
			cVector operator -( const cVector& i_rhs ) const;
			cVector& operator -=( const cVector& i_rhs );
			cVector operator -() const;

			// Multiplication
			cVector operator *( const float i_rhs ) const;
			cVector& operator *=( const float i_rhs );

			// Division
			cVector operator /( const float i_rhs ) const;
			cVector& operator /=( const float i_rhs );

			// Length / Normalization
			float GetLength() const;
			float Normalize();
			cVector CreateNormalized() const;

			static cVector Lerp(const cVector& i_start, const cVector& i_end, const float& i_rate);

			// Products
			friend float Dot( const cVector& i_lhs, const cVector& i_rhs );
			friend cVector Cross( const cVector& i_lhs, const cVector& i_rhs );

			// Comparison
			bool operator ==( const cVector& i_rhs ) const;
			bool operator !=( const cVector& i_rhs ) const;

			// Initialization / Shut Down
			//---------------------------

			cVector( const float i_x = 0.0f, const float i_y = 0.0f, const float i_z = 0.0f );

			// Data
			//=====

		// A vector class is one of the very few examples where I make data public
		public:

			float x, y, z;

			// Friend Classes
			//===============

			friend class cMatrix_transformation;
			friend class cQuaternion;
		};

		// Friend Function Declarations
		//=============================

		float Dot( const cVector& i_lhs, const cVector& i_rhs );
		cVector Cross( const cVector& i_lhs, const cVector& i_rhs );
	}
}

eae6320::Math::cVector operator *(const eae6320::Math::cVector& i_lhs, const eae6320::Math::cMatrix_transformation i_rhs);


#endif	// EAE6320_MATH_CVECTOR_H
