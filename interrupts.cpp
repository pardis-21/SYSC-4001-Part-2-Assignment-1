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
int CPU_time = 50;
int CPU = 0;
int context_save_time = 0;
bool interrupt_flag = false;
int ISR = 0;
argc = 4;
const argv[] = ("./interrupts", "trace.txt", "vector_table.txt", "device_table.txt");

    /******************************************************************/
    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/


//setting that the flag is false because no interrupt has been raised
context_save_time = 10;

while (activity != "END") {
    //Step 1: Interrupt flag is raised (hardware detects the interrupt)
        if (activity == "SYSCALL") {
            interrupt_flag = true;
            //entering kernel mode
            execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) + ", switch to kernel mode\n";
            mode_bit = 1;  
            CPU = 0;
            current_time++;

        //Step 2: Context is saved into registers and PC is saved to so that when the interupt is handled, it can return to the same process
        execution += std::to_string(current_time) + ", " + std::to_string(context_save_time) + ", context saved\n";
        current_time += context_save_time;  


        } 
        else if (activity == "END_IO"){

            interrupt_flag = true;
        }                               
        
        else {
            interrupt_flag = false;
        }   



        //Step 3: Obtain the ISR address from the vector table
       
        
    
        

        /************************************************************************/
    }
    input_file.close();

    write_output(execution);

   
    }
     return 0;

}