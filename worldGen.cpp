//WorldGen

#include <iostream>
#include <vector>
#include <stdlib.h>
#include "Node.h"

using namespace pathfinding;

static std::vector< std::vector< Node* > > graph;

int main() {
  /*     
  int vertices;
	int gridSize;

	std::cin >> vertices;
	std::cin >> gridSize;
	
	*/
	
	/*
	
	std::cout << "height: ";
	int height;
  std::cin >> height;
  
  std::cout << "width: ";
  int width;
  std::cin >> width;
	*/
	
	int height = 50;
	int width = 10;
	
	int max_number_of_cities = 1;
	int number_of_cities = 0;
	
	int* grid = new int[sizeof(int) * width * height];
    
  int i = 0;
  int j = 0;
  
  //generate
  srand(time(NULL));
  for(i = 0; i < height; i++)
  {
    //srand(time(NULL));
    for(j = 0; j < width; j++)
    {
      if(rand() % (height * width) == 1)
      {
        grid[i*width + j] = 10;
      }
      else
      {
        grid[i*width + j] = rand() % 3;
      }
    }
  }
  
  
  //out
  for(i = 0; i < height; i++)
  {
    //srand(time(NULL));
    for(j = 0; j < width; j++)
    {
      std::cout << grid[i*width + j] << " ";
    }
    std::cout << std::endl;
  }
  return 0;
}




