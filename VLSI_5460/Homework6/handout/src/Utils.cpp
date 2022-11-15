#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <cmath>
#include <cassert>
#include <sys/resource.h>
#include <cstdlib>
#include <cstdio>

using namespace std;

#include "SimpleGR.h"

double cpuTime(void)
{
  struct rusage cputime;
  getrusage(RUSAGE_SELF, &cputime);

  return static_cast<double>(cputime.ru_utime.tv_sec)
      +(1.e-6)*static_cast<double>(cputime.ru_utime.tv_usec)
      +static_cast<double>(cputime.ru_stime.tv_sec)
      +(1.e-6)*static_cast<double>(cputime.ru_stime.tv_usec);
}

void SimpleGR::printStatistics(bool checkRouted, bool final)
{
  cout << endl << "GR Stats :" << endl;
  int maxOverfill = 0;
  for (unsigned i = 0; i<grEdgeArr.size(); ++i) {
    maxOverfill = max(maxOverfill, static_cast<int>(grEdgeArr[i].usage-grEdgeArr[i].capacity));
  }

  unsigned netsRouted = 0, routedLen = 0, numVias = 0;
  for (unsigned i = 0; i<grNetArr.size(); ++i) {
    if (!checkRouted||grNetArr[i].routed) {
      ++netsRouted;
      numVias += grNetArr[i].numVias;
      routedLen += grNetArr[i].numSegments;
    }
  }
  string stringFinal = final ? " Final ":" ";

  cout<<stringFinal<<"nets routed "<<netsRouted<<" ("
      <<100.*static_cast<double>(netsRouted)/static_cast<double>(routableNets)<<"%)"<<endl;
  cout<<stringFinal<<"total length for routed nets "<<routedLen<<endl;
  cout<<stringFinal<<"total number of vias "<<numVias<<endl;
  cout<<stringFinal<<"total wire length "<<routedLen+viaFactor*numVias<<endl;
  cout<<stringFinal<<"number of overflowing edges is "<<overfullEdges<<" ("
      <<100.*static_cast<double>(overfullEdges)/static_cast<double>(nonViaEdges)<<"%)"<<endl;
  cout<<stringFinal<<"max overflow is "<<maxOverfill<<endl;
  cout<<stringFinal<<"total overflow is "<<totalOverflow<<endl;
  cout<<stringFinal<<"avg overflow is "<<totalOverflow/static_cast<double>(nonViaEdges)<<endl;
  cout<<stringFinal<<"CPU time: "<<cpuTime()<<" seconds"<<endl<<endl<<flush;
}

void SimpleGR::printStatisticsLight(void)
{
  cout<<"total length for routed nets "<<totalSegments<<endl;
  if (numLayers>1) {
    cout<<"total number of vias "<<totalVias<<endl;
    cout<<"total wire length "<<totalSegments+viaFactor*totalVias<<endl;
  }
  cout<<"number of overflowing edges is "<<overfullEdges<<" ("
      <<100.*static_cast<double>(overfullEdges)/static_cast<double>(nonViaEdges)<<"%)"<<endl;
  cout<<"total overflow is "<<totalOverflow<<endl;
  cout<<"avg overflow is "<<totalOverflow/static_cast<double>(nonViaEdges)<<endl;
  cout<<"CPU time: "<<cpuTime()<<" seconds"<<endl<<flush;
}

//@brief: building the routing grid and initialize it with the default capacity
//        this should be done before the edge capacity adjustment
void SimpleGR::buildGrid(void)
{
  // Note: Hard coded to work with 2 layers only
  assert(numLayers==2);
  // Note: Hard coded to let 1st layer be horizontal and 2nd layer be vertical
  assert(horizCaps[0]);
  assert(vertCaps[1]);

  // Allocate a 3D matrix for the gcell grid
  // Note: all gcells created this way are constructed by their default constructor
  gcellArr3D.resize(numLayers);
  for (unsigned k = 0; k<numLayers; ++k) {
    gcellArr3D[k].resize(gcellArrSzY);
    for (unsigned j = 0; j<gcellArrSzY; ++j) {
      gcellArr3D[k][j].resize(gcellArrSzX);
    }
  }

  Edge newEdge;
  newEdge.usage = 0;

  //Roughly estimate upper bound of edge vector size and allocate it
  // Note: all edges created this way are constructed by their default constructor
  grEdgeArr.reserve(gcellArrSzX*gcellArrSzY*(numLayers+1));

  // add horizontal edges on the first layer
  for (CoordType j = 0; j<gcellArrSzY; ++j) {
    for (CoordType i = 0; i<gcellArrSzX-1; ++i) {
      // fill up edge data
      IdType edgeId = grEdgeArr.size();
      newEdge.gcell1 = &gcellArr3D[0][j][i];
      newEdge.gcell2 = &gcellArr3D[0][j][i+1];
      newEdge.capacity = horizCaps[0];
      newEdge.type = HORIZ;
      newEdge.layer = 0;
      newEdge.id = edgeId;
      // fill up gcell data
      newEdge.gcell1->incX = edgeId;
      newEdge.gcell2->decX = edgeId;
      newEdge.gcell1->setCoord(i, j, 0);
      if (i==gcellArrSzX-2) { //last gcell on the row
        newEdge.gcell2->setCoord(i+1, j, 0);
      }
      // save edge
      grEdgeArr.push_back(newEdge);
    }
  }

  // add vertical edge on the second layer
  for (CoordType i = 0; i<gcellArrSzX; ++i) {
    for (CoordType j = 0; j<gcellArrSzY-1; ++j) {
      // fill up edge data
      IdType edgeId = grEdgeArr.size();
      newEdge.gcell1 = &gcellArr3D[1][j][i];
      newEdge.gcell2 = &gcellArr3D[1][j+1][i];
      newEdge.capacity = vertCaps[1];
      newEdge.type = VERT;
      newEdge.layer = 1;
      newEdge.id = edgeId;
      // fill up gcell data
      newEdge.gcell1->incY = edgeId;
      newEdge.gcell2->decY = edgeId;
      newEdge.gcell1->setCoord(i, j, 1);
      if (j==gcellArrSzY-2) { //last gcell on the column
        newEdge.gcell2->setCoord(i, j+1, 1);
      }
      // save edge
      grEdgeArr.push_back(newEdge);
    }
  }

  nonViaEdges = grEdgeArr.size();

  // add vias
  for (unsigned i = 0; i<gcellArrSzX; ++i) {
    for (unsigned j = 0; j<gcellArrSzY; ++j) {
      IdType edgeId = grEdgeArr.size();
      newEdge.gcell1 = &gcellArr3D[0][j][i];
      newEdge.gcell2 = &gcellArr3D[1][j][i];
      newEdge.capacity = NULLCAP;     //via capacity is not considered
      newEdge.type = VIA;
      newEdge.id = edgeId;

      newEdge.gcell1->incZ = edgeId;
      newEdge.gcell2->decZ = edgeId;
      grEdgeArr.push_back(newEdge);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Priority Queue is used by A* Search. It prioritize the gcell with
// the lowest cost and closer to the sink, and stores the best gcell (or least
// costly) gcell at the front of the queue.
///////////////////////////////////////////////////////////////////////////////

// Return the element at the top of the priority queue
IdType PQueue::getBestGCell(void) const
{
  assert(!isEmpty());
  //printf("Getting the best gcell: %d\n", heap.front());
  return heap.front();
}

// Remove the element at the top of the priority queue
void PQueue::rmBestGCell(void)
{
  assert(!isEmpty());

  data[heap.back()].heapLoc = 0;
  data[heap.front()].heapLoc = NULLID;
  heap.front() = heap.back();
  heap.pop_back();

  // now heap down
  IdType idx = 0, lc = 1, rc = 2;
  while (lc<heap.size()) {
    if (rc<heap.size()) {
      if (data[heap[lc]].totalCost<data[heap[rc]].totalCost) {
        if (data[heap[idx]].totalCost>data[heap[lc]].totalCost) {
          data[heap[idx]].heapLoc = lc;
          data[heap[lc]].heapLoc = idx;
          swap(heap[idx], heap[lc]);
          idx = lc;
        } else {
          break;
        }
      } else {
        if (data[heap[idx]].totalCost>data[heap[rc]].totalCost) {
          data[heap[idx]].heapLoc = rc;
          data[heap[rc]].heapLoc = idx;
          swap(heap[idx], heap[rc]);
          idx = rc;
        } else {
          break;
        }
      }
    } else {
      if (data[heap[idx]].totalCost>data[heap[lc]].totalCost) {
        data[heap[idx]].heapLoc = lc;
        data[heap[lc]].heapLoc = idx;
        swap(heap[idx], heap[lc]);
        idx = lc;
      } else {
        break;
      }
    }
    lc = 2*idx+1;
    rc = lc+1;
  }
}

//@Brief: set the cost of a gcell and add it to the priority queue
//        where totalCost = pathCost + heuristicCost
void PQueue::setGCellCost(IdType gcellId, CostType totalCost, CostType pathCost, IdType parent)
{
  //printf("New gcell added %d with cost %f\n", gcellId, pathCost);
  if (dataValid.isBitSet(gcellId)) {
    if (totalCost<data[gcellId].totalCost) {
      data[gcellId].totalCost = totalCost;
      data[gcellId].pathCost = pathCost;
      data[gcellId].parentGCell = parent;

      // heap up
      IdType idx = data[gcellId].heapLoc, parent;
      while (idx>0) {
        parent = (idx-1)/2;
        if (data[heap[idx]].totalCost<data[heap[parent]].totalCost) {
          data[heap[idx]].heapLoc = parent;
          data[heap[parent]].heapLoc = idx;
          swap(heap[idx], heap[parent]);
          idx = parent;
        } else {
          break;
        }
      }
    }
  } else {
    data[gcellId].totalCost = totalCost;
    data[gcellId].pathCost = pathCost;
    data[gcellId].parentGCell = parent;
    data[gcellId].heapLoc = heap.size();
    heap.push_back(gcellId);
    dataValid.setBit(gcellId);

    // heap up
    IdType idx = data[gcellId].heapLoc;
    while (idx>0) {
      parent = (idx-1)/2;
      if (data[heap[idx]].totalCost<data[heap[parent]].totalCost) {
        data[heap[idx]].heapLoc = parent;
        data[heap[parent]].heapLoc = idx;
        swap(heap[idx], heap[parent]);
        idx = parent;
      } else {
        break;
      }
    }
  }
}

// Return the gcell data
const PQueue::GCellData& PQueue::getGCellData(IdType gcellId) const
{
  assert(dataValid.isBitSet(gcellId));
  return data[gcellId];
}

// Look up whether the gcell has been visited before
bool PQueue::isGCellVsted(IdType gcellId) const
{
  return dataValid.isBitSet(gcellId);
}

// Reset the priority queue to an empty state
void PQueue::clear(void)
{
  dataValid.clear();
  heap.clear();
}

//@brief: commit the edge segment to a net's route, while updating the corresponding
//        changes in edge usage and overflow
void SimpleGR::addSegment(Net& net, Edge& edge)
{
  IdType netId = net.id;
  IdType edgeId = edge.id;
  const CapType curDmd = edge.type==VIA ? 0 : minWidths[edge.layer]+minSpacings[edge.layer];

  vector<IdType>::iterator pos2 = lower_bound(net.segments.begin(), net.segments.end(), edgeId);
  assert(pos2==net.segments.end()||*pos2!=edgeId);
  net.segments.insert(pos2, edgeId);

  vector<IdType>::iterator pos3 = lower_bound(edge.nets.begin(), edge.nets.end(), netId);
  assert(pos3==edge.nets.end()||*pos3!=netId);
  edge.nets.insert(pos3, netId);

  CapType oldOverflow = edge.usage>edge.capacity ? edge.usage-edge.capacity : 0;
  totalOverflow -= oldOverflow;
  edge.usage += curDmd;
  CapType newOverflow = edge.usage>edge.capacity ? edge.usage-edge.capacity : 0;
  totalOverflow += newOverflow;
  if (oldOverflow==0&&newOverflow>0) {
    ++overfullEdges;
  }
  if (edge.type==VIA) {
    ++net.numVias;
    ++totalVias;
  } else {
    ++net.numSegments;
    ++totalSegments;
  }
}

//@brief: ripping the edge segment from a net's route, while updating the corresponding
//        changes in edge usage and overflow
void SimpleGR::ripUpSegment(const IdType netId, IdType edgeId)
{
  Edge& edge = grEdgeArr[edgeId];
  const CapType curDmd = grEdgeArr[edgeId].type==VIA ? 0 : minWidths[edge.layer]+minSpacings[edge.layer];

  assert(edge.usage>=curDmd);

  CapType oldOverflow = edge.usage>edge.capacity ? edge.usage-edge.capacity : 0;
  totalOverflow -= oldOverflow;
  edge.usage -= curDmd;
  CapType newOverflow = edge.usage>edge.capacity ? edge.usage-edge.capacity : 0;
  totalOverflow += newOverflow;
  if (oldOverflow>0&&newOverflow==0) {
    --overfullEdges;
  }
  if (edge.type==VIA) {
    --grNetArr[netId].numVias;
    --totalVias;
  } else {
    --grNetArr[netId].numSegments;
    --totalSegments;
  }

  vector<IdType>::iterator pos = lower_bound(edge.nets.begin(), edge.nets.end(), netId);
  assert(pos!=edge.nets.end()&&*pos==netId);
  edge.nets.erase(pos);
}

void SimpleGRParams::usage(const char *exename)
{
  cout<<"Usage: "<<exename<<" -f <design> [options]"<<endl;
  cout<<endl;
  cout<<"Available options:"<<endl;
  cout<<"* -f <filename>         Specify design file <filename>"<<endl;
  cout<<"  -o <filename>         Save routes in <filename>"<<endl;
  cout<<"  -maxRipIter <uint>    Maximum rip-up and re-route iterations"<<endl;
  cout<<"  -timeOut <double>     Rip-up and re-route timeout (seconds)"<<endl;
  cout<<"  -maxGreedyIter <uint> Maximum greedy iterations"<<endl;
  cout<<"  -h, -help             Show this page"<<endl;
  cout<<"Must provide option marked by *"<<endl;
  cout<<endl;
  exit(0);
}

void SimpleGRParams::setDefault(void)
{
  layerAssign = true;
  verbose = false;
  maxRipIter = 20;
  maxGreedyIter = 1;
  timeOut = 60.*5; // 5 mins
  outputFile = "";
  inputFile = "";
}

void SimpleGRParams::print(void) const
{
  cout<<endl<<"SimpleGR parameters:"<<endl;
  cout<<"Design file to read:       "<<inputFile<<endl;
  cout<<"Maximum RRR iterations:    "<<maxRipIter<<endl;
  cout<<"Max RRR runtime:           "<<timeOut<<" seconds"<<endl;
  cout<<"Maximum greedy iterations: "<<maxGreedyIter<<endl;
  if (!outputFile.empty()) {
    cout<<"Save solution to file:     '"<<outputFile<<"'"<<endl;
  } else {
    cout<<"Warning: output unspecified (-o). No solution file will be generated." << endl;
  }
  cout<<endl;
}

//@brief: a simple implementation of command line parameters
SimpleGRParams::SimpleGRParams(int argc, char **argv)
{
  setDefault();

  if (argc<2) {
    usage(argv[0]);
  }

  for (int i = 1; i<argc; ++i) {
    if (argv[i]==string("-h")||argv[i]==string("-help")) {
      usage(argv[0]);
    } else if (argv[i]==string("-o")) {
      if (i+1<argc) {
        outputFile = argv[ ++i];
      } else {
        cout<<"option -o requires an argument"<<endl;
        usage(argv[0]);
      }
    } else if (argv[i]==string("-f")) {
      if (i+1<argc) {
        inputFile = argv[++i];
      } else {
        cout<<"option -f requires an argument"<<endl;
        usage(argv[0]);
      }
    } else if (argv[i]==string("-maxRipIter")) {
      if (i+1<argc) {
        maxRipIter = static_cast<unsigned>(atoi(argv[ ++i]));
      } else {
        cout<<"option -maxRipIter requires an argument"<<endl;
        usage(argv[0]);
      }
    } else if (argv[i]==string("-maxGreedyIter")) {
      if (i+1<argc) {
        maxGreedyIter = static_cast<unsigned>(atoi(argv[ ++i]));
      } else {
        cout<<"option -maxGreedyIter requires an argument"<<endl;
        usage(argv[0]);
      }
    } else if (argv[i]==string("-timeOut")) {
      if (i+1<argc) {
        timeOut = atof(argv[ ++i]);
      } else {
        cout<<"option -timeOut requires an argument"<<endl;
        usage(argv[0]);
      }
    } else {
      cout<<"unknown commandline option"<<endl;
      usage(argv[0]);
    }
  }

  if (inputFile.empty()) {
    cout<<"Must provide '-f' option"<<endl;
    usage(argv[0]);
  }
}

//@brief: a simple implementation to report progress of routing
void SimpleProgRpt::update(unsigned i)
{
  int percent = i*100/size;
  while (percent > checkpoint[j]) {
    j+=1;
    if (j>=len) {
      j=len-1;
      break;
    }
  }
  if (j!=k) {
    cout << "global router has finished " << checkpoint[j] << "%: " << size-i << " left." << endl<<flush;
    k=j;
  }
}
