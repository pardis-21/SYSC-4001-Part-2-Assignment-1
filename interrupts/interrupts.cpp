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
    //the index of these elements is the device number, starting from 0
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
    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/


    //Step 1: Interrupt flag is raised (hardware detects the interrupt)
        if (activity == "SYSCALL") {
            interrupt_flag = true;
            //entering kernel mode
            CPU += duration_intr; //CPU time is incremented by the duration of the activity
            ISR = 2; //SYSCALL interrupt number is 2
            intr_boilerplate(current_time, ISR, context_save_time, vectors);
            write_output(execution);
            mode_bit = 1; 
            current_time++;

        //Step 2: Context is saved into registers and PC is saved to so that when the interupt is handled, it can return to the same process
        } 
        else if (activity == "CPU") {
            if (mode_bit == 0) { //user mode
                if (interrupt_flag == true && CPU + duration_intr > CPU_time) {
                    //handle interrupt
                    int time_to_interrupt = CPU_time - CPU;
                    CPU += time_to_interrupt;
                    current_time += time_to_interrupt;
                    //entering kernel mode
                    std::pair<std::string, int> result = intr_boilerplate(current_time, ISR, context_save_time, vectors);
                    write_output(execution);
                    mode_bit = 1; 
                    current_time++;
                    interrupt_flag = false; //reset interrupt flag after handling interrupt
                    CPU = duration_intr - time_to_interrupt; //remaining CPU time after interrupt is handled
                    current_time += CPU; //increment current time by remaining CPU time
                } 
            }
          else if (activity == "END_IO"){

            interrupt_flag = true;
        }                               
        
        else {
            interrupt_flag = false;
        }   


        //Step 3: Obtain the ISR address from the vector table
       
        
    
        

        /************************************************************************/

    input_file.close();

    write_output(execution);

   
    }
}
     return 0;

}