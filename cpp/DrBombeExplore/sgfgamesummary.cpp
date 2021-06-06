//
// Created by Haoda Fu on 5/17/21.
//
#include "../game/board.h"
#include "../dataio/sgf.h"
#include <string>
#include <vector>
#include "sgfgamesummary.h"
#include <filesystem>
#include <locale>
#include <cmath>

double phi(double x)
{
  // constants
  double a1 =  0.254829592;
  double a2 = -0.284496736;
  double a3 =  1.421413741;
  double a4 = -1.453152027;
  double a5 =  1.061405429;
  double p  =  0.3275911;

  // Save the sign of x
  int sign = 1;
  if (x < 0)
    sign = -1;
  x = fabs(x)/sqrt(2.0);

  // A&S formula 7.1.26
  double t = 1.0/(1.0 + p*x);
  double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

  return 0.5*(1.0 + sign*y);
}


SGFFilesReader::SGFFilesReader(const std::string &folderName) {
  for (const auto &file : std::filesystem::directory_iterator(folderName)) {
    if (file.path().string().size() > 5
        && file.path().string().substr(file.path().string().size() - 5) == ".sgfs") { // <1>
      sgfsfiles.push_back(file.path().string());
    }
  }
  std::cout << sgfsfiles.size() << " sgfs files have been found in directory: " << folderName << std::endl;
  loadSgfsFilesIntoMemroy();
}
// <1> require the file extension as .sgfs file. We didn't change upper cases or other mixed cases.

void SGFFilesReader::splitSGFFilesIntoSingleSgfGameFile(const std::string &outputPath,
                                                        const std::string &outputFilePrefix) {
  bool checkOutputPath = checkAndCreateADir(outputPath);
  if (checkOutputPath) {
    outputDir = outputPath;
    prefix = outputFilePrefix;
    indx = 0;
  } else {
    return;
  }

  for (const auto &game: gamesInMemory) {
    std::string outputFileName = outputDir + "/" + prefix + std::to_string(indx) + ".sgf";
    std::ofstream out(outputFileName);
    out << game;
    out.close();
    ++indx;
  }
}

bool SGFFilesReader::checkAndCreateADir(const std::string &outputPath) {
  if (!std::filesystem::is_directory(outputPath)) {
    std::cout << "The input directory does not exist, and we are creating: " << outputPath << std::endl;
    if (std::filesystem::create_directory(outputPath)) {
      std::cout << "Succeeded!" << std::endl;
    } else {
      std::cout << "Failed to create directory: " << outputPath << std::endl;
      return false;
    };
  }
  return true;
}

void SGFFilesReader::loadSgfsFilesIntoMemroy() {
  if (gamesInMemory.size() != 0) {
    gamesInMemory.clear();
  }
  for (const auto &fileName : sgfsfiles) {
    if (std::filesystem::exists(fileName)) {
      std::ifstream file(fileName);
      std::string str;
      while (std::getline(file, str)) {
        gamesInMemory.push_back(str);
      }
      if (file.is_open()) {
        file.close();
      }
    }
  }
}

GamesResults SGFFilesReader::winningRate() {
  GamesResults results{};
  Sgf sgfparser;
  bool setPlayerName{false};
  for (const auto &game: gamesInMemory) {
    auto sgfgame = sgfparser.parse(game);
    if (!setPlayerName) {
      results.player1Name = sgfgame->getPlayerName(P_BLACK);
      results.player2Name = sgfgame->getPlayerName(P_WHITE);
      setPlayerName = true;
    }
    if( (sgfgame->getPlayerName(P_BLACK)==results.player1Name && sgfgame->getPlayerName(P_WHITE)==results.player2Name) ||
        (sgfgame->getPlayerName(P_BLACK)==results.player2Name && sgfgame->getPlayerName(P_WHITE)==results.player1Name)
    ){
      ++results.totalGamesPlayed;
      if (sgfgame->getSgfWinner()==C_EMPTY){
        ++results.draws;
      } else if (sgfgame->getPlayerName(sgfgame->getSgfWinner()) ==results.player1Name){
        ++results.player1Wons;
      }
    }
  }
  results.printResults();
  return results;
}



GamesResults::GamesResults()
    : player1Name{},
      player2Name{},
      totalGamesPlayed{0},
      player1Wons{0},
      draws{0} {}

void GamesResults::printResults() {
  std::cout << "Total games have been played: " << totalGamesPlayed << std::endl;
  std::cout << "Player: " << player1Name << " won: " << player1Wons << " games." << std::endl;
  size_t player2wons = totalGamesPlayed- draws - player1Wons;
  std::cout << "Player: " << player2Name << " won: " << player2wons << " games." << std::endl;
  std::cout << "The number of draw games: " << draws << " games." << std::endl;

  double p1w = static_cast<double>(player1Wons);
  double n = static_cast<double>(totalGamesPlayed);
  double d = static_cast<double>(draws);
  double d1 = d*p1w/(n-d);
  p1w += d1;
  double p1rate = p1w/n;
  double z_value = -abs(p1rate-0.5)/sqrt(p1rate*(1-p1rate)/n);
  std::cout << "Under null hypothesis that two players are equally strong, a two sided p-value is equal to: " << 2*phi(z_value) << std::endl;
}
