#include <cassert>
#include <iostream>

inline int symbolToValue (char symbol)
{
	int value = symbol - '1';
	return value;
}

inline char valueToSymbol (int value)
{
	char symbol = value + '1';
	return symbol;
}

class Cell
{
public:
	Cell() = default;

	void set(char symbol)
	{
		int value = symbolToValue (symbol);
		assert((0 <= value) && (value < 9));
		m_isSolved = true;
		m_value = value;
		std::fill_n(m_isCandidate, 9, false);
	}

	int numCandidates() const
	{
		assert (!m_isSolved);
		int result = 0;
		for (auto &candidate : m_isCandidate)
		{
			if (candidate)
			{
				result++;
			}
		}
		assert ((0 <= result) && (result <= 9));
		return result;
	}

	int getSingleCandidate() const
	{
		assert (!m_isSolved);
		int result = -1;
		for (int i=0; i<9; i++)
		{
			if (!m_isCandidate[i])
			{
				continue;
			}
			assert (result == -1);
			result = i;
		}
		assert (result != -1);
		return result;
	}

	bool m_isSolved = false;
	int m_value = -1;
	bool m_isCandidate[9] = { true, true, true, true, true, true, true, true, true };
};

inline int makeIndex (int x, int y)
{
	assert ((0 <= x) && (x < 9));
	assert ((0 <= y) && (y < 9));
	return x + 9 * y;
}

inline void splitIndex (int idx, int &x, int &y)
{
	assert ((0 <= idx) && (idx < 81));
	x = idx % 9;
	y = idx / 9;
}

class Group
{
public:
	Group (int idx0, int idx1, int idx2, int idx3, int idx4, int idx5, int idx6, int idx7, int idx8);

	int m_idx[9] = { };
};

Group::Group (int idx0, int idx1, int idx2, int idx3, int idx4, int idx5, int idx6, int idx7, int idx8)
{
	m_idx[0] = idx0;
	m_idx[1] = idx1;
	m_idx[2] = idx2;
	m_idx[3] = idx3;
	m_idx[4] = idx4;
	m_idx[5] = idx5;
	m_idx[6] = idx6;
	m_idx[7] = idx7;
	m_idx[8] = idx8;
}

constexpr int NumGroups = 27;
const Group groups[NumGroups] =
{
	// rows
	{  0,  1,  2,  3,  4,  5,  6,  7,  8 },
	{  9, 10, 11, 12, 13, 14, 15, 16, 17 },
	{ 18, 19, 20, 21, 22, 23, 24, 25, 26 },
	{ 27, 28, 29, 30, 31, 32, 33, 34, 35 },
	{ 36, 37, 38, 39, 40, 41, 42, 43, 44 },
	{ 45, 46, 47, 48, 49, 50, 51, 52, 53 },
	{ 54, 55, 56, 57, 58, 59, 60, 61, 62 },
	{ 63, 64, 65, 66, 67, 68, 69, 70, 71 },
	{ 72, 73, 74, 75, 76, 77, 78, 79, 80 },

	// columns
	{  0,  9, 18, 27, 36, 45, 54, 63, 72 },
	{  1, 10, 19, 28, 37, 46, 55, 64, 73 },
	{  2, 11, 20, 29, 38, 47, 56, 65, 74 },
	{  3, 12, 21, 30, 39, 48, 57, 66, 75 },
	{  4, 13, 22, 31, 40, 49, 58, 67, 76 },
	{  5, 14, 23, 32, 41, 50, 59, 68, 77 },
	{  6, 15, 24, 33, 42, 51, 60, 69, 78 },
	{  7, 16, 25, 34, 43, 52, 61, 70, 79 },
	{  8, 17, 26, 35, 44, 53, 62, 71, 80 },

	// blocks
	{  0,  1,  2,  9, 10, 11, 18, 19, 20 },
	{  3,  4,  5, 12, 13, 14, 21, 22, 23 },
	{  6,  7,  8, 15, 16, 17, 24, 25, 26 },
	{ 27, 28, 29, 36, 37, 38, 45, 46, 47 },
	{ 30, 31, 32, 39, 40, 41, 48, 49, 50 },
	{ 33, 34, 35, 42, 43, 44, 51, 52, 53 },
	{ 54, 55, 56, 63, 64, 65, 72, 73, 74 },
	{ 57, 58, 59, 66, 67, 68, 75, 76, 77 },
	{ 60, 61, 52, 69, 70, 71, 78, 79, 80 },
};

class Board
{
public:
	Board() = default;

	void set(int x, int y, int value);
	void initCandidates();
	bool isSolved() const;

	void print(std::ostream &);
	void printCandidates(std::ostream &);

	Cell m_cells[81];
};

void Board::set(int x, int y, int value)
{
	assert ((0 <= x) && (x < 9));
	assert ((0 <= y) && (y < 9));
	int idx = makeIndex (x, y);
	Cell &cell (m_cells[idx]);
	assert (!cell.m_isSolved);
	cell.set (value);
	return;
}

void Board::initCandidates()
{
	for (auto &group : groups)
	{
		for (int idx : group.m_idx)
		{
			const Cell &cell (m_cells[idx]);
			if (!cell.m_isSolved)
			{
				continue;
			}
			for (int idx1 : group.m_idx)
			{
				Cell &cell1 (m_cells[idx1]);
				if (cell1.m_isSolved)
				{
					continue;
				}
				cell1.m_isCandidate[cell.m_value] = false;
			}
		}
	}
}

bool Board::isSolved() const
{
	for (const Cell &cell : m_cells)
	{
		if (!cell.m_isSolved)
		{
			return false;
		}
	}
	return true;
}

void Board::print(std::ostream &os)
{
	for (int y=0; y<9; y++)
	{
		for (int x=0; x<9; x++)
		{
			int idx = makeIndex (x, y);
			const Cell &cell (m_cells[idx]);
			char ch = cell.m_isSolved ? (valueToSymbol (cell.m_value)) : '.';
			char sep = (x + 1 == 9) ? '\n' : ' ';
			os << ch << sep;
		}
	}
}

void Board::printCandidates(std::ostream &os)
{
	for (int y=0; y<9; y++)
	{
		for (int s=0; s<3; s++)
		{
			for (int x=0; x<9; x++)
			{
				int idx = makeIndex (x, y);
				const Cell &cell (m_cells[idx]);
				for (int r=0; r<3; r++)
				{
					int idxc = r + 3 * s;
					char ch = '.';
					if (cell.m_isSolved)
					{
						ch = (idxc == 4) ? (valueToSymbol(cell.m_value)) : '#';
					}
					else if (cell.m_isCandidate[idxc])
					{
						ch = valueToSymbol(idxc);
					}
					char sep = ((x + 1 == 9) && (r + 1 == 3)) ? '\n' : ' ';
					os << ch << sep;
				}
				if (x + 1 < 9)
				{
					os << "  ";
				}
			}
		}
		os << '\n';
	}
}

void setBoard20250215 (Board &board)
{
	board.set (3, 0, '6');
	board.set (0, 1, '4');
	board.set (2, 1, '6');
	board.set (7, 1, '7');
	board.set (0, 2, '3');
	board.set (4, 2, '7');
	board.set (5, 2, '9');
	board.set (7, 2, '8');
	board.set (1, 3, '6');
	board.set (4, 3, '3');
	board.set (6, 3, '2');
	board.set (7, 3, '5');
	board.set (8, 3, '4');
	board.set (3, 4, '9');
	board.set (5, 4, '2');
	board.set (0, 5, '2');
	board.set (1, 5, '5');
	board.set (2, 5, '7');
	board.set (4, 5, '6');
	board.set (7, 5, '3');
	board.set (1, 6, '2');
	board.set (3, 6, '7');
	board.set (4, 6, '9');
	board.set (8, 6, '5');
	board.set (1, 7, '4');
	board.set (6, 7, '1');
	board.set (8, 7, '2');
	board.set (5, 8, '1');
}

int solve (Board &board, bool singleStep, std::ostream *log)
{
	int numChanges = 0;
	for (Cell &cell : board.m_cells)
	{
		if (cell.m_isSolved)
		{
			continue;
		}
		int num = cell.numCandidates();
		if (num != 1)
		{
			continue;
		}
		int value = cell.getSingleCandidate();
		char symbol = valueToSymbol(value);
		if (log != nullptr)
		{
			int idx = static_cast<int>(&cell - &board.m_cells[0]);
			int x = 0, y = 0;
			splitIndex (idx, x, y);
			*log << "Cell(" << x << "," << y << ") = " << symbol << "  - single candidate\n";
		}
		cell.set (symbol);
		numChanges++;
		if (singleStep)
		{
			return numChanges;
		}
	}

	for (const Group &group : groups)
	{
		for (int value=0; value<9; value++)
		{
			bool isSolved = false;
			int numCandidates = 0;
			int idxFirstCandidate = -1;
			for (int idx : group.m_idx)
			{
				const Cell &cell (board.m_cells[idx]);
				if (cell.m_isSolved)
				{
					if (cell.m_value == value)
					{
						isSolved = true;
						break;
					}
				}
				if (cell.m_isCandidate[value])
				{
					numCandidates++;
					if (idxFirstCandidate < 0)
					{
						idxFirstCandidate = idx;
					}
				}
			}
			if (isSolved)
			{
				continue;
			}
			if (numCandidates == 1)
			{
				assert (idxFirstCandidate >= 0);
				char symbol = valueToSymbol(value);
				if (log != nullptr)
				{
					int x = 0, y = 0;
					splitIndex (idxFirstCandidate, x, y);
					*log << "Cell(" << x << "," << y << ") = " << symbol << "  - only candidate for value\n";
				}
				Cell &cell (board.m_cells[idxFirstCandidate]);
				cell.set (symbol);
				numChanges++;
		if(singleStep)
		{
			return numChanges;
	}
	}
		}
	}

	return numChanges;
}

int main()
{
	Board board;
	setBoard20250215 (board);
	board.initCandidates();
	//board.print (std::cout);
	board.printCandidates (std::cout);

	while (!board.isSolved())
	{
		int numChanges = solve(board, true, &std::cerr);
		if (numChanges == 0)
		{
			break;
		}
		board.initCandidates();
		board.printCandidates (std::cout);
	}
}

