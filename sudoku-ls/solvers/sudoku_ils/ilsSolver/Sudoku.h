/*
 * Sudoku.h
 *
 * Part of the Source Code for the Sudoku solver described in
 * "A Hybrid Heuristic Approach for the Sudoku Problem"
 */

#ifndef SUDOKU_H_
#define SUDOKU_H_

#include <vector>
#include <set>
#include <string>
#include <deque>
#include <utility>

class Sudoku {

	/**
	 * This struct is used to store information of a single field in a sudoku field
	 */
	struct field {
		int value;
		bool fixed;
	};

private:
	int order;
	/** The number of rows and columns of the sudokugrid */
	int numrows;

	std::vector< std::vector<field> > fields;

	/** The number of conflicts for each row are stored in this vector */
	std::vector<int> rowConflicts;
	/** The number of conflicts for each column are stored in this vector */
	std::vector<int> colConflicts;

	std::vector<std::pair<int, int> > unfixedIndices;

	std::set<int> referenceSet;

	int numberOfUnfixedCells;

public:
	static std::vector<Sudoku> createSudokus(std::string filename);

	Sudoku();
	Sudoku(int order);
	Sudoku(std::vector< std::vector<int> >, int order);

	virtual ~Sudoku();

	int setValue(int x, int y, int val, bool calculate = true);

	void lockValues();

	void setRowCost(int i);
	void setColCost(int i);

	int calculateColCost(int i);
	int calculateRowCost(int i);

	std::pair<int, int> getRandomlyConflictedField();
	std::vector< std::pair<int, int> > getConflictedFields();

	void clearSudoku();

	std::set<int> getRow(int i) const;
	std::set<int> getColumn(int i) const;
	std::set<int> getSquare(int i) const;

	int getOrder() const;
	int getNumrows() const;
	int getRowCost(int i) const;
	int getColCost(int i) const;
	int getValue(int x, int y) const;
	int getCost();
	bool isFixed(int x, int y);
	int calculateNumberOfUnfixedCells();
	int getNumberOfUnfixedCells() const;

	bool equals(Sudoku sudoku);
	std::string str() const;
	void applyCpProcedure();
	bool applyBtProcedure(bool minDomainFirst, std::string btHeuristic);

	std::vector<std::pair<int, int> > getUnfixedIndices();


	int printToFile(std::string filename);
};

#endif /* SUDOKU_H_ */
