#ifndef __TRIANGLEMAGIC_H_
#define __TRIANGLEMAGIC_H_

class TriangleMagic 
{
private:


public:
	TriangleMagic();

	//inputs a number and outputs which row that index is on Pascal's Triangle
	int RowOnPascals(int num);

	//inputs a number and outputs how much larger it is than the start of the row
	int PascalsAmountAboveRow(int num);
};

#endif //__MYBOMANAGER_H_