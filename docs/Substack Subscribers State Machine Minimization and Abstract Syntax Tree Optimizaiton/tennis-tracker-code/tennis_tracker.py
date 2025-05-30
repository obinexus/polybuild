import argparse
import csv
from enum import Enum
from dataclasses import dataclass
from typing import List, Dict, Optional
from datetime import datetime
import os

class Score(Enum):
    LOVE = 0
    FIFTEEN = 15
    THIRTY = 30
    FORTY_FIVE = 40
    GAME = 1

@dataclass
class Player:
    name: str
    current_score: Score = Score.LOVE
    games_won: int = 0

class TennisTrackerBase:
    def save_to_csv(self, filename: str):
        """Save game history to CSV file"""
        os.makedirs('game_history', exist_ok=True)
        timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
        filepath = os.path.join('game_history', f'{filename}_{timestamp}.csv')
        
        with open(filepath, 'w', newline='') as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow(['Point', 'Player1', 'Score1', 'Player2', 'Score2', 'Games Won P1', 'Games Won P2'])
            
            for i, state in enumerate(self.history):
                row_data = [f"Point {i+1}"]
                row_data.extend(self._get_csv_row_data(state))
                writer.writerow(row_data)
        
        print(f"\nGame history saved to: {filepath}")

class TennisTrackerA(TennisTrackerBase):
    """
    Program A: Tracks every state change for both players
    """
    def __init__(self, player1: str, player2: str):
        self.player1 = Player(player1)
        self.player2 = Player(player2)
        self.score_sequence = {
            Score.LOVE: Score.FIFTEEN,
            Score.FIFTEEN: Score.THIRTY,
            Score.THIRTY: Score.FORTY_FIVE,
            Score.FORTY_FIVE: Score.GAME
        }
        self.history: List[Dict[str, Score]] = []
    
    def score_point(self, scoring_player: str):
        player = self.player1 if scoring_player == self.player1.name else self.player2
        other_player = self.player2 if scoring_player == self.player1.name else self.player1
        
        if player.current_score == Score.FORTY_FIVE:
            player.current_score = Score.GAME
            player.games_won += 1
            self.record_state()
            self._reset_scores()
        else:
            player.current_score = self.score_sequence[player.current_score]
            self.record_state()
    
    def record_state(self):
        """Record the current state of both players"""
        self.history.append({
            self.player1.name: self.player1.current_score,
            self.player2.name: self.player2.current_score
        })
    
    def _reset_scores(self):
        self.player1.current_score = Score.LOVE
        self.player2.current_score = Score.LOVE
    
    def get_game_status(self) -> str:
        return f"Games won - {self.player1.name}: {self.player1.games_won}, {self.player2.name}: {self.player2.games_won}"
    
    def print_history(self):
        print("\nGame History (Program A):")
        for i, state in enumerate(self.history):
            print(f"Point {i+1}: {self.player1.name}: {state[self.player1.name].name}, "
                  f"{self.player2.name}: {state[self.player2.name].name}")
    
    def _get_csv_row_data(self, state):
        return [
            self.player1.name,
            state[self.player1.name].name,
            self.player2.name,
            state[self.player2.name].name,
            self.player1.games_won,
            self.player2.games_won
        ]

class TennisTrackerB(TennisTrackerBase):
    """
    Program B: Optimized version that only tracks necessary state changes
    """
    def __init__(self, player1: str, player2: str):
        self.player1 = Player(player1)
        self.player2 = Player(player2)
        self.score_sequence = {
            Score.LOVE: Score.FIFTEEN,
            Score.FIFTEEN: Score.THIRTY,
            Score.THIRTY: Score.FORTY_FIVE,
            Score.FORTY_FIVE: Score.GAME
        }
        self.history: List[Dict[str, Score]] = []
    
    def score_point(self, scoring_player: str):
        player = self.player1 if scoring_player == self.player1.name else self.player2
        
        if player.current_score == Score.FORTY_FIVE:
            player.current_score = Score.GAME
            player.games_won += 1
            self.record_state(player)
            self._reset_scores()
        else:
            player.current_score = self.score_sequence[player.current_score]
            self.record_state(player)
    
    def record_state(self, scoring_player: Player):
        """Record only the scoring player's state"""
        self.history.append({
            scoring_player.name: scoring_player.current_score
        })
    
    def _reset_scores(self):
        self.player1.current_score = Score.LOVE
        self.player2.current_score = Score.LOVE
    
    def get_game_status(self) -> str:
        return f"Games won - {self.player1.name}: {self.player1.games_won}, {self.player2.name}: {self.player2.games_won}"
    
    def print_history(self):
        print("\nGame History (Program B - Optimized):")
        for i, state in enumerate(self.history):
            player_name = list(state.keys())[0]
            print(f"Point {i+1}: {player_name}: {state[player_name].name}")
    
    def _get_csv_row_data(self, state):
        player_name = list(state.keys())[0]
        if player_name == self.player1.name:
            return [
                self.player1.name,
                state[player_name].name,
                self.player2.name,
                "LOVE",  # Non-scoring player stays at LOVE
                self.player1.games_won,
                self.player2.games_won
            ]
        else:
            return [
                self.player1.name,
                "LOVE",  # Non-scoring player stays at LOVE
                self.player2.name,
                state[player_name].name,
                self.player1.games_won,
                self.player2.games_won
            ]

def interactive_mode(tracker):
    """Handle interactive scoring input from users"""
    print("\nInteractive Tennis Score Tracker")
    print("--------------------------------")
    print(f"Player 1: {tracker.player1.name}")
    print(f"Player 2: {tracker.player2.name}")
    
    while True:
        print("\nCurrent Status:")
        print(tracker.get_game_status())
        print(f"\nCurrent Score - {tracker.player1.name}: {tracker.player1.current_score.name}, "
              f"{tracker.player2.name}: {tracker.player2.current_score.name}")
        
        command = input("\nEnter command (1: Point for Player 1, 2: Point for Player 2, "
                       "h: Show History, s: Save to CSV, q: Quit): ").lower()
        
        if command == 'q':
            break
        elif command == 'h':
            tracker.print_history()
        elif command == 's':
            filename = input("Enter base filename for CSV (default: 'tennis_match'): ").strip()
            if not filename:
                filename = 'tennis_match'
            tracker.save_to_csv(filename)
        elif command in ('1', '2'):
            scoring_player = tracker.player1.name if command == '1' else tracker.player2.name
            tracker.score_point(scoring_player)
        else:
            print("Invalid command. Please try again.")

def main():
    parser = argparse.ArgumentParser(description='Tennis Score Tracker')
    parser.add_argument('--tracker', type=str, choices=['A', 'B'], required=True,
                       help='Choose tracker type: A (full state) or B (optimized)')
    parser.add_argument('--player1', type=str, required=True, help='Name of player 1')
    parser.add_argument('--player2', type=str, required=True, help='Name of player 2')
    parser.add_argument('--games', type=int, default=5, help='Number of games to play (default: 5)')
    parser.add_argument('--interactive', action='store_true', help='Enable interactive mode')
    
    args = parser.parse_args()
    
    tracker_class = TennisTrackerA if args.tracker == 'A' else TennisTrackerB
    tracker = tracker_class(args.player1, args.player2)
    
    if args.interactive:
        interactive_mode(tracker)
    else:
        # Simulate the specified number of games
        for game in range(args.games):
            print(f"\nGame {game + 1}:")
            for _ in range(4):
                tracker.score_point(args.player1)
        
        print("\nFinal Results:")
        print(tracker.get_game_status())
        tracker.print_history()

if __name__ == "__main__":
    main()