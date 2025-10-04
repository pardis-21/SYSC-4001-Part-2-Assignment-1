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

    //Step 1: Interrupt flag is raised (hardware detects the interrupt)
        if (activity == "SYSCALL") {
            interrupt_flag = true;
            ISR = duration_intr;
            //entering kernel mode
            std::pair<std::string, int> result = intr_boilerplate(current_time, ISR, context_save_time, vectors);
            //write_output(result.next);
            execution += result.first; //add to output trace
            current_time = result.second; //update time to new time
            mode_bit = 1; 
            current_time++;

            //delay?
            int isr_delay = (ISR < (int)delays.size()? delays[ISR]: 40);
            execution += std::to_string(current_time) + ","+std::to_string(isr_delay)+",exeute ISR for device"+ std::to_string(ISR)+"\n";
            current_time += isr_delay;

        //Step 2: Context is saved into registers and PC is saved to so that when the interupt is handled, it can return to the same process
        } 

        else if (activity == "CPU") {
            if (mode_bit == 0) { //user mode
                if (interrupt_flag == true) {
                    //handle interrupt
                    //int time_to_interrupt = CPU_time - CPU;
                    CPU += context_save_time;
                    //current_time += time_to_interrupt;
                    //entering kernel mode
                    std::pair<std::string, int> result = intr_boilerplate(current_time, ISR, context_save_time, vectors);
                    write_output(result.first);
                    mode_bit = 1; 
                    current_time++;
                    interrupt_flag = false; //reset interrupt flag after handling interrupt
                    CPU = duration_intr; //remaining CPU time after interrupt is handled
                    current_time += CPU; //increment current time by remaining CPU time
                } 
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
    
    }
    input_file.close();

    write_output(execution);
   

     return 0;

}