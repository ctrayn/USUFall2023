/*
 * MazeRouter.cpp
 * Work on this file to complete your maze router
 */

using namespace std;

#include "SimpleGR.h"

///////////////////////////////////////////////////////////////////////////////
// Implement an A* search based maze routing algorithm
// and a corresponding back-trace procedure
// The function needs to correctly deal with the following conditions:
// 1. Only search within a bounding box defined by botleft and topright points
// 2. Control if any overflow on the path is allowed or not
///////////////////////////////////////////////////////////////////////////////
CostType SimpleGR::routeMaze(Net& net, bool allowOverflow, const Point &botleft,
                             const Point &topright, const EdgeCost &func, vector<Edge*> &path)
{
  // find out the ID of the source and sink gcells
  const IdType srcGCellId = getGCellId(net.gCellOne);
  const IdType snkGCellId = getGCellId(net.gCellTwo);

  // insert the source gcell into the priority queue
  priorityQueue.setGCellCost(srcGCellId, 0., 0., NULLID);

  // Instantiate the Cost function to calculate the Manhattan distance between
  // two gcells. This distance is used as the heuristic cost for A* search.
  ManhattanCost &lb = ManhattanCost::getFunc();

  // A* search kernel
  // Loop until all "frontiers" in the priority queue are exhausted, or when
  // the sink gcell is found.
  IdType curr_cell_id;
  IdType next_cell_id;
  GCell curr_cell;
  GCell next_cell;
  CostType cost;

  GCell source = getGCell(srcGCellId);
  GCell sink = getGCell(snkGCellId);
  Point src_point = Point(source.x, source.y, source.z);
  CostType curr_len = 0;

  vector<IdType> avail_moves;
  do {
    // YOUR A* search CODE GOES IN HERE
    curr_cell_id = priorityQueue.getBestGCell();
    priorityQueue.rmBestGCell();
    curr_len = priorityQueue.getGCellData(curr_cell_id).pathCost;
    curr_cell = getGCell(curr_cell_id);

    if (curr_cell_id == snkGCellId) {
      break; //got to get out of the loop somehow
    }

    //Update surronding gcells
    avail_moves.clear();

    // printf("Current ID: %d\ndecX %d\ndecY %d\ndecZ %d\nincX %d\nincY %d\nincZ %d\n", curr_cell_id, curr_cell.decX, curr_cell.decY, curr_cell.decZ, curr_cell.incX, curr_cell.incY, curr_cell.incZ);
    avail_moves.push_back(curr_cell.decX);
    avail_moves.push_back(curr_cell.decY);
    avail_moves.push_back(curr_cell.decZ);
    avail_moves.push_back(curr_cell.incX);
    avail_moves.push_back(curr_cell.incY);
    avail_moves.push_back(curr_cell.incZ);

    int size = avail_moves.size();
    for (int i = 0; i < size; i++) {
      //Check validity of next Id
      next_cell_id = avail_moves.back();
      avail_moves.pop_back();

      //Check that it's a valid cell id
      if (next_cell_id != NULLID && next_cell_id < numLayers * gcellArrSzX * gcellArrSzY) {
        next_cell = getGCell(next_cell_id);

        //Check that the cell lives within the bounds
        if ((botleft.x < next_cell.x && next_cell.x < topright.x) &&
            (botleft.y < next_cell.y && next_cell.y < topright.y) && 
            (botleft.z < next_cell.z && next_cell.z < topright.z)) {
          //Add to the priority queue if valid
          cost = lb(src_point, Point(next_cell.x, next_cell.y, next_cell.z));
          priorityQueue.setGCellCost(next_cell_id, cost, curr_len + 1, curr_cell_id);
        }
      }
      else {
        // printf("Skipping %d\n", next_cell_id);
      }
    }

    // YOUR A* search CODE ENDS HERE
  } while (!priorityQueue.isEmpty());

  // printf("*******************FINISHED A* SEARCH***********************\n");

  // now backtrace and build up the path, if we found one
  // back-track from sink to source, and fill up 'path' vector with all the edges that are traversed
  if (priorityQueue.isGCellVsted(snkGCellId)) {
    // YOUR backtrace CODE GOES IN HERE

    IdType current = snkGCellId;
    IdType next;
    Edge edge;
    while (next != srcGCellId) {
      next = priorityQueue.getGCellData(current).parentGCell;
      edge.gcell1 = &getGCell(current);
      edge.gcell2 = &getGCell(next);
      path.push_back(new Edge(edge));
      current = next;
    }

    // YOUR backtrace CODE ENDS HERE
  }
  // else {
  //   cout << "Missed the sink!!!" << endl;
  // }

  // calculate the accumulated cost of the path
  const CostType finalCost =
      priorityQueue.isGCellVsted(snkGCellId) ?
          priorityQueue.getGCellData(snkGCellId).pathCost : numeric_limits<CostType>::max();

  // clean up
  priorityQueue.clear();

  return finalCost;
}


