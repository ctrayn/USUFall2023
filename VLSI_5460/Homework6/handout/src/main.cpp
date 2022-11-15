#include <iostream>

using namespace std;

#include "SimpleGR.h"

void printTitle(void)
{
  cout << "SimpleGR " << SimpleGRversion << " (" << sizeof(void*) * 8 << "-bit) Compiled on "
      << COMPILETIME;
#ifdef __GNUC__
  cout << " with GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__;
#endif
  cout << endl;
}

/* $begin main */
int main(int argc, char **argv)
{
  printTitle();
  // init parameters of SimpleGR
  SimpleGRParams params(argc, argv);
  // instantiate an instance of SimpleGR
  SimpleGR simplegr(params);

  // read in the nets and create the grid input file
  simplegr.parseInput();
  simplegr.printParams();

  // perform 3-stage global routing
  simplegr.initialRouting();
  simplegr.printStatistics();

  simplegr.doRRR();
  simplegr.printStatistics();

  simplegr.greedyImprovement();
  simplegr.printStatistics(true, true);

  // output solution file
  simplegr.writeRoutes();

  return 0;
}
/* $end main */
