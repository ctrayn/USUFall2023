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

  // Instantiate the Cost function to calculate the Manhattan distance between two gcells.
  // This distance is used as the heuristic cost for A* search.
  ManhattanCost &lb = ManhattanCost::getFunc();

  // A* search kernel
  // cout << "Top Right X " << topright.x << " Y " << topright.y << " Z " << topright.z << endl;
  // cout << "Bot Left  X " << botleft.x << " Y " << botleft.y << " Z " << botleft.z << endl; 
  // Loop until all "frontiers" in the priority queue are exhausted, or when the sink gcell is found.
  IdType curr_cell_id;
  GCell curr_cell;
  GCell next_cell;
  IdType next_edge_id;
  CostType manhattan_cost;
  CostType edge_cost;

  GCell source = getGCell(srcGCellId);
  GCell sink = getGCell(snkGCellId);
  Point src_point = Point(source.x, source.y, source.z);
  CostType curr_len = 0;

  vector<IdType> avail_edges;
  do {
    // YOUR A* search CODE GOES IN HERE.
    curr_cell_id = priorityQueue.getBestGCell();
    if (curr_cell_id == snkGCellId) {
      // cout << "Found Sink " << curr_cell_id << endl;
      break; //got to get out of the loop somehow
    }

    priorityQueue.rmBestGCell();
    // curr_len = priorityQueue.getGCellData(curr_cell_id).pathCost;
    curr_cell = getGCell(curr_cell_id);

    //Get Edges
    avail_edges.clear();
    // printf("Current ID: %d\ndecX %d\ndecY %d\ndecZ %d\nincX %d\nincY %d\nincZ %d\n", curr_cell_id, curr_cell.decX, curr_cell.decY, curr_cell.decZ, curr_cell.incX, curr_cell.incY, curr_cell.incZ);
    avail_edges.push_back(curr_cell.decX);
    avail_edges.push_back(curr_cell.decY);
    avail_edges.push_back(curr_cell.decZ);
    avail_edges.push_back(curr_cell.incX);
    avail_edges.push_back(curr_cell.incY);
    avail_edges.push_back(curr_cell.incZ);

    int size = avail_edges.size();
    for (int i = 0; i < size; i++) {
      //Check validity of next Id
      next_edge_id = avail_edges.back();
      avail_edges.pop_back();

// && (next_edge_id < (numLayers * gcellArrSzX * gcellArrSzY))

      //Check that it's a valid cell id
      if (next_edge_id != NULLID) {
        // printf("Routing Edge %d\n", next_edge_id);
        //Get the cell on the other side of the Edge
        if (*grEdgeArr[next_edge_id].gcell1 == curr_cell) {
          next_cell = *grEdgeArr[next_edge_id].gcell2;
        }
        else {
          next_cell = *grEdgeArr[next_edge_id].gcell1;
        }

        //Check that the cell lives within the bounds
        if ((botleft.x <= next_cell.x && next_cell.x <= topright.x) &&
            (botleft.y <= next_cell.y && next_cell.y <= topright.y) && 
            (botleft.z <= next_cell.z && next_cell.z <= topright.z)) {
          //Add to the priority queue if valid
          manhattan_cost = lb(src_point, Point(next_cell.x, next_cell.y, next_cell.z));
          edge_cost = func(next_edge_id);
          priorityQueue.setGCellCost(gCelltoId(next_cell), manhattan_cost, edge_cost, curr_cell_id);
          // cout << "Added to Priority Queue " << next_cell.x << " " << next_cell.y << " " << next_cell.z << endl;
        }
        else {
          // cout << "Skipping cell " << next_cell.x << " " << next_cell.y << " " << next_cell.z << endl;
        }
      }
      else {
        // printf("Skipping Edge %d\n", next_edge_id);
      }
    }

    // YOUR A* search CODE ENDS HERE
  } while (!priorityQueue.isEmpty());

  // printf("*******************FINISHED A* SEARCH***********************\n");

  // now backtrace and build up the path, if we found one
  // back-track from sink to source, and fill up 'path' vector with all the edges that are traversed
  if (priorityQueue.isGCellVsted(snkGCellId)) {
    // YOUR backtrace CODE GOES IN HERE

    // cout << "Backtracing" << endl;

    IdType current = snkGCellId;
    IdType next;
    Edge* edge_to_add;
    while (next != srcGCellId) {
      // cout << "Current " << current << endl;
      next = priorityQueue.getGCellData(current).parentGCell;
      // cout << "Next " << next << endl;
      edge_to_add = gCellstoEdge(getGCell(next), getGCell(current));
      // cout << "Edge ID " << edge_to_add->id << endl;
      path.push_back(edge_to_add);
      // cout << "Pushed onto path" << endl;
      current = next;
    }

    // cout << "Done backtracing" << endl;

    // YOUR backtrace CODE ENDS HERE
  }
  else {
    // cout << "Missed the sink!!!" << endl;
  }

  // for (int i = 0; i < path.size(); i++) {
  //   cout << path[i]->id << endl;
  // }

  // calculate the accumulated cost of the path
  const CostType finalCost =
      priorityQueue.isGCellVsted(snkGCellId) ?
          priorityQueue.getGCellData(snkGCellId).pathCost : numeric_limits<CostType>::max();

  // clean up
  priorityQueue.clear();

  return finalCost;
}

Edge* SimpleGR::gCellstoEdge(const GCell alpha, const GCell beta) {
  // cout << "Alpha X " << alpha.x << " Y " << alpha.y << " Z " << alpha.z << endl;
  // cout << "Beta  X " << beta.x  << " X " << beta.y  << " Z " << beta.z  << endl;
  vector<IdType> edges;
  edges.push_back(alpha.decX);
  edges.push_back(alpha.decY);
  edges.push_back(alpha.decZ);
  edges.push_back(alpha.incX);
  edges.push_back(alpha.incY);
  edges.push_back(alpha.incZ);

  IdType current;
  GCell one;
  GCell two;

  for (int i = 0; i < 6; i++) {
    current = edges.back();
    edges.pop_back();
    if (current == NULLID) {
      // printf("Skipping %d\n", current);
      continue;
    }
    one = *grEdgeArr[current].gcell1;
    two = *grEdgeArr[current].gcell2;
    // cout << "GCell One X " << one.x << " Y " << one.y << " Z " << one.z << endl;
    // cout << "GCell Two X " << two.x << " Y " << two.y << " Z " << two.z << endl;
    if (((one.x == beta.x) && (one.y == beta.y) && (one.z == beta.z)) || ((two.x == beta.x) && (two.y == beta.y) && (two.z == beta.z))) {
      // cout << "Returning " << current << endl;;
      return &grEdgeArr[current];
    }
    else {
      // cout << "No Match" << endl;
    }
  }
  
  // No Edge exists
  // cout << "Not found" << endl;
  return NULL;
}
