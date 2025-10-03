/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 * 
 * @student Pardis Ehsani pardis-21
 * @Seham Khalifa sehamkhalifa1226-beep
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include"interrupts.hpp"

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
int CPU = 0;
int context_save_time = 0;
bool interrupt_flag = true;
int ISR = 0;

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/

    //Step 1.1: Interrupt flag is raised (hardware detects the interrupt, CPU)
     if (activity == "CPU") {
        CPU = duration_intr;
        //checking to see if the CPU got a flag indicating an interrupt
        //Step 1.2: If the CPU is in user mode (mode bit = 0) and interrupt flag is raised, then handle the interrupt
        if (interrupt_flag == true && mode_bit == 0) {
            //switching to kernel mode
            mode_bit = 1;
            int time_to_interrupt =  CPU; //same as duration_intr
            CPU += time_to_interrupt;
            current_time += time_to_interrupt;
            ISR = delays.size() - 1; //last index of the vector table is for system calls
            vectors.at(ISR); //getting the ISR address from the vector table
            //entering kernel mode
            std::pair<std::string, int> result = intr_boilerplate(current_time, ISR, context_save_time, vectors);
            execution += result.first + "\n";
            current_time++;
            interrupt_flag = false; //reset interrupt flag after handling the interrupt
            }
        }
         //Step 2: Context is saved into registers and PC is saved to so that when the interupt is handled, it can return to the same process

        else if (activity == "SYSCALL") {
            //interrupt flag is raised to let the CPU know there is an interrupt to handle
            interrupt_flag = true;
            current_time += duration_intr;
            CPU += duration_intr;
            context_save_time = duration_intr;
            delays.push_back(duration_intr); //adding the syscall delay to the device table
            vectors.push_back(vectors.at(vectors.size() - 1)); //adding the syscall vector to the vector table, same as the last entry
            vectors.at(delays.size() - 1);
            ISR = delays.size() - 1; //last index of the vector table is for system calls
            
            std::pair<std::string, int> result = intr_boilerplate(current_time, ISR, context_save_time, vectors);
            execution += result.first + "\n";
            //write_output(std::to_string(result.second));
            mode_bit = 1; 
            current_time++;
       
        }
          else if (activity == "END_IO"){

            interrupt_flag = true;
        }                               
        
        else {
            interrupt_flag = false;
        }   


        //Step 3: Obtain the ISR address from the vector table
       
        
    
        //HELLLLLLLLLLLLLLOOOOOOOOOOOOOOOO

        /************************************************************************/
    
    }
    input_file.close();

    write_output(execution);
   

     return 0;

}