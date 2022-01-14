Where most of source content came from:
https://github.com/akalicki/matrix

All credit to this hero

Changes made were done for ease of creating matrices and understanding content.
If you have any problems with the class, just message me on teams.

How to use Matrix Class:

MAJOR WARNING:
	THERE IS NOT MUCH ERROR CHECKING. 
	INVERSE DOES NOT CHECK FOR DETERMINANT (!= 0) SO YOU CAN GET INVERSE OF NON-INVERTIBLE FUNCTION

	Currently trying to improve this but 
	should not be a major issue as all content should be invertible in the first place.

	THERE WILL BE MANY ERRORS BUT BASICS ALL WORK.
	If you find any math errors, let me know and I will head back to fixing, don't feel bad about sending a lot.
	Just trying to get out working preliminary so that we can test content.

WARNING DONE

	Creating matrix:
		There is a no-args constructor, for a 1x1 matrix.
		There is a copy matrix constructor (passing matrix address as reference)

		To create a matrix with own input:
			Matrix(std::vector<std::vector<double>> content, int rows, int columns) is the constructor.
			Using nested std::vector, for example, for a 3x4 matrix (3 rows, 4 columns):

			std::vector<std::vector<double>> matrixContent = {{1,2,3}, {4,5,6}, {7,8,9}}
			Matrix* practiceMatrix = new Matrix(matrixContent, 3,4)


	Operators:
		All operators are accounted for, that I can think of.


	Functions:
		expHelper is recursive and can run for a long-time if the power is too large.
		Probably not the most efficient exponentation method.

		Inverse is being done through gaussian method, which means that it can happen for any square-sized matrix, not using 2x2 method or 3x3 method.

	Important things:

		Matrices are made as pointers. For example
		// Matrix* thisMatrix = new Matrix(3,4);
		This will create an empty matrix of 3 rows by 4 columns.
		However, to do actions on this, you must dereference.
		For example (adding a matrix):
		*thisMatrix += *otherMatrix;

		This includes cout:
		cout << *thisMatrix << '\n';
		This is because the constructors, and most '=' operators are returning the address
		enabling most of the changes to happen in-place and reduce memory usage.

		However, inverse returns a new matrix each time, for example, which may be annoying.
		Easy workaround would be creating a new matrix by copying and sending the inverse through that, if you want consistency 
		of matrix dereferencing.

		For example: 
			std::vector<std::vector<double>> squareList = { {1,2}, {5,6} };
			Matrix* squareMatrix = new Matrix(squareList, 2, 2);
			Matrix invertedMatrix = squareMatrix->inverse(); // Could be "Matrix invertedMatrix = *squareMatrix.inverse();" instead
			Matrix* copiedMatrix = new Matrix(invertedMatrix);


If you have any more questions, let me know.
