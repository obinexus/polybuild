# Tennis Score Tracker

A Python-based tennis score tracking system that implements two different approaches to state machine management. This program provides both a complete state tracking system (Program A) and an optimized state tracking system (Program B), along with interactive gameplay and score history management capabilities.

## Features

The Tennis Score Tracker includes comprehensive features for managing tennis game scores:

State Machine Implementation: Two distinct tracking approaches:
- Program A tracks complete state changes for both players
- Program B provides an optimized version tracking only necessary state changes

Score Management:
- Standard tennis scoring progression (0→15→30→40→Game)
- Automatic game progression and score resetting
- Support for multiple games tracking

Interactive Mode:
- Real-time score input and tracking
- Command-line interface for game management
- History viewing during gameplay
- CSV export functionality

Data Export:
- CSV file generation with timestamp-based naming
- Comprehensive match statistics
- Historical game data preservation

## Installation

The Tennis Score Tracker requires Python 3.7 or higher. To set up the program:

```bash
# Clone the repository
git clone https://github.com/yourusername/tennis-tracker.git

# Navigate to the project directory
cd tennis-tracker

# Optional: Create and activate a virtual environment
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate
```

## Usage

The program can be run in either automatic or interactive mode with various configuration options.

### Basic Command Structure

```bash
python tennis_tracker.py --tracker <A/B> --player1 <name> --player2 <name> [--games <number>] [--interactive]
```

### Command-line Arguments

- `--tracker`: Choose tracking system (Required)
  - `A`: Full state tracking
  - `B`: Optimized tracking
- `--player1`: Name of first player (Required)
- `--player2`: Name of second player (Required)
- `--games`: Number of games to simulate (Optional, default: 5)
- `--interactive`: Enable interactive mode (Optional)

### Running in Automatic Mode

To simulate a set number of games automatically:

```bash
python tennis_tracker.py --tracker A --player1 "John" --player2 "Alice" --games 3
```

### Running in Interactive Mode

To start an interactive session:

```bash
python tennis_tracker.py --tracker B --player1 "John" --player2 "Alice" --interactive
```

### Interactive Mode Commands

During an interactive session, the following commands are available:

- `1`: Score a point for Player 1
- `2`: Score a point for Player 2
- `h`: Display match history
- `s`: Save current match to CSV file
- `q`: Quit the program

### CSV Export

When saving match data to CSV, files are stored in the `game_history` directory with the following format:
- Filename pattern: `<base_name>_YYYYMMDD_HHMMSS.csv`
- CSV columns: Point, Player1, Score1, Player2, Score2, Games Won P1, Games Won P2

Example CSV export command in interactive mode:
```bash
s
Enter base filename for CSV (default: 'tennis_match'): my_game
```

## Implementation Details

### Score Progression

The scoring system follows standard tennis rules:
- Love (0)
- 15
- 30
- 40
- Game Point (1)

### State Machine Types

Program A (Complete State Tracking):
- Records all state changes for both players
- Maintains complete history of score progression
- Suitable for detailed analysis and replay

Program B (Optimized State Tracking):
- Records only scoring player's state changes
- Minimizes memory usage and processing
- Ideal for basic score tracking

## Examples

### Example 1: Basic Automatic Game

```bash
python tennis_tracker.py --tracker A --player1 "John" --player2 "Alice" --games 1
```

### Example 2: Interactive Session with CSV Export

```bash
python tennis_tracker.py --tracker B --player1 "John" --player2 "Alice" --interactive
```

Interactive session commands:
```
1    # Score point for John
2    # Score point for Alice
h    # View history
s    # Save to CSV
q    # End session
```

## Development

The program is structured with object-oriented principles and includes:
- Enum-based score management
- Dataclass player representation
- Inheritance-based tracker implementations
- CSV data management
- Interactive command processing

## License

This project is licensed under the MIT License. See the LICENSE file for details.