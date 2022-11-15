#ifndef _SIMPLEGR_H_
#define _SIMPLEGR_H_

#include <string>
#include <vector>
#include <map>
#include <limits>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <cassert>
#include <stdint.h>
#include <cstring>

typedef uint16_t CoordType;
typedef uint32_t IdType;
typedef uint32_t LenType;
typedef uint16_t CapType;
typedef float CostType;

const CostType powMax = 1.e12;
const CostType powBase = 5.;
const CostType edgeBase = 2.;
const CostType viaFactor = 3.;
const CostType epsilon = 1.;
const CostType historyIncrement = 0.4;
const IdType NULLID = numeric_limits<IdType>::max();
const CoordType NULLCOORD = numeric_limits < CoordType > ::max();
const CapType NULLCAP = numeric_limits<CapType>::max();

const string SimpleGRversion = "1.1";

class Point
{
public:
  CoordType x;
  CoordType y;
  CoordType z;

  Point() :
      x(0), y(0), z(0)
  {
  }
  Point(CoordType X, CoordType Y, CoordType Z) :
      x(X), y(Y), z(Z)
  {
  }
  Point(const Point &orig) :
      x(orig.x), y(orig.y), z(orig.z)
  {
  }

  const Point &operator=(const Point &assign)
  {
    x = assign.x;
    y = assign.y;
    z = assign.z;
    return *this;
  }

  void setCoord(CoordType x_, CoordType y_, CoordType z_) {
    x = x_;
    y = y_;
    z = z_;
  }
};

inline bool operator==(const Point &a, const Point &b)
{
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline bool operator!=(const Point &a, const Point &b)
{
  return a.x != b.x || a.y != b.y || a.z != b.z;
}

inline bool operator<(const Point &a, const Point &b)
{
  return a.x < b.x || (a.x == b.x && a.y < b.y) || (a.x == b.x && a.y == b.y && a.z < b.z);
}

class Net
{
public:
  LenType numSegments, numVias;
  Point gCellOne, gCellTwo;
  IdType id;
  bool routed;
  vector<IdType> segments;

  Net() :
      numSegments(0), numVias(0), gCellOne(0, 0, 0), gCellTwo(0, 0, 0), id(NULLID), routed(false)
  {
  }
  Net(const Net &orig) :
      numSegments(orig.numSegments), numVias(orig.numVias), gCellOne(orig.gCellOne), gCellTwo(orig.gCellTwo), id(
          orig.id), routed(orig.routed), segments(orig.segments)
  {
  }
};

//@brief: GCell class is derived from Point to include coordinate on a grid
//        It also contains the Id of edges connected to this gcell
class GCell : public Point
{
public:
  //Edge Id in all 6 directions (3D grid)
  IdType incX, decX, incY, decY, incZ, decZ;

  //Default constructor. The default gcell has no connection in all
  //6 directions (NULLID)
  GCell() :
    Point(),
    incX(NULLID), decX(NULLID), incY(NULLID),
    decY(NULLID), incZ(NULLID), decZ(NULLID)
  {
  }
  GCell(const GCell &orig) :
    Point(orig),
    incX(orig.incX), decX(orig.decX), incY(orig.incY),
    decY(orig.decY), incZ(orig.incZ), decZ(orig.decZ)
  {
  }
};

enum EdgeType
{
  HORIZ, VERT, VIA
};

//@brief: Edge class defines an edge that connects adjacent gcells
class Edge
{
public:
  //the two adjacent gcells this edge connects
  GCell* gcell1;
  GCell* gcell2;
  //edge properties. The following 4 fields consumes an uint32 together
  uint32_t type     : 8;
  uint32_t capacity : 8;
  uint32_t usage    : 8;
  uint32_t layer    : 8;
  uint32_t id;
  //nets that routes pass this edge
  vector<IdType> nets;
  //used by DLM cost function
  CostType historyCost;

  //Default constructor. The default edge does not connect to any gcell, and has
  //no usage, capacity or layer info.
  Edge() :
      gcell1(NULL), gcell2(NULL),
      type(VIA), capacity(0), usage(0), layer(NULLCOORD), id(NULLID), historyCost(1)
  {
  }
  Edge(const Edge &orig) :
      gcell1(orig.gcell1), gcell2(orig.gcell2), type(orig.type), capacity(orig.capacity), usage(orig.usage), layer(
          orig.layer), id(orig.id), nets(orig.nets), historyCost(orig.historyCost)
  {
  }
};

// @brief: a simple implementation of priority queue
class PQueue
{
  class GCellData
  {
  public:
    IdType heapLoc;      // gcell's location in the priority queue. If it equals to NULLID, that means the gcell isn't in the queue.
    CostType totalCost;  // aggregated edge cost(pathCost) + Manhattan distance cost of this gcell
    CostType pathCost;   // aggregated edge cost along the path
    IdType parentGCell;  // the gcell that this gcell propagated from. This is used for the back-trace process
  };


  ///////////////////////////////////////////////////////////////////////////////
  // BitBoard is used inside the Priority Queue to keep track of visited gcells
  ///////////////////////////////////////////////////////////////////////////////
  class BitBoard
  {
  private:
    vector<IdType> setBits;
    vector<bool> bits;

  public:
    BitBoard() :
        setBits(0), bits(0)
    {
    }
    BitBoard(IdType size) :
        setBits(0), bits(size, false)
    {
    }
    const vector<IdType> &getSetBits(void)
    {
      return setBits;
    }
    void setBit(IdType id)
    {
      if (!bits.at(id)) {
        bits[id] = true;
        setBits.push_back(id);
      }
    }

    bool isBitSet(IdType id) const
    {
      return bits.at(id);
    }

    void clear(void)
    {
      for (unsigned i = 0; i < setBits.size(); ++i) {
        bits[setBits[i]] = false;
      }
      setBits.clear();
    }

    void resize(IdType size)
    {
      IdType oldSize = bits.size();

      clear();
      bits.resize(size);
      if (oldSize < size) {
        for (IdType i = oldSize; i < size; ++i) {
          bits[i] = false;
        }
      }
    }
  };

  vector<GCellData> data;
  BitBoard dataValid;
  vector<IdType> heap;

public:

  PQueue() :
      data(), dataValid(), heap()
  {
  }
  ;

  //allocate the pqueue data size, typically it's the same as the total num of gcells
  void resize(unsigned newSize)
  {
    data.resize(newSize);
    dataValid.resize(newSize);
  }
  //check if nothing left in heap
  bool isEmpty() const
  {
    return heap.empty();
  }
  // Reset the priority queue to an empty state
  void clear();

  // The following APIs will be used extensively by the A*search
  // Returns the gcell ID that currently has the minimum cost
  IdType getBestGCell(void) const;
  // Remove the minimum cost gcell from priority queue
  void rmBestGCell(void);
  // Update the cost of a gcell
  void setGCellCost(IdType gcellId, CostType totalCost, CostType pathCost, IdType parent);
  // Returns gcell data for given a gcell ID
  const GCellData& getGCellData(IdType gcellId) const;
  // Returns if a gcell has been visited previously
  bool isGCellVsted(IdType gcellId) const;
};

//@brief: manages commandline parameters passed to the SimpleGR
class SimpleGRParams
{
  friend class SimpleGR;

private:
  void setDefault(void);
  void print(void) const;

public:

  bool layerAssign;
  bool verbose;
  unsigned maxRipIter, maxGreedyIter;
  double timeOut;
  string outputFile;
  string inputFile;

  SimpleGRParams(void)
  {
    setDefault();
  }
  SimpleGRParams(int argc, char **argv);

  static void usage(const char* exename);
};


class EdgeCost;

class SimpleGR
{
  friend class EdgeCost;

private:

  // design stats
  IdType gcellArrSzX, gcellArrSzY, numLayers, routableNets, nonViaEdges;
  LenType minX, minY, gcellWidth, gcellHeight, halfWidth, halfHeight;

  // routing stats
  unsigned totalOverflow, overfullEdges, totalSegments, totalVias;

  // global routing data
  vector<CapType> vertCaps, horizCaps, minWidths, minSpacings, viaSpacings;
  vector<Net> grNetArr;
  vector<string> netNameArr;
  vector<IdType> netDBIdArr;
  vector<vector<vector<GCell> > > gcellArr3D;
  vector<Edge> grEdgeArr;
  PQueue priorityQueue;
  map<string, Net*> netNameToPtrMap;

  SimpleGRParams params;

  inline IdType gcellCoordToId(const CoordType x, const CoordType y, const CoordType z) const
  {
    assert(x<gcellArrSzX);
    assert(y<gcellArrSzY);
    assert(z<numLayers);
    return z*gcellArrSzY*gcellArrSzX+y*gcellArrSzX+x;
  }
  inline Point gcellIdtoCoord(const IdType id) const
  {
    assert(id<numLayers*gcellArrSzX*gcellArrSzY);
    return Point(id%gcellArrSzX, (id/gcellArrSzX)%gcellArrSzY, id/(gcellArrSzX*gcellArrSzY));
  }

  //@brief: get the reference of a gcell from an gcell ID
  GCell& getGCell(const IdType gcellId) {
    Point gcell = gcellIdtoCoord(gcellId);
    return gcellArr3D[gcell.z][gcell.y][gcell.x];
  }

  //@brief: get the gcell's ID from a gcell
  IdType getGCellId(const Point gcell) {
    return gcellCoordToId(gcell.x, gcell.y, gcell.z);
  }

  void buildGrid(void);

  void addSegment(Net& net, Edge& edge);
  void ripUpSegment(const IdType netId, IdType edgeId);
  void ripUpNet(const IdType netId);

  void routeFlatNets(bool allowOverflow, const EdgeCost &func);
  CostType routeNet(Net& net, bool allowOverflow, bool bboxConstrain,
                    const EdgeCost &f);
  void routeNets(bool allowOverflow, const EdgeCost &func);

  CostType routeMaze(Net& net, bool allowOverflow, const Point &botleft,
                     const Point &topright, const EdgeCost &func, vector<Edge*> &path);

  // !!! More function declarations should go here
  // !!!function declare

public:

  //Constructor
  SimpleGR(const SimpleGRParams &_params = SimpleGRParams()) :
      gcellArrSzX(0), gcellArrSzY(0), numLayers(0), routableNets(0), nonViaEdges(0), minX(
          0), minY(0), gcellWidth(0), gcellHeight(0), halfWidth(0), halfHeight(0), totalOverflow(0), overfullEdges(
          0), totalSegments(0), totalVias(0), params(_params)
  {
  }

  void parseInput();
  void parseInputMapper(const char *filename);
  void parseSolution(const char *filename);
  void writeRoutes(void);
  void initialRouting(void);
  void doRRR(void);
  void greedyImprovement(void);

  void printParams(void)
  {
    params.print();
  }
  void printStatistics(bool checkRouted = true, bool final = false);
  void printStatisticsLight(void);

  void plotXPM(const string &filename);
};

//@brief: Functor class defined to compare nets by their bounding boxes
class CompareByBox
{
  const vector<Net>* p_nets_;
public:
  CompareByBox(const vector<Net>* p_nets) :
      p_nets_(p_nets)
  {
  }

  bool operator()(const IdType a, const IdType b) const;
};

//@brief: Edge cost function class. It will be extensively used by the MazeRouter.
//@note:  This class is a Singleton -- it is instantiated only once, but can be
//        referenced globally. It is also a Functor that computes the cost of
//        an edge.
class EdgeCost
{
public:
  // Two types of cost functions are defined here
  enum EdgeCostType {UnitCost, DLMCost};
  // Use this API to get a reference of this cost function
  static EdgeCost& getFunc(const SimpleGR * p_gr)
  {
    static EdgeCost costf(p_gr);
    return costf;
  }
  // Functor API. Returns cost of the edge
  inline CostType operator()(IdType edgeId) const
  {
    const Edge &edge=p_gr_->grEdgeArr[edgeId];
    if (edge.type == VIA) {
      return viaCost();
    } else if (type_ == UnitCost) {
      return Unit();
    } else {
      return DLM(edge);
    }
  }
  // This API sets the cost function type for the proper circumstance
  void setType(EdgeCostType type)
  {
    type_ = type;
  }
private:
  const SimpleGR* p_gr_;
  EdgeCostType type_;
  EdgeCost(const SimpleGR * p_gr) :
    p_gr_(p_gr), type_(DLMCost)
  {  //Not Implemented
  }
  EdgeCost(EdgeCost const&);  //Not Implemented
  void operator=(EdgeCost const&);  //Not Implemented

  inline CostType viaCost(void) const
  {
    return viaFactor * edgeBase;  // 3x as costly as a "regular" segment
  }

  // Unit cost treats all edges as equal, returns a unified cost
  inline CostType Unit(void) const
  {
    return edgeBase;
  }

  // DLM cost considers congestion. High congestion is heavily penalized.
  // DLM also uses historyCost to penalize edges that repeated overflow
  CostType DLM(const Edge &edge) const
  {
    const CapType capacity = edge.capacity;
    const CapType newUsage = edge.usage+p_gr_->minWidths[edge.layer]+p_gr_->minSpacings[edge.layer];

    CostType uRatio = static_cast<CostType>(newUsage)/static_cast<CostType>(capacity);
    if (newUsage>capacity) {
      return edgeBase+edge.historyCost*std::min(powMax, powf(powBase, uRatio-1.0));
    } else {
      return edgeBase+edge.historyCost*uRatio;
    }
  }
};

//@brief: Computes the Manhattan distance between two gcells. This cost function can
//        be used as the 'heuristic cost' for A* star search
//@note:  This class is a Singleton -- it is instantiated only once, but can be
//        referenced globally. It is also a Functor.
class ManhattanCost
{
public:
  // Use this API to acquire an reference of this class
  static ManhattanCost& getFunc()
  {
    static ManhattanCost em;
    return em;
  }
  // Functor API, returns Manhattan distance
  inline CostType operator()(const Point a, const Point b) const
  {
    return edgeBase
        *(static_cast<CostType>(abs(a.x-b.x))+static_cast<CostType>(abs(a.y-b.y))
            +viaFactor*static_cast<CostType>(abs(a.z-b.z)));
  }
private:
  ManhattanCost()  //Not Implemented
  {
  }
  ManhattanCost(ManhattanCost const&);  //Not Implemented
  void operator=(ManhattanCost const&);  //Not Implemented
};

//@brief: a percentage progress printing utility for simpleGR
class SimpleProgRpt
{
public:
  SimpleProgRpt(const unsigned size_) : checkpoint(NULL), len(0), k(0), j(0), size(size_)
  {
    int handwritten[] = {10,30,50,70,90};
    set(handwritten, sizeof(handwritten));
    std::cout << "scheduled number of workloads : " << size_ << std::endl;
  }
  void update(unsigned i);
  ~SimpleProgRpt()
  {
    if (!checkpoint) {
      free(checkpoint);
    }
  }
protected:
  inline void set(int * from, size_t byteSize) {
    checkpoint = (int *) malloc(byteSize);
    memcpy(checkpoint, from, byteSize);
    len = byteSize/sizeof(int);
  }
private:
  int* checkpoint;
  int len;
  int k, j;
  const unsigned size;
};

//@brief: A simple CPU timer API
double cpuTime(void);



#endif
