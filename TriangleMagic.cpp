#include "TriangleMagic.h"


TriangleMagic::TriangleMagic()
{

}

int TriangleMagic::RowOnPascals(int num)
{
	int start = 0;
	int count = 0;

	while (start < num)
	{
		start += count + 1;
		count++;
	}

	return count;
}


int TriangleMagic::PascalsAmountAboveRow(int num)
{
	int start = 0;
	int count = 0;

	while (start < num)
	{
		start += count + 1;
		count++;
	}

	return start - num;
}