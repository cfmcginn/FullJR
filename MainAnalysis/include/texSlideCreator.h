#ifndef TEXSLIDECREATOR_H
#define TEXSLIDECREATOR_H

//cpp dependencies
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

//ROOT dependencies
#include "TDatime.h"

//Local (MainAnalysis) dependencies
#include "MainAnalysis/include/plotToSuperPlotDim.h"

//Non-local (Utility) dependencies
#include "Utility/include/checkMakeDir.h"

class texSlideCreator
{
 public:
  std::string texFileName;
  std::string dirName;
  std::string tagStr;
  std::string authorName = "PLACEHOLDER";
  std::vector<std::string> slideTitles;
  std::vector<std::vector<std::string> > pdfPerSlide;

  plotToSuperPlotDim plotter;

  texSlideCreator(){return;}
  texSlideCreator(const std::string inName){Init(inName); return;}
  void Init(const std::string inName){texFileName = inName; return;}
  void InitDir(const std::string inDirName){dirName = inDirName; return;}
  void InitTag(const std::string inTag){tagStr = inTag; return;}
  void SetAuthor(const std::string inName){authorName = inName; return;}
  void SetSlideTitles(std::vector<std::string> inVect){slideTitles = inVect; return;}
  void SetSlidePdfs(std::vector<std::vector<std::string > > inVect){pdfPerSlide = inVect; return;}
  bool CreateTexSlides();
  double GetTextWidthFromNPDF(int npdf);
  int GetPlotsPerRowFromNPDF(int npdf);
  std::string FixString(std::string inStr);
  void Clean();
};

double texSlideCreator::GetTextWidthFromNPDF(int npdf)
{
  double defVal = plotter.GetWidthX(npdf);
  return defVal;
}

int texSlideCreator::GetPlotsPerRowFromNPDF(int npdf)
{
  int defVal = plotter.GetNPlotsX(npdf);
  return defVal;
}

bool texSlideCreator::CreateTexSlides()
{
  if(slideTitles.size() == 0){
    std::cout << "WARNING: Zero slideTitles given to texSliderCreator, no tex produced, return false" << std::endl;
    return false;
  }
  else if(slideTitles.size() != pdfPerSlide.size()){
    std::cout << "WARNING: Number slideTitles, " << slideTitles.size() << ", given to texSliderCreator does not match pdfPerSlide, " << pdfPerSlide.size() << ", no tex produced, return false" << std::endl;
    return false;
  }

  TDatime* date = new TDatime();
  const std::string dateStr = std::to_string(date->GetDate());
  const std::string dateStr2 = std::to_string(date->GetYear()) + "." + std::to_string(date->GetMonth()) + "." + std::to_string(date->GetDay());
  delete date;

  //Fix back of texfilename
  if(texFileName.rfind(".") != std::string::npos) texFileName.replace(texFileName.rfind("."), texFileName.size(), "");
  if(tagStr.size() != 0) texFileName = texFileName + "_" + tagStr;
  texFileName = texFileName + ".";
  if(texFileName.find((dateStr + ".")) == std::string::npos) texFileName.replace(texFileName.rfind("."), 1, "_" + dateStr + ".");

  texFileName = texFileName + "tex";

  std::string dirStr = "pdfDir/" + dateStr + "/";
  if(checkDir(dirName)) dirStr = dirName;
  //fix front of texfilename
  if(texFileName.substr(0, (dirStr).size()).find(dirStr) == std::string::npos){
    while(texFileName.find("/") != std::string::npos){texFileName.replace(0, texFileName.find("/")+1, "");}

    texFileName = dirStr + texFileName;
  }

  checkMakeDir(dirStr);

  std::ofstream texFile(texFileName.c_str());

  texFile << "\\RequirePackage{xspace}" << std::endl;
  texFile << "\\RequirePackage{amsmath}" << std::endl;
  texFile << std::endl;

  texFile << "\\documentclass[xcolor=dvipsnames]{beamer}" << std::endl;
  texFile << "\\usetheme{Warsaw}" << std::endl;
  texFile << "\\setbeamercolor{structure}{fg=NavyBlue!90!NavyBlue}" << std::endl;
  texFile << "\\setbeamercolor{footlinecolor}{fg=white,bg=lightgray}" << std::endl;
  texFile << std::endl;

  texFile << "\\newcommand{\\pt}{\\ensuremath{p_{\\mathrm{T}}}\\xspace}" << std::endl;
  texFile << std::endl;

  texFile << "\\setbeamersize{text margin left=3pt,text margin right=3pt}" << std::endl;
  texFile << std::endl;

  texFile << "\\setbeamertemplate{frametitle}" << std::endl;
  texFile << "{" << std::endl;
  texFile << "    \\nointerlineskip" << std::endl;
  texFile << "    \\begin{beamercolorbox}[sep=0.1cm, ht=1.0em, wd=\\paperwidth]{frametitle}" << std::endl;
  texFile << "        \\vbox{}\\vskip-2ex%" << std::endl;
  texFile << "        \\strut\\insertframetitle\\strut" << std::endl;
  texFile << "        \\vskip-0.8ex%" << std::endl;
  texFile << "    \\end{beamercolorbox}" << std::endl;
  texFile << "}" << std::endl;
  texFile << std::endl;

  texFile << "\\setbeamertemplate{footline}{%" << std::endl;
  texFile << "  \\begin{beamercolorbox}[sep=.4em,wd=\\paperwidth,leftskip=0.5cm,rightskip=0.5cm]{footlinecolor}" << std::endl;
  texFile << "    \\hspace{0.15cm}%" << std::endl;
  texFile << "    \\hfill\\insertauthor \\hfill\\insertpagenumber" << std::endl;
  texFile << "  \\end{beamercolorbox}%" << std::endl;
  texFile << "}" << std::endl;
  texFile << "\\setbeamertemplate{navigation symbols}{}" << std::endl;
  texFile << std::endl;

  texFile << "\\setbeamertemplate{itemize item}[circle]" << std::endl;
  texFile << "\\setbeamertemplate{itemize subitem}[circle]" << std::endl;
  texFile << "\\setbeamertemplate{itemize subsubitem}[circle]" << std::endl;
  texFile << "\\setbeamercolor{itemize item}{fg=black}" << std::endl;
  texFile << "\\setbeamercolor{itemize subitem}{fg=black}" << std::endl;
  texFile << "\\setbeamercolor{itemize subsubitem}{fg=black}" << std::endl;
  texFile << std::endl;

  texFile << "\\definecolor{links}{HTML}{00BFFF}" << std::endl;
  texFile << "\\hypersetup{colorlinks,linkcolor=,urlcolor=links}" << std::endl;
  texFile << std::endl;

  texFile << "\\author[CM]{" << authorName << "}" << std::endl;
  texFile << std::endl;

  texFile << "\\begin{document}" << std::endl;
  texFile << "\\begin{frame}" << std::endl;
  texFile << "\\frametitle{\\centerline{JEC Validation (" << dateStr2 << ")}}" << std::endl;
  texFile << " \\begin{itemize}" << std::endl;
  texFile << "  \\fontsize{10}{10}\\selectfont" << std::endl;
  texFile << "  \\item{Placeholder}" << std::endl;
  texFile << "  \\begin{itemize}" << std::endl;
  texFile << "   \\fontsize{10}{10}\\selectfont" << std::endl;
  texFile << "   \\item{Placeholder}" << std::endl;
  texFile << "  \\end{itemize}" << std::endl;
  texFile << " \\end{itemize}" << std::endl;
  texFile << "\\end{frame}" << std::endl;

  texFile << std::endl;

  for(unsigned int spI = 0; spI < slideTitles.size(); ++spI){
    const int npdf = pdfPerSlide.at(spI).size();

    const double textWidth = GetTextWidthFromNPDF(npdf);
    const int plotsPerRow = GetPlotsPerRowFromNPDF(npdf);

    texFile << "\\begin{frame}" << std::endl;
    texFile << "\\frametitle{\\centerline{\\fontsize{10}{10}\\selectfont " << FixString(slideTitles.at(spI)) << "}}" << std::endl;

    for(unsigned int pI = 0; pI < pdfPerSlide.at(spI).size(); ++pI){
      texFile << "\\includegraphics[width=" << textWidth << "\\textwidth]{" << pdfPerSlide.at(spI).at(pI) << "}";
      if((pI+1)%plotsPerRow == 0) texFile << "\\\\";
      texFile << std::endl;
    }

    texFile << "\\begin{itemize}" << std::endl;
    texFile << "\\fontsize{5}{5}\\selectfont" << std::endl;
    texFile << "\\item{test}" << std::endl;
    texFile << "\\end{itemize}" << std::endl;
    texFile << "\\end{frame}" << std::endl;

    texFile << std::endl;
  }

  texFile << "\\end{document}" << std::endl;
  texFile << std::endl;

  texFile.close();

  return true;
}

std::string texSlideCreator::FixString(std::string inStr)
{
  const Int_t nCharToEscape = 2;
  const std::string charToEscape[nCharToEscape] = {"%", "_"};

  for(Int_t cI = 0; cI < nCharToEscape; ++cI){
    if(inStr.find(charToEscape[cI]) == std::string::npos) continue;

    unsigned int pos = 0;
    while(pos < inStr.size()){
      bool increment = true;
      
      if(inStr.substr(pos, 1).find(charToEscape[cI]) != std::string::npos){
	increment = false;
	if(pos == 0) inStr = "\\" + inStr;
	else if(inStr.substr(pos-1, 1).find("\\") == std::string::npos) inStr.replace(pos, 1, "\\" + charToEscape[cI]);
	else increment = true;
      }
            
      if(increment) ++pos;
    }
  }

  return inStr;
}


void texSlideCreator::Clean()
{
  texFileName = "";
  authorName = "PLACEHOLDER";
  slideTitles.clear();
  pdfPerSlide.clear();

  return;
}

#endif
