/*
 * Sudoku.cpp
 *
 * Part of the Source Code for the Sudoku solver described in
 * "A Hybrid Heuristic Approach for the Sudoku Problem"
 */

#include "Sudoku.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Util.h"
#include "../cpProcedure/cpProcedure.h"
#include "../cpProcedure/btProcedure.h"

#include "Constants.h"



/**
 * Reads given textfile and creates Sudoku objects as specified in the file.
 */
std::vector<Sudoku> Sudoku::createSudokus(std::string filename) {
	std::ifstream input(filename.c_str());
	std::string line;
	int linenr = 0;
	std::string ref = "0123456789";
	std::vector<Sudoku> sudokus;

	while (std::getline(input, line)) {
		linenr++;

		if (ref.find(line[0]) == std::string::npos) {
			// Not a sudoku line (must start with a number)
			continue;
		}
		else {
			std::istringstream converter(line);
			int order;
			int numrows;

			converter >> order;

			if (converter.fail()) {
				std::cout << linenr << ": Invalid order. Reason: Invalid character." << std::endl;
				continue;
			}
			if (order < 1 || order > 9) {
				std::cout << linenr << ": Invalid order. Reason: Must be between 1 and 9." << std::endl;
				continue;
			}

			numrows = order*order;
			std::vector< std::vector<int> > values;

			for (int i=0; i<numrows; i++) {
				if (std::getline(input, line)) {
					linenr++;

					// Accept Sudokus from lewisGenerator
					if (i==0 && line.length() <= (numrows+numrows/2)) {
						i--;
						continue;
					}

					if (line.length() < (unsigned)numrows*2-1) {
						std::cout << linenr << ": Invalid Sudoku line. Reason: Line is too short. (length is " << line.length() << ")" << std::endl;
						break;
					}

					std::istringstream converter(line);
					std::vector<int> row_values;
					int value;

					for (int j=0; j<numrows; j++) {
						converter >> value;
						if (converter.fail()) {
							std::cout << linenr << ": Invalid character in row!" << std::endl;
							break;
						}
						if (value < 0) {
							value = 0;
						}
						row_values.push_back(value);
					}
					if (row_values.size() != (unsigned)numrows) {
						std::cout << linenr << ": Incomplete row!" << std::endl;
						break;
					}
					else {
						values.push_back(row_values);
					}
				}
			}
			if (values.size() != (unsigned)numrows) {
				std::cout << linenr << ": Incomplete Sudoku!" << std::endl;
			}
			else {
				sudokus.push_back(Sudoku(values, order));
			}
		}
	}

	return sudokus;
}

Sudoku::Sudoku() {
	order = 0;
	numrows = 0;
	numberOfUnfixedCells = -1;
}

/** Creates an empty sudoku of the given order. */
Sudoku::Sudoku(int order) : order(order), numrows(order*order), rowConflicts(numrows, -1), colConflicts(numrows, -1), numberOfUnfixedCells(-1) {
	for (int i=0; i<numrows; i++) {
		std::vector<field> rowvalues;

		referenceSet.insert(i+1);

		field f;
		f.value = 0;
		f.fixed = false;
		for (int j=0; j<numrows; j++) {
			rowvalues.push_back(f);
		}
		fields.push_back(rowvalues);
	}
}

Sudoku::~Sudoku() {

}

/** Creates a sudoku of the given order, prefilled with the given values */
Sudoku::Sudoku(std::vector< std::vector<int> > values, int order) : order(order), numrows(order*order), rowConflicts(numrows, -1), colConflicts(numrows, -1), numberOfUnfixedCells(-1) {
	for (int i=0; i<numrows; i++) {
		std::vector<field> fields;

		referenceSet.insert(i+1);


		for (int j=0; j<numrows; j++) {
			field f;
			f.value = values[i][j];
			f.fixed = false;
			fields.push_back(f);
		}
		this->fields.push_back(fields);
	}

	lockValues();
}

/**
 * Sets the value of a specified field.
 *
 * @param calculate if true, the row and column costs of the Sudoku will be recalculated
 *        after the field is set
 * @return 0 if the value was set successfully, -1 if illegal coordinates were given
 *         and -2 if the chosen field is locked
 */
int Sudoku::setValue(int x, int y, int val, bool calculate) {
		fields[x][y].value = val;
		if (calculate) {
			setRowCost(x);
			setColCost(y);
		}
	    return 0;
}

/** Lock all values which have been set yet */
void Sudoku::lockValues() {
	unfixedIndices.clear();
	for (int i=0; i<numrows; i++) {
		for (int j=0; j<numrows; j++) {
			if (fields[i][j].value != 0) {
				fields[i][j].fixed = true;
			}
			else {
				unfixedIndices.push_back(std::pair<int, int>(i, j));
			}
		}
	}
	this->numberOfUnfixedCells = calculateNumberOfUnfixedCells();
}

void Sudoku::setRowCost(int i) {
	rowConflicts[i] = calculateRowCost(i);
}

void Sudoku::setColCost(int i) {
	colConflicts[i] = calculateColCost(i);
}

std::vector<std::pair<int, int> > Sudoku::getUnfixedIndices() {
	return unfixedIndices;
}

/** Calculates the cost of a row by determining the number of conflicts in this row. */
int Sudoku::calculateRowCost(int i) {
	std::set<int> rowset;
	std::vector<int> result;

	rowset = getRow(i);

	std::set_difference(referenceSet.begin(), referenceSet.end(), rowset.begin(), rowset.end(), std::inserter(result, result.end()));

	return result.size();
}

/** Calculates the cost of a column by determining the number of conflicts in this row. */
int Sudoku::calculateColCost(int i) {
	std::set<int> colset;
	std::vector<int> result;

	colset = getColumn(i);

	std::set_difference(referenceSet.begin(), referenceSet.end(), colset.begin(), colset.end(), std::inserter(result, result.end()));

	return result.size();
}

/**
 * @return The coordinates of a field which is causing a conflict.
 *         If multiple conflicting fields exists, one of them is chosen randomly.
 */
std::pair<int, int> Sudoku::getRandomlyConflictedField() {
	std::vector<std::pair<int, int> > conflictedFields = getConflictedFields();

	int random = Util::randomInt(0, conflictedFields.size()-1);

	return conflictedFields[random];
}

std::vector< std::pair<int, int> > Sudoku::getConflictedFields() {
	std::set< std::pair<int, int> > conflictedCells;
	std::vector<int> rowSet, colSet, possibleValues;

	for (int i=0; i<numrows; i++) {
		if (rowConflicts[i] > 0) {
			rowSet.clear();
			possibleValues.clear();

			for (int j=0; j<numrows; j++) {
				rowSet.push_back(fields[i][j].value);
			}

			std::sort(rowSet.begin(), rowSet.end());
			std::set_difference(rowSet.begin(), rowSet.end(), referenceSet.begin(), referenceSet.end(), std::inserter(possibleValues, possibleValues.end()));

			for (int j=0; j<numrows; j++) {
				if (!fields[i][j].fixed && std::find(possibleValues.begin(), possibleValues.end(), fields[i][j].value) != possibleValues.end()) {
					conflictedCells.insert(std::pair<int,int>(i, j));
				}
			}
		}
	}

	for (int i=0; i<numrows; i++) {
		if (colConflicts[i] > 0) {
			colSet.clear();
			possibleValues.clear();

			for (int j=0; j<numrows; j++) {
				colSet.push_back(fields[j][i].value);
			}

			std::sort(colSet.begin(), colSet.end());
			std::set_difference(colSet.begin(), colSet.end(), referenceSet.begin(), referenceSet.end(), std::inserter(possibleValues, possibleValues.end()));

			for (int j=0; j<numrows; j++) {
				if (!fields[j][i].fixed && std::find(possibleValues.begin(), possibleValues.end(), fields[j][i].value) != possibleValues.end()) {
					conflictedCells.insert(std::pair<int,int>(j, i));
				}
			}
		}
	}


	std::vector<std::pair<int, int> > v( conflictedCells.begin(), conflictedCells.end() );
	return v;
}


/**
 * Clears the values of all unfixed cells.
 */
void Sudoku::clearSudoku() {
	for (int i=0; i<numrows; i++) {
		for (int j=0; j<numrows; j++) {
			if (!this->isFixed(i, j)) {
				this->setValue(i, j, 0, true);
			}
		}
	}

}

/** Creates a printable version of the Sudoku. Fixed values are prefixed by a !. */
std::string Sudoku::str() const {
	std::ostringstream output;

	for (int i=0; i<numrows; i++) {
		if (i%order == 0) {
			for (int k=0; k<order; k++) {
				output << "+";
				for (int l=0; l<order; l++) {
					if (order>3) {
						output << "-";
					}
					output << "--";
				}
				for (int l=0; l<order-1; l++) {
					output << "-";
				}
			}
			output << "+\n";
		}
		for (int j=0; j<numrows; j++) {
			if (j%order == 0) {
				output << "|";
			}

			if (fields[i][j].fixed) {
				output << "!";
			}
			else {
				output << " ";
			}

			if (order > 3 &&  fields[i][j].value < 10) {
				output << "0";
			}

			output << fields[i][j].value;

			if (j%order != order-1) {
				output << " ";
			}
		}
		output << "|\n";
	}

	// last line
	for (int k=0; k<order; k++) {
		output << "+";
		for (int l=0; l<order; l++) {
			if (order>3) {
				output << "-";
			}
			output << "--";
		}
		for (int l=0; l<order-1; l++) {
			output << "-";
		}
	}
	output << "+\n";

	return output.str();
}

/** @return A set which contains all values of a row (without duplicates). */
std::set<int> Sudoku::getRow(int i) const {
	std::set<int> set;

	if (i >= 0 && i < numrows) {
		for (int j=0; j<numrows; j++) {
			set.insert(fields[i][j].value);
		}
	}

	return set;
}

/** @return A set which contains all values of a column (without duplicates). */
std::set<int> Sudoku::getColumn(int i) const {
	std::set<int> set;

	if (i >= 0 && i < numrows) {
		for (int j=0; j<numrows; j++) {
			set.insert(fields[j][i].value);
		}
	}

	return set;
}

/** @return A set which contains all values of a square (without duplicates). */
std::set<int> Sudoku::getSquare(int i) const {
	std::set<int> set;

	if (i >= 0 && i < numrows) {
		int rowstart = (i/order)*order;
		int colstart = (i%order)*order;

		for (int j=rowstart; j<rowstart+order; j++) {
			for (int k=colstart; k<colstart+order; k++) {
				set.insert(fields[j][k].value);
			}
		}
	}
	return set;
}

int Sudoku::getOrder() const {
	return order;
}

int Sudoku::getNumrows() const {
	return numrows;
}


int Sudoku::getCost(){
	int cost = 0;

	for (int i=0; i<numrows; i++) {
		cost += rowConflicts[i];
	}

	for (int i=0; i<numrows; i++) {
		cost += colConflicts[i];
	}

	return cost;
}

int Sudoku::getValue(int x, int y) const {
	return fields[x][y].value;
}

bool Sudoku::isFixed(int x, int y) {
	if (x < 0 || x >= numrows || y < 0 || y >= numrows)
		return false;
	else
		return fields[x][y].fixed;
}

bool Sudoku::equals(Sudoku sudoku) {
	if (this->order != sudoku.getOrder()) {
		return false;
	}

	for (int i=0; i<numrows; i++) {
		for (int j=0; j<numrows; j++) {
			if (this->fields[i][j].value != sudoku.getValue(i,j)) {
				return false;
			}
			if (this->fields[i][j].fixed != sudoku.isFixed(i,j)) {
				return false;
			}
		}
	}

	return true;
}

int Sudoku::calculateNumberOfUnfixedCells() {

	int sum = 0;

	for (int i=0; i<numrows; i++) {
		for (int j=0; j<numrows; j++) {
			if (!this->fields[i][j].fixed) {
				sum++;
			}
		}
	}
	this->numberOfUnfixedCells = sum;

	return this->numberOfUnfixedCells;
}

int Sudoku::getNumberOfUnfixedCells() const {
	return numberOfUnfixedCells;
}

int Sudoku::printToFile(std::string filename) {
	std::ofstream myfile;
	myfile.open(filename.c_str());
	myfile << order << "\n";
	myfile << "1\n";
	for (int i=0; i<numrows; i++) {
		for (int j=0; j<numrows; j++) {
			myfile << this->fields[i][j].value << " ";
		}
		myfile << "\n";
	}
	myfile.close();
	return 0;
}

void Sudoku::applyCpProcedure() {
	int grLen = getNumrows();

	// Copy Sudoku object into grid arrays.
	int **grid = new int* [grLen];
	for(int j=0;j<grLen;j++) grid[j] = new int [grLen];
	bool **fixed = new bool* [grLen];
	for(int j=0;j<grLen;j++) fixed[j] = new bool [grLen];

	for(int r=0;r<grLen;r++){
		for(int c=0;c<grLen;c++){
			grid[r][c]=getValue(r, c);
			fixed[r][c]=isFixed(r, c);
		}
	}

	// do cpProcedure
	cpProcedure(grid, fixed, getOrder(), grLen,
			getOrder(), 1, grLen+1);

	// Copy grid values back into sudoku
	for(int r=0;r<grLen;r++){
		for(int c=0;c<grLen;c++){
			setValue(r, c, grid[r][c], false);
		}
	}

	// Calculate row/colcosts
	for(int r=0;r<grLen;r++){
		setRowCost(r);
		setColCost(r);
	}

	// delete  grid
	for(int r=0;r<grLen;r++){
		delete [] grid[r];
		delete [] fixed[r];
	}
	delete [] grid;
	delete [] fixed;
}

bool Sudoku::applyBtProcedure(bool minDomainFirst, std::string btHeuristic) {
	int grLen = getNumrows();

		// Copy Sudoku object into grid arrays.
		int **grid = new int* [grLen];
		for(int j=0;j<grLen;j++) grid[j] = new int [grLen];
		bool **fixed = new bool* [grLen];
		for(int j=0;j<grLen;j++) fixed[j] = new bool [grLen];

		for(int r=0;r<grLen;r++){
			for(int c=0;c<grLen;c++){
				grid[r][c]=getValue(r, c);
				if (getValue(r,c) == 0) {
					fixed[r][c]=false;
				}
				else {
					fixed[r][c]=true;
				}
			}
		}

		// do btProcedure
		bool success;
		success = btProcedure(grid, fixed, getOrder(), grLen, getOrder(), 1, grLen+1, minDomainFirst, btHeuristic, clock()+(BT_TIMELIMIT*CLOCKS_PER_SEC));

		// Copy grid values back into sudoku
		for(int r=0;r<grLen;r++){
			for(int c=0;c<grLen;c++){
				setValue(r, c, grid[r][c], false);
			}
		}

		// Calculate row/colcosts
		for(int r=0;r<grLen;r++){
			setRowCost(r);
			setColCost(r);
		}

		// delete  grid
		for(int r=0;r<grLen;r++){
			delete [] grid[r];
			delete [] fixed[r];
		}
		delete [] grid;
		delete [] fixed;

		return success;
}

int Sudoku::getRowCost(int i) const {
	return rowConflicts[i];
}
int Sudoku::getColCost(int i) const {
	return colConflicts[i];
}
