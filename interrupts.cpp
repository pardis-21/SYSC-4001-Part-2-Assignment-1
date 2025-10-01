/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include"interrupts.hpp"
//#include"vector_table.txt"
//#include"device_table.txt"
/*
 * @param argc number of command line arguments
 * @param argv the command line arguments
 * @return a vector of strings (the parsed vector table)
 * 
 */

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
int current_time = 0;
int mode_bit = 0;
int CPU = 50;
int context_save_time = 0;
bool interrupt_flag = false;
int ISR = 0;
//argc = 4;
//argv = ("./interrupts", "trace.txt", "vector_table.txt", "device_table.txt");

    /******************************************************************/
    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/


//setting that the flag is false because no interrupt has been raised
//interrupt_flag == "END_IO";
while(interrupt_flag = true) {
    //Step 1: Interrupt flag is raised (hardware detects the interrupt)
        
        //intr_boilerplate(current_time, intr_num, context_save_time, vectors);
        
        //context save time is added to the current time
        execution += std::to_string(current_time) + ", " + std::to_string(1) + ", switch to kernel mode\n";
        current_time++;

        execution += std::to_string(current_time) + ", " + std::to_string(context_save_time) + ", context saved\n";

        current_time += context_save_time;

        //mode bit is set to 1 for kernel mode (for OS)
        mode_bit = 1;
       
        //Step 2: Context is saved into registers
        context_save_time = 10; //context save time is 5 time units
        current_time += context_save_time;

        execution += std::to_string(current_time) + ", " + std::to_string(context_save_time) + ", context saved\n";
        current_time += context_save_time;
        //Step 3: Obtain the ISR address from the vector table
        //vector table is opened and the address is obtained
        //std::ifstream input_vector_table;
        //input_vector_table.open(argv[2]);
        std::ifstream input_vector_table;
        input_vector_table.open("vector_table.txt");
        if (!input_vector_table.is_open()) {
            std::cerr << "Error: Unable to open file: " << argv[2] << std::endl;
            exit(1);
        }
        while(std::getline(input_vector_table, trace)) {
            vectors.push_back(trace);
        }
        input_vector_table.close();
        std::ifstream device_table;
        device_table.open(argv[3]);


        if (trace == "SYSCALL"){
            input_file.open("vector_table.txt");
            std::getline(input_file, trace);
            input_file.open(argv[2]);
            vectors.push_back(trace);
            input_file.close();

        } else if (trace == "END_IO"){
            input_file.open("vector_table.txt");
            std::getline(input_file, trace);
            std::getline(input_file, trace);
            input_file.open(argv[2]);
            vectors.push_back(trace);
            input_file.close();
        }
        else {
            std::cerr << "Error: Malformed input line: " << trace << std::endl;
            return {-1};
        }


        /************************************************************************/
    }
    input_file.close();

    write_output(execution);

   
    }
     return 0;

}