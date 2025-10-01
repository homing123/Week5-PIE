#include "pch.h"
#include "Vector.h"
const FMatrix FMatrix::ViewAxis = FMatrix
(
	0, 0, 1, 0,
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 0, 1
);
