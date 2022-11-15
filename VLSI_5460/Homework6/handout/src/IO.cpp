#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <climits>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <cstdlib>

using namespace std;

#include "SimpleGR.h"

void expect(string s1, string s2)
{
  if (s1!=s2) {
    cout<<"Parsing error. Expected `"<<s2<<"' but got `"<<s1<<"' instead"<<endl;
    exit(0);
  }
}

//@brief: load a design benchmark into memory... comment me
void SimpleGR::parseInput()
{
  if (params.inputFile.empty()) {
    cout<<"Error: Unspecified design file" << endl;
    exit(0);
  }
  ifstream infile(params.inputFile.c_str());

  if (!infile.good()) {
    cout<<"Error: Could not open `"<<params.inputFile<<"' for reading"<<endl;
    exit(0);
  } else {
    cout<<"Reading from `"<<params.inputFile<<"' ..."<<endl;
  }

  string textstring;

  // FIXME: comment purpose of the following 3 lines
  infile>>textstring;
  expect(textstring, string("grid"));
  infile>>gcellArrSzX>>gcellArrSzY>>numLayers;

  // always true for this assignment
  if (numLayers<=2)
    params.layerAssign = false;

  cout<<"grid size "<<gcellArrSzX<<"x"<<gcellArrSzY<<endl;

  //FIXME: comment on what is loaded here till the next 'checkpoint'
  infile>>textstring;
  expect(textstring, string("vertical"));
  infile>>textstring;
  expect(textstring, string("capacity"));
  for (unsigned i = 0; i<numLayers; ++i) {
    unsigned cap;
    infile>>cap;
    vertCaps.push_back(cap);
  }

  infile>>textstring;
  expect(textstring, string("horizontal"));
  infile>>textstring;
  expect(textstring, string("capacity"));
  for (unsigned i = 0; i<numLayers; ++i) {
    unsigned cap;
    infile>>cap;
    horizCaps.push_back(cap);
  }
  // checkpoint

  //FIXME: comment on what is loaded here till the next 'checkpoint'
  infile>>textstring;
  expect(textstring, string("minimum"));
  infile>>textstring;
  expect(textstring, string("width"));

  for (unsigned i = 0; i<numLayers; ++i) {
    unsigned minwidth;
    infile>>minwidth;
    minWidths.push_back(minwidth);
  }

  infile>>textstring;
  expect(textstring, string("minimum"));
  infile>>textstring;
  expect(textstring, string("spacing"));

  for (unsigned i = 0; i<numLayers; ++i) {
    unsigned minspacing;
    infile>>minspacing;
    minSpacings.push_back(minspacing);
  }
  // checkpoint

  // FIXME: leave a comment here on how to use minWidths and minSpacings to
  //         get routing demand of each routing segment on a given layer.

  // Read in via spacing, but it won't be used
  infile>>textstring;
  expect(textstring, string("via"));
  infile>>textstring;
  expect(textstring, string("spacing"));
  for (unsigned i = 0; i<numLayers; ++i) {
    unsigned viaspacing;
    infile>>viaspacing;
    viaSpacings.push_back(viaspacing);
  }

  infile>>minX>>minY>>gcellWidth>>gcellHeight;

  halfWidth = gcellWidth>>1;
  halfHeight = gcellHeight>>1;

  //FIXME: comment on what is loaded here till the next 'checkpoint'
  unsigned numNets;
  infile>>textstring;
  expect(textstring, string("num"));
  infile>>textstring;
  expect(textstring, string("net"));
  infile>>numNets;
  grNetArr.reserve(numNets);

  //FIXME: comment on the purpose of the for loop
  for (unsigned i = 0; i<numNets; ++i) {
    Net newNet;
    unsigned numPins;
    unsigned wireWidth;
    string name;
    IdType dbId;

    infile>>name>>dbId>>numPins>>wireWidth;

    //In this project, all nets have exactly 2 pins.
    assert(numPins==2);

    double pinX, pinY;
    unsigned layer;

    infile>>pinX>>pinY>>layer;
    // translate detailed pin coords to global grid coords
    newNet.gCellOne.x = static_cast<unsigned>(floor((pinX-minX)/gcellWidth));
    newNet.gCellOne.y = static_cast<unsigned>(floor((pinY-minY)/gcellHeight));
    newNet.gCellOne.z = layer-1;

    infile>>pinX>>pinY>>layer;
    // translate detailed pin coords to global grid coords
    newNet.gCellTwo.x = static_cast<unsigned>(floor((pinX-minX)/gcellWidth));
    newNet.gCellTwo.y = static_cast<unsigned>(floor((pinY-minY)/gcellHeight));
    newNet.gCellTwo.z = layer-1;

    // FIXME: comment on the if.else. logic
    if (newNet.gCellOne!=newNet.gCellTwo) {
      ++routableNets;

      newNet.id = grNetArr.size();
      grNetArr.push_back(newNet);
      netNameToPtrMap[name] = &grNetArr.back();
      netNameArr.push_back(name);
      netDBIdArr.push_back(dbId);
    } else {
      netNameToPtrMap[name] = NULL;
    }
  }
  // checkpoint

  cout<<"read in " << grNetArr.size() << " GR nets from "<<numNets<< " nets design" << endl;

  // build the routing grid graph
  buildGrid();

  unsigned capacityChanges;
  infile>>capacityChanges;

  // Start to apply the capacity adjustments
  for (unsigned i = 0; i<capacityChanges; ++i) {
    unsigned gridCol1, gridRow1, layer1, gridCol2, gridRow2, layer2, newCap;

    infile>>gridCol1>>gridRow1>>layer1>>gridCol2>>gridRow2>>layer2>>newCap;

    assert(layer1==layer2);
    assert(layer1<=2);
    assert(layer2<=2);

    if (gridCol1==gridCol2) {
      // FIXME: is it a vertical or horizantal edge?
      assert(gridRow1==gridRow2+1||gridRow1+1==gridRow2);
      if (gcellArr3D[layer1-1][min(gridRow1, gridRow2)][gridCol1].incY==NULLID) {
        if (newCap!=0.) {
          cout<<"Error: Adjusting capacity on a previously non-existing edge."<<endl;
          exit(0);
        }
      } else {
        grEdgeArr[gcellArr3D[layer1-1][min(gridRow1, gridRow2)][gridCol1].incY].capacity = newCap;
        // FIXME: explain this if logic
        if (newCap==0.) {
          gcellArr3D[layer1-1][min(gridRow1, gridRow2)][gridCol1].incY = NULLID;
          gcellArr3D[layer1-1][max(gridRow1, gridRow2)][gridCol1].decY = NULLID;
        }
      }
    } else if (gridRow1==gridRow2) {
      // FIXME: is it a vertical or horizantal edge?
      assert(gridCol1==gridCol2+1||gridCol1+1==gridCol2);
      if (gcellArr3D[layer1-1][gridRow1][min(gridCol1, gridCol2)].incX==NULLID) {
        if (newCap!=0.) {
          cout<<"Error: Adjusting capacity on a previously non-existing edge."<<endl;
          exit(0);
        }
      } else {
        grEdgeArr[gcellArr3D[layer1-1][gridRow1][min(gridCol1, gridCol2)].incX].capacity = newCap;
        // FIXME: explain this if logic (same as above)
        if (newCap==0.) {
          gcellArr3D[layer1-1][gridRow1][min(gridCol1, gridCol2)].incX = NULLID;
          gcellArr3D[layer1-1][gridRow1][max(gridCol1, gridCol2)].decX = NULLID;
        }
      }
    } else {
      cout<<"Error: Bad capacity adjustment."<<endl;
      exit(0);
    }
  }

  infile.close();

  //Initialize the priority queue for maze routing
  priorityQueue.resize(numLayers*gcellArrSzX*gcellArrSzY);

  cout << "CPU time: " << cpuTime() << " seconds" << endl;
}

void SimpleGR::parseInputMapper(const char *filename)
{
  ifstream infile(filename);

  if (!infile.good()) {
    cout<<"Could not open `"<<filename<<"' for reading"<<endl;
    exit(0);
  } else {
    cout<<"Reading from `"<<filename<<"' ..."<<endl;
  }

  string tmp;

  infile>>tmp;
  expect(tmp, string("grid"));

  infile>>gcellArrSzX>>gcellArrSzY>>numLayers;

  if (numLayers<=2)
    params.layerAssign = false;

  cout<<"grid size "<<gcellArrSzX<<"x"<<gcellArrSzY<<endl;

  infile>>tmp;
  expect(tmp, string("vertical"));
  infile>>tmp;
  expect(tmp, string("capacity"));

  for (unsigned i = 0; i<numLayers; ++i) {
    unsigned cap;
    infile>>cap;
    vertCaps.push_back(cap);
  }

  infile>>tmp;
  expect(tmp, string("horizontal"));
  infile>>tmp;
  expect(tmp, string("capacity"));

  for (unsigned i = 0; i<numLayers; ++i) {
    unsigned cap;
    infile>>cap;
    horizCaps.push_back(cap);
  }

  infile>>tmp;
  expect(tmp, string("minimum"));
  infile>>tmp;
  expect(tmp, string("width"));

  for (unsigned i = 0; i<numLayers; ++i) {
    unsigned minwidth;
    infile>>minwidth;
    minWidths.push_back(minwidth);
  }

  infile>>tmp;
  expect(tmp, string("minimum"));
  infile>>tmp;
  expect(tmp, string("spacing"));

  for (unsigned i = 0; i<numLayers; ++i) {
    unsigned minspacing;
    infile>>minspacing;
    minSpacings.push_back(minspacing);
  }

  infile>>tmp;
  expect(tmp, string("via"));
  infile>>tmp;
  expect(tmp, string("spacing"));

  for (unsigned i = 0; i<numLayers; ++i) {
    unsigned viaspacing;
    infile>>viaspacing;
    viaSpacings.push_back(viaspacing);
  }

  infile>>minX>>minY>>gcellWidth>>gcellHeight;

  halfWidth = static_cast<LenType>(trunc(0.5*gcellWidth));
  halfHeight = static_cast<LenType>(trunc(0.5*gcellHeight));

  infile>>tmp;
  expect(tmp, string("num"));
  infile>>tmp;
  expect(tmp, string("net"));

  unsigned numNets;

  infile>>numNets;

  cout<<"total nets "<<numNets<<endl;

  grNetArr.reserve(numNets);

  for (unsigned i = 0; i<numNets; ++i) {
    Net temp;
    unsigned wireWidth;
    unsigned numPins;
    string name;
    IdType id;

    infile>>name>>id>>numPins>>wireWidth;

    //For this project, all nets have exactly 2 pins.
    assert(numPins==2);

    // translate absolute coords to grid coords
    double pinX, pinY;
    unsigned layer;

    infile>>pinX>>pinY>>layer;
    temp.gCellOne.x = static_cast<unsigned>(floor((pinX-minX)/gcellWidth));
    temp.gCellOne.y = static_cast<unsigned>(floor((pinY-minY)/gcellHeight));
    temp.gCellOne.z = layer-1;

    infile>>pinX>>pinY>>layer;
    temp.gCellTwo.x = static_cast<unsigned>(floor((pinX-minX)/gcellWidth));
    temp.gCellTwo.y = static_cast<unsigned>(floor((pinY-minY)/gcellHeight));
    temp.gCellTwo.z = layer-1;

    if (temp.gCellOne!=temp.gCellTwo) {
      ++routableNets;

      grNetArr.push_back(temp);
      netNameToPtrMap[name] = &grNetArr.back();
      netNameArr.push_back(name);
      netDBIdArr.push_back(id);
    } else {
      netNameToPtrMap[name] = NULL;
    }
  }

  // build the routing grid graph
  buildGrid();

  unsigned capacityChanges;
  infile>>capacityChanges;

  for (unsigned i = 0; i<capacityChanges; ++i) {
    unsigned gridCol1, gridRow1, layer1, gridCol2, gridRow2, layer2, newCap;

    infile>>gridCol1>>gridRow1>>layer1>>gridCol2>>gridRow2>>layer2>>newCap;

    assert(layer1==layer2);
    if (gridCol1==gridCol2) {
      // vert segment
      assert(gridRow1==gridRow2+1||gridRow1+1==gridRow2);
      grEdgeArr[gcellArr3D[layer1-1][min(gridRow1, gridRow2)][gridCol1].incY].capacity = newCap;
    } else if (gridRow1==gridRow2) {
      // horiz segment
      assert(gridCol1==gridCol2+1||gridCol1+1==gridCol2);
      grEdgeArr[gcellArr3D[layer1-1][gridRow1][min(gridCol1, gridCol2)].incX].capacity = newCap;
    } else {
      assert(0);
    }
  }

  infile.close();
}

void SimpleGR::parseSolution(const char* filename)
{
  ifstream infile(filename);

  if (!infile.good()) {
    cout<<"Could not open `"<<filename<<"' for reading"<<endl;
    exit(0);
  } else {
    cout<<"Reading from `"<<filename<<"' ..."<<endl;
  }

  string netname;

  infile>>netname;

  while (infile.good()) {
    if (netNameToPtrMap.find(netname)==netNameToPtrMap.end()) {
      cout<<"unknown net with name `"<<netname<<"'"<<endl;
      exit(0);
    }

    Net& net = *netNameToPtrMap[netname];

    char junk;
    string restofline;
    getline(infile, restofline);

    while (infile.peek()=='(') {
      unsigned x1, y1, z1, x2, y2, z2;
      infile>>junk>>x1>>junk>>y1>>junk>>z1>>junk>>junk>>junk>>x2>>junk>>y2>>junk>>z2;
      getline(infile, restofline);

      unsigned gcell1x, gcell1y, gcell1z, gcell2x, gcell2y, gcell2z;

      gcell1x = static_cast<unsigned>(floor((x1-minX)/gcellWidth));
      gcell2x = static_cast<unsigned>(floor((x2-minX)/gcellWidth));
      gcell1y = static_cast<unsigned>(floor((y1-minY)/gcellHeight));
      gcell2y = static_cast<unsigned>(floor((y2-minY)/gcellHeight));
      gcell1z = z1-1;
      gcell2z = z2-1;

      if (gcell1x>=gcellArrSzX) {
        cout<<"illegal x position "<<x1<<endl;
        exit(0);
      }
      if (gcell1y>=gcellArrSzY) {
        cout<<"illegal y position "<<y1<<endl;
        exit(0);
      }
      if (gcell1z>=numLayers) {
        cout<<"illegal layer "<<z1<<endl;
        exit(0);
      }

      if (gcell2x>=gcellArrSzX) {
        cout<<"illegal x position "<<x2<<endl;
        exit(0);
      }
      if (gcell2y>=gcellArrSzY) {
        cout<<"illegal y position "<<y2<<endl;
        exit(0);
      }
      if (gcell2z>=numLayers) {
        cout<<"illegal layer "<<z2<<endl;
        exit(0);
      }

      x1 = gcell1x;
      y1 = gcell1y;
      z1 = gcell1z;
      x2 = gcell2x;
      y2 = gcell2y;
      z2 = gcell2z;

      if (x1!=x2) {
        for (unsigned j = min(x1, x2); j<max(x1, x2); ++j) {
          IdType edgeId = gcellArr3D[z1][y1][j].incX;
          assert(edgeId!=NULLID);
          addSegment(net, grEdgeArr[edgeId]);
        }
      } else if (y1!=y2) {
        for (unsigned j = min(y1, y2); j<max(y1, y2); ++j) {
          IdType edgeId = gcellArr3D[z1][j][x1].incY;
          assert(edgeId!=NULLID);
          addSegment(net, grEdgeArr[edgeId]);
        }
      } else {
        for (unsigned j = min(z1, z2); j<max(z1, z2); ++j) {
          IdType edgeId = gcellArr3D[j][y1][x1].incZ;
          assert(edgeId!=NULLID);
          addSegment(net, grEdgeArr[edgeId]);
        }
      }
    }

    getline(infile, restofline); // the trailing !

    // read in the next one
    infile>>netname;
  }
}

void SimpleGR::writeRoutes()
{
  if (params.outputFile.empty()) {
    return;
  }
  string filename = params.outputFile;
  ofstream outfile(filename.c_str());

  if (!outfile.good()) {
    cout<<"Could not open `"<<filename<<"' for writing."<<endl;
    return;
  }

  cout<<"Writing `"<<filename<<"' ..."<<flush;

  for (unsigned i = 0; i<grNetArr.size(); ++i) {
    bool netRouted = grNetArr[i].routed;

    if (netRouted) {
      vector<unsigned> usedEdges(grNetArr[i].segments);

      sort(usedEdges.begin(), usedEdges.end());

      vector<pair<Point, Point> > segments;

      // add routed segments
      assert(usedEdges.size()>0);
      Point startPt(*grEdgeArr[usedEdges[0]].gcell1);
      Point endPt(*grEdgeArr[usedEdges[0]].gcell2);
      for (unsigned j = 1; j<usedEdges.size(); ++j) {
        unsigned prevEdge = usedEdges[j-1];
        unsigned currEdge = usedEdges[j];

        if (grEdgeArr[prevEdge].type!=grEdgeArr[currEdge].type
            ||grEdgeArr[prevEdge].gcell2!=grEdgeArr[currEdge].gcell1) {
          segments.push_back(make_pair(startPt, endPt));
          startPt = *grEdgeArr[currEdge].gcell1;
          endPt = *grEdgeArr[currEdge].gcell2;
        } else {
          endPt = *grEdgeArr[currEdge].gcell2;
        }
      }
      segments.push_back(make_pair(startPt, endPt));

      outfile<<netNameArr[i]<<" "<<netDBIdArr[i]<<" "<<segments.size()<<endl;
      for (unsigned j = 0; j<segments.size(); ++j) {
        outfile<<"("<<minX+gcellWidth*static_cast<double>(segments[j].first.x)+halfWidth<<","
            <<minY+gcellHeight*static_cast<double>(segments[j].first.y)+halfHeight;
        outfile<<","<<segments[j].first.z+1;
        outfile<<")-("<<minX+gcellWidth*static_cast<double>(segments[j].second.x)+halfWidth<<","
            <<minY+gcellHeight*static_cast<double>(segments[j].second.y)+halfHeight;
        outfile<<","<<segments[j].second.z+1;
        outfile<<")"<<endl;
      }
      outfile<<"!"<<endl;
    }
  }

  cout<<" done"<<endl;
}

const char* coolMap[] = {
/* columns rows colors chars-per-pixel */
"a c #ffffff", "b c #00009f", "c c #0000af", "d c #0000bf", "e c #0000cf", "f c #0000df",
    "g c #0000ef", "h c #0000ff", "i c #000fff", "j c #001fff", "k c #002fff", "l c #003fff",
    "m c #004fff", "n c #005fff", "o c #006fff", "p c #007fff", "q c #008fff", "r c #009fff",
    "s c #00afff", "t c #00bfff", "u c #00cfff", "v c #00dfff", "w c #00efff", "x c #00ffff",
    "y c #0fffef", "z c #1fffdf", "A c #2fffcf", "B c #3fffbf", "C c #4fffaf", "D c #5fff9f",
    "E c #6fff8f", "F c #7fff7f", "G c #8fff6f", "H c #9fff5f", "I c #afff4f", "J c #bfff3f",
    "K c #cfff2f", "L c #dfff1f", "M c #efff0f", "N c #ffff00", "O c #ffef00", "P c #ffdf00",
    "Q c #ffcf00", "R c #ffbf00", "S c #ffaf00", "T c #ff9f00", "U c #ff8f00", "V c #ff7f00",
    "W c #ff6f00", "X c #ff5f00", "Y c #ff4f00", "Z c #ff3f00", "0 c #ff2f00", "1 c #ff1f00",
    "2 c #ff0f00", "3 c #ff0000", "4 c #ef0000", "5 c #df0000", "6 c #cf0000", "7 c #bf0000",
    "8 c #af0000", "9 c #9f0000", ", c #8f0000", ". c #7f0000" };

const char* Colors[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o",
    "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "A", "B", "C", "D", "E", "F", "G", "H",
    "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "0",
    "1", "2", "3", "4", "5", "6", "7", "8", "9", ",", "." };

void SimpleGR::plotXPM(const string &filename)
{
  const unsigned numColors = 64;

  string fullfilename = filename+string(".xpm");

  ofstream xpmFile(fullfilename.c_str());

  xpmFile<<"/* XPM */"<<endl;
  xpmFile<<"static char *congestion[] = {"<<endl;
  xpmFile<<"/* columns rows colors chars-per-pixel */"<<endl;
  xpmFile<<"\""<<gcellArrSzX-1<<" "<<gcellArrSzY-1<<" "<<numColors<<" 1\","<<endl;
  for (unsigned i = 0; i<numColors; ++i)
    xpmFile<<"\""<<coolMap[i]<<"\","<<endl;
  xpmFile<<"/* pixels */"<<endl;

  vector<vector<double> > image;

  double maxRatio = 1.;

  for (unsigned rj = 2; rj<=gcellArrSzY; ++rj) {
    unsigned j = gcellArrSzY-rj;
    vector<double> horizLine;
    for (unsigned i = 0; i<gcellArrSzX-1; ++i) {
      double xUsage = 0., xCap = 0., yUsage = 0., yCap = 0.;
      for (unsigned k = 0; k<gcellArr3D.size(); ++k) {
        unsigned xEdgeId = gcellArr3D[k][j][i].incX;
        unsigned yEdgeId = gcellArr3D[k][j][i].incY;

        if (xEdgeId!=UINT_MAX) {
          xUsage += grEdgeArr[xEdgeId].usage;
          xCap += grEdgeArr[xEdgeId].capacity;
        }
        if (yEdgeId!=UINT_MAX) {
          yUsage += grEdgeArr[yEdgeId].usage;
          yCap += grEdgeArr[yEdgeId].capacity;
        }
      }

      double xRatio = static_cast<double>(xUsage)/static_cast<double>(xCap);
      double yRatio = static_cast<double>(yUsage)/static_cast<double>(yCap);

      double totalRatio = 0.5*(xRatio+yRatio);

      maxRatio = max(maxRatio, totalRatio);

      horizLine.push_back(totalRatio);
    }
    image.push_back(horizLine);
  }

  cout<<"maxratio was "<<maxRatio<<endl;

  for (unsigned j = 0; j<gcellArrSzY-1; ++j) {
    xpmFile<<"\"";
    for (unsigned i = 0; i<gcellArrSzX-1; ++i) {
      unsigned index = static_cast<unsigned>(floor(
          (image[j][i]/maxRatio)*static_cast<double>(numColors-1)));

      xpmFile<<Colors[index];
    }
    if (j==gcellArrSzY-2) {
      xpmFile<<"\""<<endl;
    } else {
      xpmFile<<"\","<<endl;
    }
  }
  xpmFile<<");"<<endl;
  xpmFile.close();
}
