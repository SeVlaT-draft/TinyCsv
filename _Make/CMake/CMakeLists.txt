cmake_minimum_required (VERSION 2.6)
 
set (PROJECT TinyCsv) 

set (HEADERS
	../../Csv.h
	../../Fsm.h
	../../Input.h
	../../Output.h
	)

set (SOURCES_TEST 
	../../Tests/main.cpp
	../../Tests/TestA.cpp
	../../Tests/TestB.cpp
	../../Tests/TestC.cpp
	../../Tests/z.cpp
	)

set (SOURCES_SAMPLES 
	../../Samples/main.cpp
	../../Samples/Sample.cpp
	)
 
project (${PROJECT})
 
add_executable (CsvTest    ${SOURCES_TEST})
add_executable (CsvSamples ${SOURCES_SAMPLES})
 