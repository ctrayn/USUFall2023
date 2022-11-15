#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <cstdio>

using namespace std;

#include "SimpleGR.h"

//@brief: This functor compares two nets by their bounding boxes. The one with
//        shorter perimeter and smaller aspect ratio is considered less.
//@param: Two net IDs
bool
CompareByBox::operator()(const IdType a, const IdType b) const
{
  // Reference nets without boundary check
  const Net& neta = (*p_nets_)[a];
  const Net& netb = (*p_nets_)[b];
  // Reference the ending points on nets a and b
  const Point& aOne = neta.gCellOne;
  const Point& bOne = netb.gCellOne;
  const Point& aTwo = neta.gCellTwo;
  const Point& bTwo = netb.gCellTwo;

  // net A's measurements
  CostType aWidth  = static_cast<CostType>(abs(aOne.x-aTwo.x));
  CostType aHeight = static_cast<CostType>(abs(aOne.y-aTwo.y));
  pair<CostType, CostType> costA(aWidth + aHeight, min(aWidth, aHeight) / max(aWidth, aHeight));
  // net B's measurements
  CostType bWidth  = static_cast<CostType>(abs(bOne.x-bTwo.x));
  CostType bHeight = static_cast<CostType>(abs(bOne.y-bTwo.y));
  pair<CostType, CostType> costB(bWidth + bHeight, min(bWidth, bHeight) / max(bWidth, bHeight));

  return costA < costB;
}

//@brief: Look for all "flat" nets, sort them from small to large
//        and route them with a bounding box constraint
//@param: Overflow constraint, EdgeCost functor ref
void
SimpleGR::routeFlatNets(bool allowOverflow, const EdgeCost &func)
{
  unsigned flatNetsRouted = 0;
  const bool bboxConstrain = true;

  //Collect the 'flat' nets
  vector<IdType> netIdVec;
  for (IdType i = 0; i < grNetArr.size(); ++i) {
    Net& net = grNetArr[i];
    if (net.gCellOne.x == net.gCellTwo.x || net.gCellOne.y == net.gCellTwo.y) {
      //count it flat if straight horizontal or vertical
      netIdVec.push_back(i);
    }
  }

  //Sort nets with bounding box sizes. Smaller goes first
  sort(netIdVec.begin(), netIdVec.end(), CompareByBox(&grNetArr));

  //iterator over all collected nets
  SimpleProgRpt report(netIdVec.size());
  for (unsigned i = 0; i < netIdVec.size(); ++i) {
    // print a simple progress report
    report.update(i);

    Net& net = grNetArr[netIdVec[i]];

    if (net.routed)
      continue;

    if (params.verbose) {
      printf("routing flat GR net Id %d. ", net.id);
      printf("(%d, %d, %d) ", net.gCellOne.x, net.gCellOne.y, net.gCellOne.z);
      printf("(%d, %d, %d) ", net.gCellTwo.x, net.gCellTwo.y, net.gCellTwo.z);
    }
    // call router engine to route net with bounding box constraint
    CostType cost = routeNet(net, allowOverflow, bboxConstrain, func);
    if (net.routed)
      ++flatNetsRouted;
    if (params.verbose) {
      if (net.routed) {
        printf(" routed with cost %.2f.\n", cost);
      } else {
        printf(" unable to route.\n");
      }
    }
  }

  cout << flatNetsRouted << " flat GR nets routed" << endl;
}

//@brief: A router engine wrapper. It honors two constraints: overflow and bounding box.
//        It treats overflow as a hard constraint, i.e. if client
//        has specified not allow overflow, then the net is absolutely not
//        routed with any path that causes overflow, or it fails to route.
//        Bounding box constraint is soft. This function will try its best to route
//        within the bounded area, but if that fails then it will remove the constraint.
//@param: The net to be routed, boolean constraints: overflow and bounding box, and EdgeCost functor ref
//@ret:   If net's routed, it returns the cost of the route. Otherwise an undefined value is returned
CostType SimpleGR::routeNet(Net& net, bool allowOverflow, bool bboxConstrain,
                            const EdgeCost &costfunc)
{
  vector<Edge*> routePath;
  CostType totalCost;

  if (bboxConstrain) {
    const Point botleft(min(net.gCellOne.x, net.gCellTwo.x), min(net.gCellOne.y, net.gCellTwo.y), 0);
    const Point topright(max(net.gCellOne.x, net.gCellTwo.x), max(net.gCellOne.y, net.gCellTwo.y), 0);
    totalCost = routeMaze(net, false, botleft, topright, costfunc, routePath);
    if (routePath.empty()) {
      // if not possible, relax the bounding box constraints to find a feasible path
      totalCost = routeMaze(net, allowOverflow, Point(0, 0, 0), Point(gcellArrSzX, gcellArrSzY, 0), costfunc, routePath);
    }
  } else {
    totalCost = routeMaze(net, allowOverflow, Point(0, 0, 0), Point(gcellArrSzX, gcellArrSzY, 0), costfunc, routePath);
  }

  net.routed = (routePath.size() > 0);
  if (net.routed) {
    for (unsigned i = 0; i < routePath.size(); ++i) {
      addSegment(net, *routePath[i]);
    }
  }

  return totalCost;
}

//@brief: rips up a routed net, frees the routing resources it uses, and marks it unrouted.
void SimpleGR::ripUpNet(const IdType netId)
{
  Net &net = grNetArr[netId];
  for (unsigned i = 0; i < net.segments.size(); ++i) {
    ripUpSegment(netId, net.segments[i]);
  }
  net.segments.clear();
  net.routed = false;
}

//@brief: Process nets in a bulk mode: Collect unrouted nets, sort them, and route them
void SimpleGR::routeNets(bool allowOverflow, const EdgeCost &func)
{
  vector<IdType> netIdVec;
  for (unsigned i = 0; i < grNetArr.size(); ++i) {
    if (!grNetArr[i].routed) {
      netIdVec.push_back(i);
    }
  }

  sort(netIdVec.begin(), netIdVec.end(), CompareByBox(&grNetArr));

  bool bboxConstrain = true;

  SimpleProgRpt report(netIdVec.size());
  unsigned routedNets = 0;
  for (unsigned i = 0; i < netIdVec.size(); ++i) {
    Net& net = grNetArr[netIdVec[i]];
    report.update(i);

    if (net.routed)
      continue;

    routeNet(net, allowOverflow, bboxConstrain, func);
    if (net.routed) {
      routedNets++;
    }
  }
  cout << "routed " << routedNets << " GR nets" << endl;
}


//@brief: use iterative rip-up and re-route scheme to improve QoR of all nets.
void SimpleGR::doRRR(void)
{
  if (params.maxRipIter == 0)
    return;

  vector<IdType> netsToRip;
  unsigned iterations = 1;
  EdgeCost &dlm=EdgeCost::getFunc(this);
  dlm.setType(EdgeCost::DLMCost);

  cout << "[Iterative Rip-up and Re-Route starts]" << endl;
  cout<<"Performing at most "<<params.maxRipIter<<" rip-up and re-route iteration(s)"<<endl;

  double startCPU = cpuTime();

  const bool bboxConstrain = true;
  const bool nobboxConstrain = false;
  const bool allowOverflow = true;
  const bool donotallowOverflow = false;

  // outer RRR loop, each loop is one RRR iteration
  while (true) {
    // Start collecting unrouted nets
    for (unsigned i = 0; i < grNetArr.size(); ++i) {
      if (!grNetArr[i].routed) {
        netsToRip.push_back(i);
      }
    }
    // figure out which edges have overflow
    // and update their history costs
    for (unsigned i = 0; i < grEdgeArr.size(); ++i) {
      if (grEdgeArr[i].usage > grEdgeArr[i].capacity) {
        netsToRip.insert(netsToRip.end(), grEdgeArr[i].nets.begin(), grEdgeArr[i].nets.end());
        // overflow edge's history cost increments in each iteration.
        // The history cost is used by the DLM EdgeCost functor
        // to heavily penalize edges that repeatedly overflow
        grEdgeArr[i].historyCost += historyIncrement;
      }
    }
    // get rid of the duplicated nets in the queue
    sort(netsToRip.begin(), netsToRip.end());
    netsToRip.erase(unique(netsToRip.begin(), netsToRip.end()), netsToRip.end());
    // sort the queue by the size of the nets
    sort(netsToRip.begin(), netsToRip.end(), CompareByBox(&grNetArr));
    // Done collecting unrouted nets

    if (netsToRip.size() == 0) {
      cout << "No more GR nets to rip up, quitting" << endl;
      break;
    }
    cout << endl;
    cout << "RRR Iteration " << iterations << " starts" << endl;
    cout << "number of GR nets that need to be ripped up: " << netsToRip.size() << endl;

    // inner RRR loop, each loop rips up and reroutes a net.
    SimpleProgRpt report(netsToRip.size());
    for (unsigned i = 0; i < netsToRip.size(); ++i) {
      report.update(i);

      const IdType netId = netsToRip[i];
      Net& net = grNetArr[netId];

      // rip up the net
      ripUpNet(netId);

      // re-route the net
      if (totalOverflow <= 500) {
        // try to route without allowing overflow
        routeNet(net, donotallowOverflow, nobboxConstrain, dlm);
        if (!net.routed) {
          // failing that, allow overflow
          routeNet(net, allowOverflow, nobboxConstrain, dlm);
        }
      } else {
        routeNet(net, allowOverflow, bboxConstrain, dlm);
      }

      // End as soon as possible
      if (overfullEdges == 0)
        break;
    }

    netsToRip.clear();
    cout << "RRR iteration " << iterations << " ends" << endl;
    ++iterations;

    printStatisticsLight();
    double cpuTimeUsed = cpuTime();

    if (iterations >= params.maxRipIter) {
      cout << "Iterations exceeded, quitting" << endl;
      break;
    }
    if (cpuTimeUsed > startCPU + params.timeOut) {
      cout << "Timeout exceeded, quitting" << endl;
      break;
    }
  }
  cout << "[Iterative Rip-up and Re-Route ends]" << endl;
}

//@brief: Route all nets one by one in a wire length greedy mode.
void SimpleGR::greedyImprovement(void)
{
  if (totalOverflow > 0) {
    cout << "Warning: skipping greedy improvement due to overflowing solution" << endl;
    return;
  }

  cout << "[Greedy improvement routing starts]" << endl;

  // Since we want to get greedy with wire length, we use the unit cost function
  EdgeCost &uc = EdgeCost::getFunc(this);
  uc.setType(EdgeCost::UnitCost);

  const bool donotallowOverflow = false;
  const bool noBBoxConstrain = false;

  vector<IdType> netArray;
  for (unsigned i = 0; i < grNetArr.size(); ++i) {
    netArray.push_back(i);
  }
  sort(netArray.begin(), netArray.end(), CompareByBox(&grNetArr));

  cout << "performing " << params.maxGreedyIter << " greedy improvement iteration(s)" << endl;

  for (unsigned iterations = 1; iterations <= params.maxGreedyIter; ++iterations) {
    cout << endl << "examining " << netArray.size() << " GR nets " << endl;
    SimpleProgRpt report(netArray.size());
    for (unsigned i = 0; i < netArray.size(); ++i) {
      report.update(i);
      Net& net = grNetArr[netArray[i]];
      // Rip-up only this one net
      ripUpNet(netArray[i]);
      // We now know there is at least one path that the net can be routed without overflow,
      // so we can safely turn on overflow constraint for the router.
      routeNet(net, donotallowOverflow, noBBoxConstrain, uc);
    }
    cout << "after greedy improvement iteration " << iterations << endl;
    printStatisticsLight();
  }
  cout << "[Greedy improvement routing ends]" << endl;
}

//@brief: Initial route all nets with minimum effort.
void SimpleGR::initialRouting(void)
{
  unsigned totalNets = 0;
  for (unsigned i = 0; i < grNetArr.size(); ++i) {
    totalNets++;
  }
  cout << "[Initial routing starts]" << endl;

  //We want the initial route to be congestion aware, hence
  //use the DLMCost function.
  EdgeCost &dlm = EdgeCost::getFunc(this);
  dlm.setType(EdgeCost::DLMCost);

  cout << "phase 1. routing flat GR nets" << endl;
  const bool donotallowOverflow = false;
  routeFlatNets(donotallowOverflow, dlm);
  cout << "CPU time: " << cpuTime() << " seconds " << endl;

  cout << "phase 2. routing remaining GR nets" << endl;
  const bool allowOverflow = true;
  routeNets(allowOverflow, dlm);

  cout << "[Initial routing ends]" << endl;
}
