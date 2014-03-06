/*----------------------------------------------------------------------------------------------------------
ROOTmacros
==========

directory contains all useful ROOT based macros


File description:
graphDraw.C - macro which reads in the ASCII data from file. Data format of the file should be Xvalues<tab>Yvalues. One can give several files separated by the comma sign.
Usage:
root -l
.L graphDraw.C++
grapher("/path1/file1,/path2/file2,...")
*/
//----------------------------------------------------------------------------------------------------------

#include <TGraph.h>
#include <TCanvas.h>
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TAxis.h>

using namespace std;
std::list <float> Xtab;
std::list <float> Ytab;
std::list <string> Files;

/*_____________________________________________________________
*NAME: readIn
*DESCRIPTION: This function reads in the data from the file. The data is stored in
                two lists one for X values and the other for Y values.
*/

int readIn(string FileName){
   char line[80];
   float x = 100000;
   float y = 100000;
   FILE * pFile;

  pFile = fopen (FileName.c_str(),"r");
  if (pFile==0)
  {
   cout<<"Error opening file "<<FileName<<endl;
   return 0;
  }

  while(fgets(line, 80, pFile) != NULL)
  {
         /* get a line, up to 80 chars from pFile. done if NULL */
         sscanf (line, "%f\t%f", &x,&y);
          Xtab.push_back(x);
         Ytab.push_back(y);

  }
  fclose(pFile); /* close the file prior to exiting the routine */

return 1;
}
/*_____________________________________________________________
*NAME: parseFileNames
*DESCRIPTION: The Function pares string with path and file names. It returns list of files.
*
*
*/

std::vector<string> parseFileNames(string Names)
{
std::string str = Names;
std::vector<string> vect;

std::istringstream ss(str);
std::string token;

while(std::getline(ss, token, ',')) {
   // std::cout << token << '\n';
    vect.push_back(token);
}
return vect;
}

/*_____________________________________________________________
*NAME: grapher
*DESCRIPTION: The function is main function of the macro. It takes as an argument list of files (separated by ','
* then opens each file and parse the data (Xcolumn<tab>Ycolumn). Later the data is draw on the canvas.
* Legend is generated automatically using /path/fileNames.
*/
int grapher(string File, string Title="", string xAxisTitle = "", string yAxisTitle = ""){
   unsigned short FileQuantity = 1;
   TCanvas *c1;
   float *x, *y;
   Int_t n = 0;
   std::vector<string> FileNames;
   
   //parse file names
   FileNames = parseFileNames(File);
   FileQuantity = FileNames.size();


   /*Debug: print the values read from the file*/
  // std::list<string>::iterator I;
  // for (std::list<string>::iterator I= FileNames.begin(); I != FileNames.end(); ++I)
  // {
  // std::cout <<"|"<<*I<<"|"<< endl;
  // }
  // return 0;
   /*end debug*/

   //**Create one graph for each file
   TGraph **gr = new TGraph*[FileQuantity];
   TMultiGraph *mg = new TMultiGraph();
   mg->SetTitle("Impulse response");
   c1 = new TCanvas("c1","Chart",200,10,700,500);
  
   TLegend *leg = new TLegend(0.48,0.7,0.99,0.9);//relative to pad normalized coordinances (Double_t x1, Double_t y1, Double_t x2, Double_t y2)
   //leg->SetHeader("The Legend Title");
 
   for (int k = 0; k < FileQuantity ; k++){
     Ytab.clear();
     Xtab.clear();

     if(!readIn(FileNames[k])) return 0;
     cout<<"Finished reading file \""<<FileNames[k]<<"\"\n";
   
     /*Debug: print the values read from the file*/
    // std::list<float>::iterator itY;
    // itY = Ytab.begin();

    // for (std::list<float>::iterator itX=Xtab.begin(); itX != Xtab.end(); ++itX)
    // {
    //        std::cout <<"x= "<< *itX<<" y= "<<*itY<<endl;;
    //        itY++;
    // }
     /*end debug*/
     n = Ytab.size();
     x = new float[n];
     y = new float[n];
  
     for (Int_t i=0;i<n;i++)
     {
       x[i] = Xtab.front();
       Xtab.pop_front();
       y[i] = Ytab.front();
       Ytab.pop_front();
     }
     gr[k] = new TGraph(n,x,y);
     gr[k]->SetLineColor(k+1);
     gr[k]->SetLineWidth(2);
     gr[k]->SetMarkerStyle(7);
     gr[k]->SetMarkerColor(k+1);
     


     leg->AddEntry(gr[k],(FileNames[k]).c_str(),"l");
     mg->Add(gr[k]);
     delete x;
     delete y;
   }
   mg->SetTitle(Title.c_str());
   mg->Draw("AP");        //"A"        Axis are drawn around the graph
                        //"L"        A simple polyline is drawn
                        //"F"        A fill area is drawn ('CF' draw a smoothed fill area)
                        //"C"        A smooth Curve is drawn
                        //"*"        A Star is plotted at each point
                        //"P"        The current marker is plotted at each point
                        //"B"        A Bar chart is drawn
                        //"1"        When a graph is drawn as a bar chart, this option makes the bars start from the bottom of the pad. By default they start at 0.
                        //"X+"        The X-axis is drawn on the top side of the plot.
                        //"Y+"        The Y-axis is drawn on the right side of the plot.
   mg->GetXaxis()->SetTitle(xAxisTitle.c_str());
   mg->GetYaxis()->SetTitle(yAxisTitle.c_str());
   leg->Draw();
return 0;
}
