Here’s an extension of the README following your provided format:

---

# Chess Database Project

INFOH417 - Database Systems Architecture (2023/24) - ULB <br />

<div align="center">
    <img src="https://actus.ulb.be/medias/photo/logo-universite-libre-bruxelles_1661952138925-png?ID_FICHE=19524" alt="ULB Logo" width="300"/>
</div>

<br>

This project aims to develop a PostgreSQL extension for storing and retrieving chess games, utilizing data types and functions based on common chess notations (PGN, SAN, FEN). The extension will support efficient queries on chess game data with custom indexing.

## Project Overview
The extension enables efficient storage and retrieval of chess game data using the following notations:
- **PGN (Portable Game Notation):** Records entire chess games.
- **SAN (Standard Algebraic Notation):** Encodes the moves.
- **FEN (Forsyth-Edwards Notation):** Represents specific board states.

## Key Features
- **Data Types:**
  - **`chessgame`**: Represents a full chess game using SAN notation.
  - **`chessboard`**: Stores a particular board state using FEN notation.

- **Functions:**
  - **`getBoard(chessgame, integer)`**: Returns the board state (`chessboard`) at a specified half-move.
  - **`getFirstMoves(chessgame, integer)`**: Retrieves the first `N` half-moves of a chess game.
  - **`hasOpening(chessgame, chessgame)`**: Checks if a `chessgame` starts with the moves of another provided game.
  - **`hasBoard(chessgame, chessboard, integer)`**: Verifies if a board state exists within the first `N` half-moves of a game.

- **Indexing:**
  - **`hasOpening` Index**: Uses a B-tree structure for comparing game openings, treating `chessgame` as a string.
  - **`hasBoard` Index**: Extends the GIN index to allow indexing of `chessgame` based on board states.

## Technical Details
- **Language**: C with [smallchesslib](https://codeberg.org/drummyfish/smallchesslib) for chess operations.
- **Input/Output**:
  - Uses SAN notation for game moves.
  - Uses FEN notation for representing board states.

## Setup & Usage
### Prerequisites
- PostgreSQL (version X.X or later)
- C Compiler (e.g., GCC)

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/chess-db-extension.git
   cd chess-db-extension
   ```

2. Build the extension:
   ```bash
   make
   ```

3. Install the extension in PostgreSQL:
   ```sql
   CREATE EXTENSION chess_db;
   ```

### Example Queries
- Retrieve the initial board state of a game:
  ```sql
  SELECT getBoard(game, 0) FROM chessgames WHERE id = 1;
  ```
- Find games starting with a specific opening:
  ```sql
  SELECT * FROM chessgames WHERE hasOpening(game, '1. e4 e5 2. Nf3 Nc6');
  ```
- Count games with a specific board state at any point:
  ```sql
  SELECT count(*) FROM chessgames WHERE hasBoard(game, 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1', 10);
  ```
---
You can find more commands from the text file which is provided in the same repo.
You can extend this as needed and copy it directly into your GitHub repository.
