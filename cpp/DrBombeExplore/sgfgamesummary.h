//
// Created by Haoda Fu on 5/17/21.
//

#ifndef KATAGO_DRBOMBEEXPLORE_SGFGAMESUMMARY_H_
#define KATAGO_DRBOMBEEXPLORE_SGFGAMESUMMARY_H_


//this code load sgf files into memory first. So the total files should not be too large, e.g. < 1GB.
//this program is only used for testing purpose, so that to minimize IO exchange after loading the data into memory.
double phi(double x);

class GamesResults{
 public:
  GamesResults();
  std::string player1Name;
  std::string player2Name;
  size_t totalGamesPlayed;
  size_t player1Wons;
  size_t draws;
  void printResults();
};

class SGFFilesReader {
 public:
  explicit SGFFilesReader(const std::string & inputDir);
  void splitSGFFilesIntoSingleSgfGameFile(const std::string & outputDir, const std::string & outputFilePrefix);
  GamesResults winningRate();
 private:
  std::vector<std::string> sgfsfiles;
  std::string prefix;
  std::string outputDir;
  size_t indx{0};
  std::vector<std::string> gamesInMemory;
  bool checkAndCreateADir(const std::string & outputDir);
  void loadSgfsFilesIntoMemroy();
};

#endif //KATAGO_DRBOMBEEXPLORE_SGFGAMESUMMARY_H_
