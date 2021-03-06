// A simple memory benchmark.
// This file is in the public domain.

#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include "windows.h"

using namespace std;

const unsigned int BUILD_VER = 2;

const unsigned int HALF_ARRAY = 0x3FFFFFF + 1;
const unsigned int ARRAY_SIZE = HALF_ARRAY * 2;

int main()
{
	//Print info
	cout << "Random memory access benchmark build " << BUILD_VER << endl;
	unsigned int memsize = ARRAY_SIZE / 256 / 1024;
	cout << "Memory required: " << memsize << " MB" << endl;

	//Setup
	unsigned int *mem = new unsigned int[ARRAY_SIZE];
	LARGE_INTEGER frequency;
	LARGE_INTEGER t1, t2;
	QueryPerformanceFrequency(&frequency);

	//Populate memory array
	cout << "Filling memory array" << endl;
	for (unsigned int i = 0; i < HALF_ARRAY; i++)
	{
		//Fill low half of the array with values from the high half
		mem[i] = i + HALF_ARRAY;

		//Fill high half of the array with values for the low half
		mem[i + HALF_ARRAY] = i;
	}

	//Now we shuffle the high and low part of the array.
	//Doing it this way ensures that no element contains the index for itself
	cout << "Performing array shuffle (low)" << endl;
	for (unsigned int i = 0; i < HALF_ARRAY; i++) {
		int r = rand() % HALF_ARRAY;
		unsigned int temp = mem[i];
		mem[i] = mem[r]; 
		mem[r] = temp;
	}

	cout << "Performing array shuffle (high)" << endl;
	for (unsigned int i = HALF_ARRAY; i < ARRAY_SIZE; i++) {
		int r = (rand() % HALF_ARRAY) + HALF_ARRAY;
		unsigned int temp = mem[i];
		mem[i] = mem[r]; 
		mem[r] = temp;
	}

	//Wait for user to start benchmark
	cout << "Memory array ready. ";
	system("pause");

	//Start timer
	cout << "Starting benchmark, this can take up to a minute to complete." << endl;
	QueryPerformanceCounter(&t1);

	//Randomly jump through the array
	//This will alternate between the high and low half
	//This is certain to run to completion because no element can contain the index for itself.
	unsigned int value = mem[0];
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		//Set value equal to the value stored at an array index
		value = mem[value];
	}

	//Stop timer
	QueryPerformanceCounter(&t2);

	//Calculate time taken
	double elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;

	//Print final value to prevent loop from being optimized out
	cout << "Test completed in: " << elapsedTime << " ms" << endl;
	cout << "Final value was: " << value << endl;

	//Calculate bandwidth and latency
	double bandwidth = memsize / (elapsedTime / 1000.0f);
	cout << "Effective memory bandwidth = " << bandwidth << " MB/sec" << endl;
	
	double latency = (elapsedTime * 1000000) / ARRAY_SIZE;
	cout << "Average memory latency = " << latency << " ns" << endl;

	//Wait after displaying results
	system("pause");
    return 0;
}

