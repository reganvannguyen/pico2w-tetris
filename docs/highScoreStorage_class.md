HighScoreStorage
- loads and validates a top-three leaderboard from emulated EEPROM
- stores three-letter initials and scores in descending order
- saves only confirmed qualifying scores
- uses a version, magic value, and checksum to reject invalid data
- hides the flash/storage details from the Game class
