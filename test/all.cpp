/*
*
* File Name :
*
* Creation Date : 21-01-2015
* Last Modified : wo 21 jan 15:20:36 2015
* Created By : Bruno De Deken 
*
*/

#include "pdaTest.cpp"
#include "cfgTest.cpp"
#include "tmTest.cpp"
#include "cnfTest.cpp"

int main(int argc, char * argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

